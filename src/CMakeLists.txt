cmake_minimum_required(VERSION 2.6)
#set(VTK_DIR ".")

## GET R-specific ENV variables
set(R_HOME $ENV{R_HOME})
SET(RLIB "${R_HOME}/lib")
set (RSCRIPT "${R_HOME}/bin/Rscript")
set(RCPP_DIR "$ENV{RCPP_DIR}/include")
set(RCPPEIGEN_DIR "$ENV{RCPPEIGEN_DIR}/include")
set(R_INCLUDE_DIR "$ENV{R_INCLUDE_DIR}")
set(CXX "$ENV{CXX}")


set(CXXFLAGS "$ENV{CXXFLAGS}")
set(CPPFLAGS "$ENV{CPPFLAGS}")
# set(BLAS_LIBS "$ENV{BLAS_LIBS}")
# set(LAPACK_LIBS "$ENV{LAPACK_LIBS}")
## set variables accordingly
set(CMAKE_CXX_FLAGS "${CXXFLAGS}")
set(CMAKE_C_FLAGS "${CPPFLAGS} ${CFLAGS}")
SET(CMAKE_CXX_COMPILER::string "${CXX}")
set(CMAKE_VERBOSE_MAKEFILE OFF)

project(Rpcl CXX)

## look for pcl and ask statismos cmake file about hdf5 whereabouts
FIND_PACKAGE(PCL 1.6 REQUIRED)

IF( NOT PCL_FOUND )
  MESSAGE(FATAL_ERROR "You need to build pcl" )
ENDIF()


LINK_DIRECTORIES(${RLIB} ${PCL_LIBRARY_DIRS})
include_directories(BEFORE ${R_INCLUDE_DIR} ${RCPP_DIR} ${RCPPEIGEN_DIR} ${PCL_INCLUDE_DIRS} "../inst/include")
add_library(Rpcl SHARED pclIO.cpp Rexport.cpp)
# set(CMAKE_SHARED_LINKER_FLAGS "${BLAS_LIBS} ${LAPACK_LIBS}")
# option(LINK_STATIC_LIBSTDC "link statically against libstdc++" OFF)
# if (LINK_STATIC_LIBSTDC)
#   set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -static-libstdc++")
# endif(LINK_STATIC_LIBSTDC)

target_link_libraries(Rpcl ${PCL_LIBRARIES} R)
