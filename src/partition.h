#ifndef PARTITION_H
#define PARTITION_H


//#include <iostream>
#include <hpx/hpx_main.hpp>
#include <hpx/include/iostreams.hpp>
#include <hpx/include/lcos.hpp>

#include <vector>


struct Partition : hpx::components::simple_component_base<Partition>
{

  Partition() = default;
  Partition(std::size_t id)
      : _id(id), _t(0), _my_value(id), buffer(hpx::find_here()) {
    std::string const channel_string = "channel"+std::to_string(id);
    //buffer.register_as(channel_string);
  }

  void perform_one_timestep() {

    hpx::cout << "Performing one timestep on " << _id << " @ " << _t++ << "\n";

  }

  void send() {

    hpx::cout << "Doing send on " << _id << " @ " << _t << "\n";


  }


  void receive() {

    hpx::cout << "Doing receive on " << _id << " @ " << _t << "\n";

  }

  hpx::lcos::channel<double> buffer;

  HPX_DEFINE_COMPONENT_ACTION(Partition, perform_one_timestep, perf_action);
  HPX_DEFINE_COMPONENT_ACTION(Partition, send, send_action);
  HPX_DEFINE_COMPONENT_ACTION(Partition, receive, receive_action);

  //  hpx::lcos::local::channel<double> incoming;

  std::size_t _id;
  std::uint64_t _t;

  double _my_value;
  //  double* _my_value;

};

HPX_REGISTER_CHANNEL_DECLARATION(double);

HPX_REGISTER_ACTION_DECLARATION(Partition::perf_action, partition_perf_action);
HPX_REGISTER_ACTION_DECLARATION(Partition::send_action, partition_send_action);
HPX_REGISTER_ACTION_DECLARATION(Partition::receive_action, partition_receive_action);

#endif
