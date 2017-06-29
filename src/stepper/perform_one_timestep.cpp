#include "../partition.h"

hpx::future<void> Partition::perform_one_timestep()
{

  hpx::future<void> send_future = outgoing.set(hpx::launch::async, _t);

  hpx::future<void> work_future = hpx::async( &(this->work()) );

  hpx::future<void> receive_future = incoming.get(_t).then(
				      [this](hpx::future<double> f)
				        this->receive( f.get() );
				      );

  //hpx::future< std::tuple<hpx::future<void>, hpx::future<void>, hpx::future<void> > >
  return hpx::when_all( send_future, work_future, receive_future ).then(
		       [this](auto f) {
			 this->update();
		        });
}
