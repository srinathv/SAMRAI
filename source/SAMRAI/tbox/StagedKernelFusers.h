#ifndef included_tbox_StagedKernelFusers
#define included_tbox_StagedKernelFusers

#include "SAMRAI/SAMRAI_config.h"

#include "SAMRAI/tbox/Collectives.h"
#include "SAMRAI/tbox/KernelFuser.h"

#ifdef HAVE_RAJA
#include "RAJA/RAJA.hpp"
#endif


namespace SAMRAI {
namespace tbox {

class StagedKernelFusers
{
public:

   static StagedKernelFusers* getInstance();

#ifdef HAVE_RAJA
   template<typename Kernel>
   void enqueue(int stage, int begin, int end, Kernel&& kernel) {
      d_kernel_fusers[stage].enqueue(begin, end, kernel);
      d_active = true;
   }
#endif

   void launch()
   {
      for (auto& fuser : d_kernel_fusers) {
         fuser.second.launch();
      }
   }

   void cleanup()
   {
      for (auto& fuser : d_kernel_fusers) {
         fuser.second.cleanup();
      }
      d_active = false;
   }

   KernelFuser* getFuser(int stage)
   {
      return &d_kernel_fusers[stage];
   }

   void clearKernelFuser(int stage)
   {
      d_kernel_fusers.erase(stage);
      if (d_kernel_fusers.empty()) {
         d_active = false;
      }
   }

   void clearAllFusers()
   {
      d_kernel_fusers.clear();
      d_active = false;
   }

   bool isActive() const
   {
      return d_active;
   }

   void initialize();

   void launchAndCleanup()
   {
      if (d_active) {
         launch();
#ifdef HAVE_RAJA
         tbox::parallel_synchronize();
#endif
         cleanup();
      }
   }

protected:
   StagedKernelFusers()
   {
   }

   virtual ~StagedKernelFusers();


private:

   static void startupCallback();
   static void shutdownCallback();

   static StagedKernelFusers* s_staged_kernel_fusers_instance;

   static StartupShutdownManager::Handler
   s_startup_handler;

   std::map<int, KernelFuser> d_kernel_fusers;

   bool d_active = false;
};

}
}

#endif
