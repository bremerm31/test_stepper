#include "partition.h"

#include <iostream>

HPX_REGISTER_COMPONENT(hpx::components::simple_component<Partition>, Partition);

HPX_REGISTER_CHANNEL(double);

HPX_REGISTER_ACTION(Partition::perf_action, partition_perf_action);

hpx::future<void> Partition::perform_one_timestep()
{

  std::cout << "In prf1ts\n";

  std::vector<hpx::future<void> > send_futures;
  send_futures.reserve( outgoing.size() );
  send_futures.push_back( hpx::async( &Partition::send, this, 0 ) );
  send_futures.push_back( hpx::async( &Partition::send, this, 1 ) );

  hpx::future<void> work_future = hpx::async( &Partition::work, this );

  std::vector<hpx::future<void> > receive_futures;
  for ( uint indx = 0; indx < 2; ++indx ) {
    hpx::future<void> receive_future = incoming[indx].get(_t).then(
							     [this](hpx::future<double> f) {
							       this->receive( f.get() );
							     }
							     );
  }

  //hpx::future< std::tuple<hpx::future<void>, hpx::future<void>, hpx::future<void> > >
  //return work_future;  
  return hpx::when_all( send_futures, work_future, receive_futures ).then(
									  [this](auto&& f) {
									    this->update();
									  });
}
