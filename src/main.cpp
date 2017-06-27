#include <algorithm>
#include <chrono>
#include <iostream>
#include <vector>

#include "stepper.h"

#include <hpx/hpx_init.hpp>
#include <hpx/hpx.hpp>


int hpx_main(int argc, char **argv) {

  return hpx::finalize();
}

int main(int argc, char **argv){

  return hpx::init(argc,argv);
}
