#ifndef included_tbox_KernelFuserStages
#define included_tbox_KernelFuserStages

#include "SAMRAI/SAMRAI_config.h"

#include "SAMRAI/tbox/KernelFuser.h"

#ifdef HAVE_RAJA
#include "RAJA/RAJA.hpp"
#endif


namespace SAMRAI {
namespace tbox {

class KernelFuserStages
{
public:

   static KernelFuserStages* getFuserStages();

#ifdef HAVE_RAJA
   template<typename Kernel>
   void enqueue(int stage, int begin, int end, Kernel&& kernel) {
      d_kernel_fusers[stage].enqueue(begin, end, kernel);
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
   }

   KernelFuser* getDefaultFuser()
   {
      return &d_kernel_fusers[-99999];
   }

   KernelFuser* getFuser(int stage)
   {
      return &d_kernel_fusers[stage];
   }

   void clearKernelFuser(int stage)
   {
      d_kernel_fusers.erase(stage);
   }

   void clearAllFusers()
   {
      d_kernel_fusers.clear();
   }

   void initialize();

protected:
   KernelFuserStages()
   {
   }

   virtual ~KernelFuserStages();


private:

   static void startupCallback();
   static void shutdownCallback();

   static KernelFuserStages* s_kernel_fuser_stages_instance;

   static StartupShutdownManager::Handler
   s_startup_handler;

   std::map<int, KernelFuser> d_kernel_fusers;
};

}
}

#endif
