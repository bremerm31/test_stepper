#ifndef STEPPER_H
#define STEPPER_H

#include "partition.h"

#include <functional>
#include <iostream>
#include <vector>

#include <hpx/hpx_init.hpp>
#include <hpx/hpx.hpp>

#include <hpx/include/parallel_algorithm.hpp>

//depth of dependency tree
constexpr std::uint64_t nd=10;


template<typename Field>
struct TaskID
{
  int step;
  int id;
  std::unordered_map<int,std::vector<double> > out_buffs;
  Field* my_data;

  inline
  std::vector<double> get_out_buff_at(int id)
  { return out_buffs.at(id); }

  inline
  void put_out_buff_at(int id, const std::vector<double>& buff)
  { out_buffs[id] = buff; }
};

template<typename Field>
struct Stepper
{
  //  typedef std::pair<int,int> TaskID;

Stepper(uint n_ids, double FinalTime, std::function<Field(uint,uint)> ctor, std::vector<Field>* data_ptr)
  : _final_time(FinalTime), data_ptr(data_ptr)
  {
    data_ptr->reserve(n_ids);
    for ( uint id = 0; id < n_ids; ++id )
      data_ptr->push_back(ctor(id,n_ids));
  }

  TaskID<Field> update(const hpx::shared_future<TaskID<Field> >& me_future,
		std::vector<hpx::shared_future<TaskID<Field> > >&& in_buffs)
  {
    TaskID<Field> tid = me_future.get();
    //std::cout << "Start Update " << tid.step << std::endl;
    //std::vector<TaskID<Field> > to_be_placed =
    //  hpx::util::unwrapped(std::move(in_buffs));
    
    //read the neighbors' buffers
    for ( uint n =0; n < in_buffs.size(); ++n ) {
      const TaskID<Field> neigh_tid = in_buffs[n].get();
      tid.my_data->get(neigh_tid.id,tid.out_buffs.at(neigh_tid.id),
		       neigh_tid.out_buffs.at(tid.id));
    }

    //run DG kernel
    //std::cout << "INTRK" << tid.step%5 + 1 <<std::endl;
    tid.my_data->update((tid.step%5) + 1);

    //fill your out buffers
    tid.my_data->put(tid.out_buffs);

    ++tid.step;
    return tid;
  }

  hpx::future<void> do_work(std::size_t nt)
  {
    std::vector<std::vector<hpx::shared_future<TaskID<Field> > > > timestep(2);
    for ( auto& step : timestep )
      step.resize(data_ptr->size());

    const uint np = data_ptr->size();

    {
      std::vector<int> range(np);
      for ( uint i = 0; i < np; ++i)
	range[i] = i;

      //todo: get bounds right
      using hpx::parallel::execution::par;
      hpx::parallel::for_each(par,range.begin(), range.end(),
			      [&timestep,this](std::size_t i)
			      {
				TaskID<Field> tmp;
				tmp.id = i; tmp.step = 0;
				//std::cout << "In loop\n";
				(*this->data_ptr)[i].start_up(tmp.out_buffs);
				tmp.my_data = &(this->data_ptr->at(i));
				/*for (auto r_bff : tmp.out_buffs ) {
				  std::cout << "Rnk: " << r_bff.first << " " << r_bff.second[0] << std::endl;
				  }*/

				timestep[0][i] = hpx::make_ready_future(tmp);
			      });
    }

    hpx::lcos::local::sliding_semaphore sem(nd);

    //auto Op = hpx::util::unwrapped(&Stepper::update);

    uint stage_num = 0;
    for ( uint t = 0; t < nt; ++t ) {
      for ( uint irk =0; irk < 5; ++irk ) {
	auto const& current = timestep[stage_num % 2];
	auto& next = timestep[(stage_num + 1) % 2];

	for (std::size_t m = 0; m != np; ++m) {
	  std::vector<hpx::shared_future<TaskID<Field> > > receive_futures;
	  receive_futures.reserve((*data_ptr)[m].get_neighbors_size());

	  for ( int neighbor : (*data_ptr)[m].get_neighbors() ) {
	    receive_futures.push_back(current[neighbor]);
	  }

	  auto Op = std::bind(&Stepper<Field>::update,
			      *this,
			      std::placeholders::_1,
			      std::placeholders::_2);

	  next[m] = hpx::dataflow(
				  hpx::launch::async, Op,
				  current[m],
				  receive_futures
				  );
	}


	if ((stage_num % nd) == 0) {
	    next[0].then(
			 [&sem, stage_num](hpx::shared_future<TaskID<Field> >&&)
		       {
			 // inform semaphore about new lower limit
			 sem.signal(stage_num);
		       });
	}
	sem.wait(stage_num);
	++stage_num;
      }
    }

    /*
    //use a continuation to do the error analysis and clean up
    for ( uint m =0; m < np; ++m ) {
      timestep[(nt+1)%1][m] = timestep[nt%2][m].then([this](hpx::shared_future<TaskID<Field> > f)->TaskID<Field> {
	   auto tid = f.get();
	   tid.my_data->compute_l2err2(this->_final_time);
	  //data[tid.id].deallocate_mesh();
	  return tid;
	});
	}*/

    return hpx::when_all(timestep[nt%2]);
  }

  double get_global_l2e()
  {
    double gl2e = 0;
    for ( uint m = 0; m < data_ptr->size(); ++m) {
      (*data_ptr)[m].compute_l2err2(_final_time);
      gl2e += (*data_ptr)[m].get_L2e();
    }

    return gl2e;
  }

  double _final_time;
  std::vector<Field>* data_ptr;
};


#endif
