#ifndef PARTITION_H
#define PARTITION_H

//#include <iostream>
#include <hpx/hpx_main.hpp>
#include <hpx/include/iostreams.hpp>
#include <hpx/include/lcos.hpp>

#include <vector>

struct Partition : public hpx::components::simple_component_base<Partition>
{
  Partition() : _id(1000) {}
  Partition(std::size_t id, std::size_t n_ids)
    : _id(id), _t(0), n_ids(n_ids), _my_value(id)
  {
    hpx::cout << " In Partition ctor " << id << " of " << n_ids << "\n";


    //The convention is that the first channel number is the send submesh and the second number is the receiving submesh 
    std::vector<hpx::future<void> > set_up_in;
    {
      incoming.push_back( hpx::lcos::channel<double>(hpx::find_here() ) );
      std::string const in_channel_string = "channel"+std::to_string(id)+"_"+std::to_string(( id + 1 ) % n_ids);
      set_up_in.push_back(incoming[0].register_as(in_channel_string) );
    }
    {
      incoming.push_back( hpx::lcos::channel<double>(hpx::find_here() ) );
      std::string const in_channel_string = "channel"+std::to_string(id)+"_"+std::to_string((id +n_ids -1) % n_ids);
      set_up_in.push_back(incoming[1].register_as(in_channel_string) );
    }


    {
      outgoing.push_back( hpx::lcos::channel<double>() );
      std::string const out_channel_string = "channel"+std::to_string(( id + 1 ) % n_ids)+"_"+std::to_string(id);
      outgoing[0].connect_to(out_channel_string);
    }
    {
      outgoing.push_back( hpx::lcos::channel<double>() );
      std::string const out_channel_string = "channel"+std::to_string((id +n_ids -1) % n_ids)+"_"+std::to_string(id);
      outgoing[1].connect_to(out_channel_string);
    }

    hpx::when_all(set_up_in).get();
  }

  hpx::future<void> perform_one_timestep();

  inline
  void work() {

    hpx::cout << "Performing one timestep on " << _id << " @ " << _t << "\n";

  }

  inline
  void send(uint indx) {

    hpx::cout << "Doing send on " << _id << " @ " << _t << "\n";
    outgoing[indx].set(static_cast<double>(_id), _t);
  }

  inline
  void receive( double msg ) {

    hpx::cout << "Received msg: " << msg << " on " << _id << " @ " << _t << "\n";
  }

  inline
  void update()
  { ++_t; };

  HPX_DEFINE_COMPONENT_ACTION(Partition, perform_one_timestep, perf_action);

  std::vector<hpx::lcos::channel<double> > incoming;
  std::vector<hpx::lcos::channel<double> > outgoing;

  std::size_t _id;
  std::uint64_t _t;
  std::uint64_t n_ids;
  double _my_value;
  //  double* _my_value;

};

HPX_REGISTER_CHANNEL_DECLARATION(double);

HPX_REGISTER_ACTION_DECLARATION(Partition::perf_action, partition_perf_action);
#endif
