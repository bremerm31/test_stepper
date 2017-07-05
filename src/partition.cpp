#include "partition.h"

#include <iostream>

HPX_REGISTER_COMPONENT(hpx::components::simple_component<Partition>, Partition);

HPX_REGISTER_CHANNEL(double);

HPX_REGISTER_ACTION(Partition::perf_action, partition_perf_action);

hpx::future<void> Partition::perform_one_timestep()
{

  std::cout << "In prf1ts\n";

  hpx::future<void> send_future = hpx::async( &Partition::send, this );

  hpx::future<void> work_future = hpx::async( &Partition::work, this );

  hpx::future<void> receive_future = incoming.get(_t).then(
  				        [this](hpx::future<double> f) {
  				          this->receive( f.get() );
  					}
					);

  //hpx::future< std::tuple<hpx::future<void>, hpx::future<void>, hpx::future<void> > >
  //return work_future;  
  return hpx::when_all( send_future, work_future, receive_future ).then(
									[this](auto&& f) {
									  this->update();
									});
}
