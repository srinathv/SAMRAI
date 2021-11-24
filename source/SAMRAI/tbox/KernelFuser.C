/*************************************************************************
 *
 * This file is part of the SAMRAI distribution.  For full copyright
 * information, see COPYRIGHT and LICENSE.
 *
 * Copyright:     (c) 1997-2021 Lawrence Livermore National Security, LLC
 * Description:   Singleton kernel fuser
 *
 ************************************************************************/

#include "SAMRAI/tbox/KernelFuser.h"


namespace SAMRAI {
namespace tbox {

KernelFuser* KernelFuser::s_kernel_fuser_instance(nullptr);

StartupShutdownManager::Handler
KernelFuser::s_startup_handler(
    0,
    KernelFuser::startupCallback,
    0,
    0,
    tbox::StartupShutdownManager::priorityArenaManager);

void
KernelFuser::startupCallback()
{
  KernelFuser::getFuser()->initialize();
}

void
KernelFuser::shutdownCallback()
{
   if (s_kernel_fuser_instance) {
      delete s_kernel_fuser_instance;
   }
   s_kernel_fuser_instance = nullptr;
}

KernelFuser *
KernelFuser::getFuser()
{
   if (!s_kernel_fuser_instance) {
      s_kernel_fuser_instance = new KernelFuser();
   }
   return s_kernel_fuser_instance;
}

KernelFuser::~KernelFuser()
{
}

void
KernelFuser::initialize()
{
}


}
}

