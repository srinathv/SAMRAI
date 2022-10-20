/*************************************************************************
 *
 * This file is part of the SAMRAI distribution.  For full copyright
 * information, see COPYRIGHT and LICENSE.
 *
 * Copyright:     (c) 1997-2022 Lawrence Livermore National Security, LLC
 * Description:   Abstract base class for kernel fuser operations
 *
 ************************************************************************/

#ifndef included_tbox_ScheduleOpsStrategy
#define included_tbox_ScheduleOpsStrategy

#include "SAMRAI/SAMRAI_config.h"


namespace SAMRAI {
namespace tbox {

/**
 */

class ScheduleOpsStrategy
{
public:
   /**
    * The constructor does nothing interesting.
    */
   ScheduleOpsStrategy();

   /**
    * The virtual destructor does nothing interesting.
    */
   virtual ~ScheduleOpsStrategy();

   virtual void preCommunicate() = 0;

   virtual void preCopy() = 0;
   virtual void prePack() = 0;
   virtual void preUnpack() = 0;

   virtual void postCopy() = 0;
   virtual void postPack() = 0;
   virtual void postUnpack() = 0;

   virtual void postCommunicate() = 0;


private:
   ScheduleOpsStrategy(
      const ScheduleOpsStrategy&);              // not implemented
   ScheduleOpsStrategy&
   operator = (
      const ScheduleOpsStrategy&);              // not implemented

};

}
}

#endif
