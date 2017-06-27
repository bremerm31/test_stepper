#include "../stepper.h"


void Stepper::initialize(std::size_t steps){

  //hpx::future<void> step_future = make_ready_future();

  // create a ready future to kick off the initialization graph
  std::vector<hpx::future<void> > step_futures;

  for(auto && step_future : step_futures){
    step_future = make_ready_future();
  }

  for(std::size_t t=0; t!=steps; ++t){

    // loop over all partitions
    for (auto && step_future : step_futures){
      // use .then to add continuation to the future when it's ready.
      // this creates the execution graph
      step_future = step_future.then(
          [t](hpx::future<void> &&){
            return perform_one_time_step(t);
          });
    }
  }
  step_future.get();
}
