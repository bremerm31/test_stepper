#include <vector>

#include <stepper/initialize.cpp>

//typedef hpx::serialization::serialize_buffer<double> hpx_serialize_buffer;

class stepper{

  // Initialize partitions
  void initialize(std::size_t steps); // in stepper/initialize.cpp

  // Update step for a single partition
  hpx::future<void> partition::perform_one_time_step(int t);


}
