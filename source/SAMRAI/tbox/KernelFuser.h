#ifndef included_tbox_KernelFuser
#define included_tbox_KernelFuser

#include "SAMRAI/SAMRAI_config.h"

#ifdef HAVE_UMPIRE
#include "umpire/ResourceManager.hpp"
#include "umpire/TypedAllocator.hpp"
#endif

#include "SAMRAI/tbox/ExecutionPolicy.h"
#include "SAMRAI/tbox/AllocatorDatabase.h"
#include "SAMRAI/tbox/Utilities.h"

#ifdef HAVE_RAJA
#include "RAJA/RAJA.hpp"
#endif


#ifdef HAVE_UMPIRE
#include "umpire/Allocator.hpp"
#include "umpire/TypedAllocator.hpp"
#endif

namespace SAMRAI {
namespace tbox {

class KernelFuser
{
public:

  static KernelFuser* getFuser();

#ifdef HAVE_RAJA
  template<typename Kernel>
  void enqueue(int begin, int end, Kernel&& kernel) {
     if (d_launched) {
        TBOX_ERROR("KernelFuser Error: Cannont enqueue until cleanup called after previous launch.");
     }

     d_workpool.enqueue(RAJA::RangeSegment(begin, end), std::forward<Kernel>(kernel));
  }
#endif

  void launch()
  {
     if (d_launched) {
        TBOX_ERROR("KernelFuser Error: This KernelFuser already launched.");
     }

#ifdef HAVE_RAJA
     d_workgroup = d_workpool.instantiate();
     d_worksite = d_workgroup.run();
#endif

     d_launched = true;
  }

  void cleanup()
  {
#ifdef HAVE_RAJA
     d_workpool.clear();
     d_workgroup.clear();
     d_worksite.clear();
#endif
     d_launched = false;
  }

  bool launched() const
  {
     return d_launched;
  }

  void initialize();

protected:
  KernelFuser() :
#ifdef HAVE_RAJA
     d_workpool(AllocatorDatabase::getDatabase()->getKernelFuserAllocator()),
     d_workgroup(d_workpool.instantiate()),
     d_worksite(d_workgroup.run()),
#endif
     d_launched(false)
  {
  }


   virtual ~KernelFuser();


private:
#ifdef HAVE_UMPIRE
  using Allocator = umpire::TypedAllocator<char>;
#else
  using Allocator = ResourceAllocator;
#endif

#ifdef HAVE_RAJA
  using Policy = typename tbox::detail::policy_traits< tbox::policy::parallel >::WorkGroupPolicy;
  using WorkPool  = RAJA::WorkPool <Policy, int, RAJA::xargs<>, Allocator>;
  using WorkGroup = RAJA::WorkGroup<Policy, int, RAJA::xargs<>, Allocator>;
  using WorkSite  = RAJA::WorkSite <Policy, int, RAJA::xargs<>, Allocator>;
#endif

   static void startupCallback();
   static void shutdownCallback();

   static KernelFuser* s_kernel_fuser_instance;

   static StartupShutdownManager::Handler
   s_startup_handler;

#ifdef HAVE_RAJA
  WorkPool d_workpool;
  WorkGroup d_workgroup;
  WorkSite d_worksite;
#endif

  bool d_launched;
};

}
}

#endif 
