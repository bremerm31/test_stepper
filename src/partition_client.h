#include <hpx/include/components.hpp>

#include "partition.h"

struct PartitionClient : hpx::components::client_base<PartitionClient, Partition>
{

  using base_type = hpx::components::client_base<PartitionClient, Partition>;


  PartitionClient(hpx::id_type where, std::size_t id, std::size_t n_ids)
    : base_type(hpx::new_<Partition>(where, id, n_ids) )
  {}

  hpx::future<void> perform_one_timestep()
  {
    Partition::perf_action act;

    hpx::cout << "in partition client\n";
    hpx::future<void> f =  hpx::async(hpx::launch::sync, act, get_id() );
    f.get();

    return hpx::make_ready_future();
  }

};
