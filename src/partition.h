#ifndef PARTITION_H
#define PARTITION_H


//#include <iostream>
#include <hpx/hpx_main.hpp>
#include <hpx/include/iostreams.hpp>
#include <hpx/include/lcos.hpp>

#include <vector>


struct Partition : public hpx::components::simple_component_base<Partition>
{

  Partition() = default;
  Partition(std::size_t id, std::size_t n_ids)
    : _id(id), _t(0), n_ids(n_ids), _my_value(id)
  {}

  void perform_one_timestep() {
    hpx::cout << "perform\n";
  }

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
