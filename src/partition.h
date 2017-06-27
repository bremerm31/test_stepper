#ifndef PARTITION_H
#define PARTITION_H


//#include <iostream>
#include <hpx/hpx_main.hpp>
#include <hpx/include/iostreams.hpp>

#include <vector>

class Partition {
public:

  Partition() = default;
  Partition(std::size_t id, std::size_t grain_size=10)
    : _id(id), _t(0), _my_vector(grain_size) {}

  void perform_one_timestep() {

    hpx::cout << "Performing one timestep on " << _id << " @ " << _t++ << "\n";

  }

  void send() {

    hpx::cout << "Doing send on " << _id << " @ " << _t << "\n";
    
  }


  void receive() {

    hpx::cout << "Doing receive on " << _id << " @ " << _t << "\n";
    
  }


private:

  std::size_t _id;
  std::uint64_t _t;

  std::vector<double> _my_vector;

};

#endif
