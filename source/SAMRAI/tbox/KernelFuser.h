#ifndef included_tbox_KernelFuser
#define included_tbox_KernelFuser

#include "SAMRAI/tbox/ExecutionPolicy.h"
#include "SAMRAI/tbox/AllocatorDatabase.h"

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
     d_worksite(d_workgroup.run())
  {}

  template<typename Kernel>
  void enqueue(int begin, int end, Kernel&& kernel) {
     d_workpool.enqueue(RAJA::RangeSegment(begin, end), std::forward<Kernel>(kernel));
  }

  void launch()
  {
     //These might not work with umpire::TypedAllocator in the template?
//     d_workgroup = d_workpool.instantiate();
//     d_worksite = d_workgroup.run();
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
};

}
}

#endif 
