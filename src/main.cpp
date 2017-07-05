#include <algorithm>
#include <chrono>
#include <iostream>
#include <vector>

#include <hpx/hpx_init.hpp>
#include <hpx/hpx.hpp>
#include <hpx/include/iostreams.hpp>
#include <hpx/include/lcos.hpp>


///////////////////////////////////////////////////////////////////////////////////
// Partition
struct Partition : public hpx::components::simple_component_base<Partition>
{
  Partition() : _var(1000) {}
  Partition(uint var) : _var(var)
  {
    hpx::cout << "In Partition ctor\n";
  }

  hpx::future<void> perform_one_timestep()
  { return hpx::async( &Partition::print_var, this );  }

  HPX_DEFINE_COMPONENT_ACTION(Partition, perform_one_timestep, perf_action);                    

  void print_var()
  { hpx::cout << _var << "\n"; }

  double _var;
};

HPX_REGISTER_CHANNEL_DECLARATION(double);                                                       
HPX_REGISTER_ACTION_DECLARATION(Partition::perf_action, partition_perf_action); 


///////////////////////////////////////////////////////////////////////////////////
// Partition Client
struct PartitionClient : hpx::components::client_base<PartitionClient, Partition>
{

  using base_type = hpx::components::client_base<PartitionClient, Partition>;

  //  PartitionClient(hpx::future<hpx::id_type>&& id)
  //  : base_type(std::move(id))
  //{}

  PartitionClient(hpx::id_type where, uint var)
    : base_type(hpx::new_<Partition>(where, var))
  {}

  hpx::future<void> perform_one_timestep() {
    Partition::perf_action act;

    hpx::cout << "in partition client\n";
    return hpx::async(act, get_id() );
  }                                                             
}; 


///////////////////////////////////////////////////////////////////////////////////
// Stepper
class Stepper{
public:                                                                                         
  Stepper(std::size_t partitions=10){

    hpx::naming::id_type here = hpx::find_here();
    uint my_locality_number = hpx::get_locality_id();
    uint num_localities = hpx::get_num_localities().get();

    my_partitions.reserve( partitions / num_localities );

    //distribute round robin          
    for (uint id=my_locality_number; id < partitions; id += num_localities ) {

      PartitionClient c(here, id);

      my_partitions.push_back( c );
    }
  }

                                                                             
  // Build the task dependency graph
  hpx::future<void> run(std::size_t steps) {

    // create a ready future to kick off the initialization graph
    std::vector<hpx::future<void> > step_futures(my_partitions.size());

    for(auto && step_future : step_futures){
      step_future = hpx::make_ready_future();
    }


    for(std::size_t t=0; t!=steps; ++t){
      hpx::cout << "timestep: " << t << "\n";
      for ( uint pid = 0; pid < my_partitions.size(); ++pid ) {
	auto& curr_part = my_partitions[pid];

	step_futures[pid] = step_futures[pid].then(
						   [&curr_part](hpx::future<void>&&)
						   {
						     return curr_part.perform_one_timestep();
						   });
      }
    }

    return hpx::when_all( step_futures );
  }
                                                      
private:                                                                                        
  std::vector<PartitionClient> my_partitions;
};

HPX_REGISTER_COMPONENT(hpx::components::simple_component<Partition>, Partition);
HPX_REGISTER_CHANNEL(double);
HPX_REGISTER_ACTION(Partition::perf_action, partition_perf_action);

///////////////////////////////////////////////////////////////////////////////////
// hpx_main and main

int hpx_main(int argc, char **argv) {

  std::size_t timesteps=2;

  Stepper myStepper;
  hpx::future<void> f = myStepper.run(timesteps);

  f.get();

  return hpx::finalize();
}

int main(int argc, char **argv){

  return hpx::init(argc,argv);
}
