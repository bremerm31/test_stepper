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
    : _id(id), _t(0), n_ids(n_ids), _my_value(id), incoming(hpx::find_here())
  {
    hpx::cout << " In Partition ctor " << id << " of " << n_ids << "\n";

    std::string const in_channel_string = "channel"+std::to_string(id);
    //hpx::future<void> set_up_in = incoming.register_as(in_channel_string);

    //connect to the guy on the left
    std::string const out_channel_string = "channel"+std::to_string( ( id + 1 ) % n_ids);
    //outgoing.connect_to(out_channel_string);


    //set_up_in.get();
  }

  hpx::future<void> perform_one_timestep();


  void work() {

    hpx::cout << "Performing one timestep on " << _id << " @ " << _t << "\n";

  }

  void send() {

    hpx::cout << "Doing send on " << _id << " @ " << _t << "\n";
    //    outgoing.set(static_cast<double>(_id), _t );

  }

  void receive( double msg ) {

    hpx::cout << "Received msg: " << msg << " @ " << _t << "\n";
  }

  void update()
  { ++_t; };

  HPX_DEFINE_COMPONENT_ACTION(Partition, perform_one_timestep, perf_action);

  hpx::lcos::channel<double> incoming;
  hpx::lcos::channel<double> outgoing;

  std::size_t _id;
  std::uint64_t _t;
  std::uint64_t n_ids;
  double _my_value;
  //  double* _my_value;

};

HPX_REGISTER_CHANNEL_DECLARATION(double);

HPX_REGISTER_ACTION_DECLARATION(Partition::perf_action, partition_perf_action);
#endif
