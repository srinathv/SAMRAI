# When provided to CMake using the -C argument, the command line must
# also have a -DTPL_DIR:PATH=[third party library path] argument to 
# identify the location of the installed raja, umpire, camp libraries.

set(ENABLE_MPI On CACHE BOOL "")
set(CMAKE_CXX_COMPILER "/usr/tce/packages/cce-tce/cce-16.0.0/bin/crayCC" CACHE PATH "")
set(CMAKE_C_COMPILER "/usr/tce/packages/cce-tce/cce-16.0.0/bin/craycc" CACHE PATH "")
set(CMAKE_Fortran_COMPILER "/usr/tce/packages/rocmcc-tce/rocmcc-5.5.1/bin/amdflang" CACHE PATH "")
set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING "")
set(ENABLE_OPENMP Off CACHE BOOL "")
set(ENABLE_HDF5 Off CACHE BOOL "")
set(ENABLE_CUDA Off CACHE BOOL "")
set(ENABLE_HIP On CACHE BOOL "")
set(HIP_ROOT_DIR "/opt/rocm-5.5.1/hip" CACHE PATH "")
# set(HIP_HIPCC_FLAGS "-D__HIP_ROCclr__ -D__HIP_PLATFORM_AMD__ -DCAMP_USE_PLATFORM_DEFAULT_STREAM -D__HIP_ARCH_GFX90A__=1 --rocm-path=/opt/rocm-5.4.3 -std=c++14 -x hip --offload-arch=gfx90a" CACHE STRING "")
set(CMAKE_HIP_ARCHITECTURES "gfx90a" CACHE STRING "")
set(GPU_TARGETS "gfx90a" CACHE STRING "")
set(AMDGPU_TARGETS "gfx90a" CACHE STRING "")
set(ENABLE_RAJA On CACHE BOOL "")
set(RAJA_DIR "${TPL_DIR}/raja/lib/cmake/raja" CACHE PATH "")
set(ENABLE_UMPIRE On CACHE BOOL "")
set(umpire_DIR "${TPL_DIR}/umpire/lib/cmake/umpire" CACHE PATH "")
set(camp_DIR "${TPL_DIR}/camp/lib/cmake/camp" CACHE PATH "")
set(CMAKE_EXPORT_COMPILE_COMMANDS On CACHE BOOL "")
set(CMAKE_CXX_FLAGS "-O3 -std=c++14" CACHE STRING "")
set(BLT_CXX_STD "c++14" CACHE STRING "")
set(BLT_FORTRAN_FLAGS "" CACHE STRING "")
set(ENABLE_TESTS On CACHE BOOL "") 
set(ENABLE_SAMRAI_TESTS On CACHE BOOL "")
set(MPI_CXX_COMPILER "/usr/tce/packages/cray-mpich-tce/cray-mpich-8.1.26-rocmcc-5.5.1//bin/mpicxx" CACHE PATH "")
set(MPI_CXX_LINK_FLAGS "-Wl,-rpath,/opt/cray/pe/mpich/8.1.26/ofi/amd/5.0/lib -Wl,-rpath,/opt/cray/libfabric/2.0/lib64:/opt/cray/pe/pmi/6.1.10/lib:/opt/cray/pe/pals/1.2.11/lib:/opt/cray/pe/cce/16.0.0/cce/x86_64/lib:/opt/rocm-5.5.1/llvm/lib")

set(HIP_HIPCC_FLAGS "-D__HIP_ROCclr__ -D__HIP_PLATFORM_AMD__ -DCAMP_USE_PLATFORM_DEFAULT_STREAM -std=c++14 -x hip --offload-arch=gfx90a" CACHE STRING "")
