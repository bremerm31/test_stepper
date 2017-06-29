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
  Partition(std::size_t id, std::size_t n_ids)
    : _id(id), _t(0), _my_value(id)
  {
    std::string const in_channel_string = "channel"+std::to_string(id);
    hpx::future<void> set_up_in = incoming.register_as(in_channel_string);

    //connect to the guy on the left
    std::string const out_channel_string = "channel"+std::to_string( ( id + 1 ) % n_ids);
    outgoing.connect_to(out_channel_string);


    set_up_in.get();
  }

  void perform_one_timestep() {

    hpx::cout << "Performing one timestep on " << _id << " @ " << _t++ << "\n";

  }

  void send() {

    double message = _id;
    outgoing.set(message, _t);

    hpx::cout << "Doing send on " << _id << " @ " << _t << "\n";

  }


  void receive() {

    hpx::cout << "Doing receive on " << _id << " @ " << _t << "\n";

    hpx::future<double> incoming_message = incoming.get(_t);
    hpx::cout << "incoming_message.get() = " << incoming_message.get() << std::endl;
  }

  HPX_DEFINE_COMPONENT_ACTION(Partition, perform_one_timestep, perf_action);
  HPX_DEFINE_COMPONENT_ACTION(Partition, send, send_action);
  HPX_DEFINE_COMPONENT_ACTION(Partition, receive, receive_action);

  hpx::lcos::channel<double> incoming;
  hpx::lcos::channel<double> outgoing;

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
