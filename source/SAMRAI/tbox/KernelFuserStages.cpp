/*************************************************************************
 *
 * This file is part of the SAMRAI distribution.  For full copyright
 * information, see COPYRIGHT and LICENSE.
 *
 * Copyright:     (c) 1997-2021 Lawrence Livermore National Security, LLC
 * Description:   Singleton kernel fuser
 *
 ************************************************************************/

#include "SAMRAI/tbox/KernelFuserStages.h"


namespace SAMRAI {
namespace tbox {

KernelFuserStages* KernelFuserStages::s_kernel_fuser_stages_instance(nullptr);

StartupShutdownManager::Handler
KernelFuserStages::s_startup_handler(
    0,
    KernelFuserStages::startupCallback,
    0,
    0,
    tbox::StartupShutdownManager::priorityArenaManager);

void
KernelFuserStages::startupCallback()
{
  KernelFuserStages::getFuserStages()->initialize();
}

void
KernelFuserStages::shutdownCallback()
{
   if (s_kernel_fuser_stages_instance) {
      delete s_kernel_fuser_stages_instance;
   }
   s_kernel_fuser_stages_instance = nullptr;
}

KernelFuserStages *
KernelFuserStages::getFuserStages()
{
   if (!s_kernel_fuser_stages_instance) {
      s_kernel_fuser_stages_instance = new KernelFuserStages();
   }
   return s_kernel_fuser_stages_instance;
}

KernelFuserStages::~KernelFuserStages()
{
}

void
KernelFuserStages::initialize()
{
}


}
}

