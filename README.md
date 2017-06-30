# test_stepper

### Building on Rostam
`module load gcc/6.3.0 cmake/3.6.2 hwloc/1.11.3 boost/1.63.0-gcc6.3.0 openmpi/1.10.5-gcc6.3.0`
`cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo  -DCMAKE_PREFIX_PATH=$HOME/work/local_install_RelWithDebInfo/ ..`

### Building on Stampede
module load cmake gcc
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH=$WORK/local_install_Release/ ..
idev -m 120 -N 1 -n 2
ibrun ./test_stepper --hpx:run-hpx-main