#include "partition.h"


HPX_REGISTER_COMPONENT(hpx::components::simple_component<Partition>, Partition);

HPX_REGISTER_ACTION(Partition::perf_action, partition_perf_action);
HPX_REGISTER_ACTION(Partition::send_action, partition_send_action);
HPX_REGISTER_ACTION(Partition::receive_action, partition_receive_action);
