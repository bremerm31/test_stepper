#include "../stepper.h"


hpx::future<void> Stepper::run(std::size_t steps){

  //hpx::future<void> step_future = make_ready_future();

  // create a ready future to kick off the initialization graph
  std::vector<hpx::future<void> > step_futures(my_partitions.size());

  for(auto && step_future : step_futures){
    step_future = hpx::make_ready_future();
  }


  for(std::size_t t=0; t!=steps; ++t){
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
