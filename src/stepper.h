#ifndef STEPPER_H
#define STEPPER_H

#include <hpx/hpx.hpp>

#include <vector>

#include "partition_client.h"

//typedef hpx::serialization::serialize_buffer<double> hpx_serialize_buffer;

class Stepper{
 public:
  Stepper(std::size_t partitions=10){

    hpx::naming::id_type here = hpx::find_here();
    uint my_locality_number = hpx::get_locality_id();
    uint num_localities = hpx::get_num_localities().get();

    my_partitions.reserve( partitions / num_localities );

    //distribute round robin for now
    for (uint id=my_locality_number; id < partitions; id += num_localities ){
      std::cout << "Making submesh " << id << " on Locality " << my_locality_number 
		<< std::endl;

      PartitionClient c = hpx::new_<PartitionClient>(here, id);
      my_partitions.push_back( c );
    }
  }
  // Initialize partitions
  //hpx::future<void> 
  void run(std::size_t steps); // in stepper/initialize.cpp

 private:
  std::vector<PartitionClient> my_partitions;

};
#endif
