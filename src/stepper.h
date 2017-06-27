#ifndef STEPPER_H
#define STEPPER_H

#include <hpx/hpx.hpp>

#include <vector>

#include "partition.h"

//typedef hpx::serialization::serialize_buffer<double> hpx_serialize_buffer;

class Stepper{
 public:
  Stepper(std::size_t partitions=10){
    for (std::size_t id=0; id!=partitions; id++){
      Partition partition(id);
      my_partitions.push_back(partition);
    }
  }
  // Initialize partitions
  hpx::future<void> run(std::size_t steps); // in stepper/initialize.cpp

 private:
  std::vector<Partition> my_partitions;

};
#endif
