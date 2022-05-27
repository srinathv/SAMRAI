/*************************************************************************
 *
 * This file is part of the SAMRAI distribution.  For full copyright
 * information, see COPYRIGHT and LICENSE.
 *
 * Copyright:     (c) 1997-2022 Lawrence Livermore National Security, LLC
 * Description:   Main program to test cell patch data operations.
 *
 ************************************************************************/

#include "SAMRAI/SAMRAI_config.h"

#include <typeinfo>

#include "SAMRAI/tbox/SAMRAI_MPI.h"
#include "SAMRAI/tbox/PIO.h"

#include "SAMRAI/tbox/SAMRAIManager.h"

#include "SAMRAI/hier/Box.h"
#include "SAMRAI/hier/BoxContainer.h"
#include "SAMRAI/pdat/CellData.h"
#include "SAMRAI/pdat/CellIndex.h"
#include "SAMRAI/pdat/CellIterator.h"
#include "SAMRAI/hier/Index.h"
#include "SAMRAI/hier/IntVector.h"
#include <string>

#include "SAMRAI/tbox/Utilities.h"
#include "SAMRAI/tbox/MathUtilities.h"

using namespace SAMRAI;

int main(
   int argc,
   char* argv[]) {

   tbox::SAMRAI_MPI::init(&argc, &argv);
   tbox::SAMRAIManager::initialize();
   tbox::SAMRAIManager::startup();

   const tbox::Dimension dim(2);

   int num_failures = 0;

   hier::Index lo(0,0);
   hier::Index hi(1,1);
   hier::IntVector ghosts(dim, 0);
   hier::Box box(lo, hi, hier::BlockId(0));
   pdat::CellData<double> cdata(box, 1, ghosts);

   cdata.fillAll(-1.0);
   cdata.print(cdata.getGhostBox(), tbox::pout);

   cdata.fillAll(5.27, cdata.getGhostBox());

   pdat::CellIterator icend(pdat::CellGeometry::end(cdata.getGhostBox()));
   for (pdat::CellIterator c(pdat::CellGeometry::begin(cdata.getGhostBox()));
        c != icend; ++c) {
      if (cdata(*c) < 0.0) {
         ++num_failures;
      }
   }

   cdata.print(cdata.getGhostBox(), tbox::pout);

   if (num_failures == 0) {
      tbox::pout << "\nPASSED:  cell fillall" << std::endl;
   } else {
      tbox::perr << "\nFAILED:  cell fillall" << std::endl;
   }


   tbox::SAMRAIManager::shutdown();
   tbox::SAMRAIManager::finalize();
   tbox::SAMRAI_MPI::finalize();

   return num_failures;
}

