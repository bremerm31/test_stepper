#ifndef STEPPER_H
#define STEPPER_H

#include <hpx/hpx.hpp>

#include <vector>

#include "partition.h"

//typedef hpx::serialization::serialize_buffer<double> hpx_serialize_buffer;

class Stepper{

  // Initialize partitions
  hpx::future<void> run(std::size_t steps); // in stepper/initialize.cpp

  std::vector<Partition> my_partitions;

};
#endif
