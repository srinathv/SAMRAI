#ifndef included_tbox_KernelFuser
#define included_tbox_KernelFuser

#include "SAMRAI/SAMRAI_config.h"

#include "SAMRAI/tbox/ExecutionPolicy.h"
#include "SAMRAI/tbox/AllocatorDatabase.h"
#include "SAMRAI/tbox/Utilities.h"

#ifdef HAVE_RAJA
#include "RAJA/RAJA.hpp"
#endif

namespace SAMRAI {
namespace tbox {

class KernelFuser
{
public:
  KernelFuser() :
     d_workpool(AllocatorDatabase::getDatabase()->getStreamAllocator()),
     d_workgroup(d_workpool.instantiate()),
     d_worksite(d_workgroup.run()),
     d_launched(false)
  {
  }

  template<typename Kernel>
  void enqueue(int begin, int end, Kernel&& kernel) {
     if (d_launched) {
        TBOX_ERROR("KernelFuser Error: Cannont enqueue until cleanup called after previous launch.");
     }

     d_workpool.enqueue(RAJA::RangeSegment(begin, end), std::forward<Kernel>(kernel));
  }

  void launch()
  {
     if (d_launched) {
        TBOX_ERROR("KernelFuser Error: This KernelFuser already launched.");
     }

     d_workgroup = d_workpool.instantiate();
     d_worksite = d_workgroup.run();

     d_launched = true;
  }

  void cleanup()
  {
     d_workpool.clear();
     d_workgroup.clear();
     d_worksite.clear();
     d_launched = false;
  }

  bool launched() const
  {
     return d_launched;
  }
 

private:
#ifdef HAVE_UMPIRE
  using Allocator = umpire::TypedAllocator<char>;
#else
  using Allocator = ResourceAllocator;
#endif

  using Policy = typename tbox::detail::policy_traits< tbox::policy::parallel >::WorkGroupPolicy;
  using WorkPool  = RAJA::WorkPool <Policy, int, RAJA::xargs<>, Allocator>;
  using WorkGroup = RAJA::WorkGroup<Policy, int, RAJA::xargs<>, Allocator>;
  using WorkSite  = RAJA::WorkSite <Policy, int, RAJA::xargs<>, Allocator>;

  WorkPool d_workpool;
  WorkGroup d_workgroup;
  WorkSite d_worksite;

  bool d_launched;
};

}
}

#endif 
