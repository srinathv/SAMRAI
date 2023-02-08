/*************************************************************************
 *
 * This file is part of the SAMRAI distribution.  For full copyright
 * information, see COPYRIGHT and LICENSE.
 *
 * Copyright:     (c) 1997-2023 Lawrence Livermore National Security, LLC
 * Description:   Singleton container of kernel fusers
 *
 ************************************************************************/

#ifndef included_tbox_StagedKernelFusers
#define included_tbox_StagedKernelFusers

#include "SAMRAI/SAMRAI_config.h"

#include "SAMRAI/tbox/Collectives.h"
#include "SAMRAI/tbox/KernelFuser.h"

#ifdef HAVE_RAJA
#include "RAJA/RAJA.hpp"
#endif

#include <map>

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
      d_enqueued = true;
   }
#endif

   void launch()
   {
      for (auto& fuser : d_kernel_fusers) {
         fuser.second.launch();
         d_launched = (d_launched || fuser.second.launched()); 
      }
   }

   void cleanup()
   {
      for (auto& fuser : d_kernel_fusers) {
         fuser.second.cleanup();
      }
      d_enqueued = false;
      d_launched = false;
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
      d_launched = false;
      d_enqueued = false;
   }

   bool launched()
   {
      return d_launched;
   }

   bool enqueued()
   {
      return d_enqueued;
   }

   void initialize();

   void launchAndCleanup()
   {
      launch();
#ifdef HAVE_RAJA
      if (d_launched) {
         tbox::parallel_synchronize();
      }
#endif
      cleanup();
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

   bool d_launched = false;
   bool d_enqueued = false;

};

}
}

#endif
