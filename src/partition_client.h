#include <hpx/include/components.hpp>

#include "partition.h"

struct PartitionClient : hpx::components::client_base<PartitionClient, Partition>
{

  using base_type = hpx::components::client_base<PartitionClient, Partition>;


  PartitionClient(hpx::future<hpx::id_type>&& id)
    : base_type(std::move(id))
  {}

  hpx::future<void> perform_one_timestep() {
    Partition::perf_action act;

    hpx::cout << "in partition client\n";
    hpx::future<void> f =  hpx::async(act, get_id() );
    f.get();

    return hpx::make_ready_future();
  }

};
