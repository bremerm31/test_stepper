#include "partition.h"


HPX_REGISTER_COMPONENT(hpx::components::simple_component<Partition>, Partition);

HPX_REGISTER_CHANNEL(double);

HPX_REGISTER_ACTION(Partition::perf_action, partition_perf_action);
