#include <vector>

//typedef hpx::serialization::serialize_buffer<double> hpx_serialize_buffer;

boost::uint64_t t;

void initialize(std::size_t steps){
  hpx::future<void> step_future = make_ready_future();

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


// Update step for a single partition
hpx::future<void> partition::perform_one_time_step(int t){

  // Call get on the channel (left_from and right_from)
  // which produces a future
  hpx::future<void> left_boundary_future = channel_left_from.get(t);
  hpx::future<void> right_boundary_future = channel_right_from.get(t);

  // How do I get current state?


  // question: if it's an hpx::future<void>, why does it return a std::vector<double>?

  // Call .get on the incoming futures
  std::vector<double> data = channel_left_from.get();
  std::vector<double> data = channel_right_from.get();

  return when_all(left_boundary_future, right_boundary_future, interior_future);
}
