#include <algorithm>
#include <chrono>
#include <iostream>
#include <vector>

#include "stepper.h"
#include "partition.h"

#include <hpx/hpx_init.hpp>
#include <hpx/hpx.hpp>


int hpx_main(int argc, char **argv) {

  std::size_t timesteps=2;

  Stepper myStepper;
  hpx::future<void> f = myStepper.run(timesteps);

  f.get();

  return hpx::finalize();
}

int main(int argc, char **argv){

  return hpx::init(argc,argv);
}
