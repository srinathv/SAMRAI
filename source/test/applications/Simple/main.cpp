/*************************************************************************
 *
 * This file is part of the SAMRAI distribution.  For full copyright
 * information, see COPYRIGHT and COPYING.LESSER.
 *
 * Copyright:     (c) 1997-2016 Lawrence Livermore National Security, LLC
 * Description:   Main program for SAMRAI Linear Advection example problem.
 *
 ************************************************************************/

#include "SAMRAI/SAMRAI_config.h"

// Header for application-specific algorithm/data structure object

// Headers for major algorithm/data structure objects

#include "SAMRAI/appu/VisItDataWriter.h"
#include "SAMRAI/mesh/BergerRigoutsos.h"
#include "SAMRAI/geom/CartesianGridGeometry.h"
#include "SAMRAI/mesh/GriddingAlgorithm.h"
#include "SAMRAI/algs/HyperbolicLevelIntegrator.h"
#include "SAMRAI/mesh/CascadePartitioner.h"
#include "SAMRAI/hier/PatchHierarchy.h"
#include "SAMRAI/mesh/StandardTagAndInitialize.h"
#include "SAMRAI/algs/TimeRefinementIntegrator.h"
#include "SAMRAI/algs/TimeRefinementLevelStrategy.h"
#include "SAMRAI/mesh/TileClustering.h"
#include "SAMRAI/tbox/NVTXUtilities.h"

// Headers for basic SAMRAI objects

#include "SAMRAI/hier/VariableDatabase.h"
#include "SAMRAI/hier/PatchLevel.h"
#include "SAMRAI/tbox/SAMRAIManager.h"
#include "SAMRAI/tbox/BalancedDepthFirstTree.h"
#include "SAMRAI/tbox/Database.h"
#include "SAMRAI/tbox/InputDatabase.h"
#include "SAMRAI/tbox/InputManager.h"
#if 1
#include "SAMRAI/tbox/SAMRAI_MPI.h"
#endif
#include "SAMRAI/tbox/PIO.h"
#include "SAMRAI/tbox/RestartManager.h"
#include "SAMRAI/tbox/Utilities.h"

#ifndef _MSC_VER
#include <unistd.h>
#endif

#include <sys/stat.h>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <fstream>
#include <memory>
#include <iomanip>

#include "SAMRAI/tbox/StartupShutdownManager.h"

#if defined(HAVE_CUDA)
#include <cuda_runtime.h>
#include <cuda_profiler_api.h>
#endif

using namespace std;
using namespace SAMRAI;

int main(
    int argc,
    char* argv[])
{

  /*
   * Initialize tbox::MPI.
   */

  tbox::SAMRAI_MPI::init(&argc, &argv);
  tbox::SAMRAIManager::initialize();

  /*
   * Initialize SAMRAI, enable logging, and process command line.
   */
  tbox::SAMRAIManager::startup();

  bool success = true;

  tbox::pout << "\nHello world!" << std::endl;

  umpire::ResourceManager& rm = umpire::ResourceManager::getInstance();
#if defined(HAVE_HIP) || defined(HAVE_CUDA)
  auto allocator = rm.getAllocator(umpire::resource::Device);
#else
  auto allocator = rm.getAllocator(umpire::resource::Host);
#endif

  // This creates two ArrayData objects of size 5x5x5.
  // ArrayData is basically a raw array (length 125 in this case) plus
  // metadata to map entries in that array to a 3D mesh.
  // The allocator above determines the memory resource where the arrays live.
  hier::Index box_lo(0,0,0);
  hier::Index box_hi(4,4,4); // 0 to 4 inclusive makes the size 5x5x5
  hier::Box box(box_lo, box_hi, hier::BlockId(0));
  pdat::ArrayData<int> src(box, 1, allocator);
  pdat::ArrayData<int> dst(box, 1, allocator);
  pdat::ArrayData<int> sum(box, 1, allocator);

  // Initialize the src array uniformly to value 2023, then copy to dst.
  // These should be GPU operations if the builds with CUDA or HIP are
  // working properly.
  src.fillAll(2023);
  dst.copy(src, box);

#if defined(HAVE_HIP) || defined(HAVE_CUDA)
  // The ArrayView object provides a view into the ArrayData object that
  // can be used within the lambda for a RAJA kernel. It's easier to
  // declare these as auto, but I wrote out the type for one of them for
  // clarity.
  pdat::ArrayView<3, int> src_view = src.getView<3>();
  auto dst_view = dst.getView<3>();
  auto sum_view = sum.getView<3>();

  // parallel_for_all with a Box argument is a wrapper around RAJA that
  // causes the kernel to thread on 3 dimensions. Output should show it
  // is 2023 everywhere, but the display will be unordered due to threading.
  hier::parallel_for_all(box, [=] SAMRAI_HOST_DEVICE(int i, int j, int k) {
    printf("dst_view(%d,%d,%d) = %d\n", i, j, k, dst_view(i,j,k));
  });

  // Sum of dst + src should be 4046 for all entries
  hier::parallel_for_all(box, [=] SAMRAI_HOST_DEVICE(int i, int j, int k) {
    sum_view(i,j,k) = dst_view(i,j,k) + src_view(i,j,k);
  });

  // This gets the raw arrays from the ArrayData.
  //
  // One could add more things here to do operations on these arrays.
  // RAJA kernels to change values or do arithmetic, explicit CUDA or
  // HIP code, etc.
  int* src_array = src.getPointer();
  int* dst_array = dst.getPointer();
  int* sum_array = sum.getPointer();

  // We can examine the raw array to see the values that were set in the
  // earlier kernel that filled sum_view. Arguments "0, n" are the loop
  // extents when treating this as an ordinary array.
  int n = box.size();
  hier::parallel_for_all(0, n, [=] SAMRAI_HOST_DEVICE(int i) {
    printf("uniform sum_array[%d] = %d\n", i, sum_array[i]);
  });

  // Do something to make the array no longer have uniform values.
  hier::parallel_for_all(0, n, [=] SAMRAI_HOST_DEVICE(int i) {
    sum_array[i] -= i;
  });

  // Print out the array again to see the new values.
  hier::parallel_for_all(0, n, [=] SAMRAI_HOST_DEVICE(int i) {
    printf("nonuniform sum_array[%d] = %d\n", i, sum_array[i]);
  });

#endif

  // For now, success is defined by getting here without crashing or hanging. 
  if (success) { 
    tbox::pout << "\nPASSED:  Simple" << std::endl;
  } else {
    tbox::pout << "\nFAILED:  Simple" << std::endl;
  }

  tbox::SAMRAIManager::shutdown();

  tbox::SAMRAIManager::finalize();
  tbox::SAMRAI_MPI::finalize();
  return(0);
}
