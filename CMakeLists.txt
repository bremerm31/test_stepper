
# cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH=$WORK/local_install_Release/ ..

find_program(CMAKE_C_COMPILER NAMES $ENV{CC} gcc PATHS ENV PATH NO_DEFAULT_PATH)
find_program(CMAKE_CXX_COMPILER NAMES $ENV{CXX} g++ PATHS ENV PATH NO_DEFAULT_PATH)

cmake_minimum_required (VERSION 2.6)
project (test_stepper CXX C)

set(CMAKE_VERBOSE_MAKEFILE on)

if (NOT CMAKE_BUILD_TYPE)
  set (CMAKE_BUILD_TYPE DEBUG CACHE STRING
      "Choose the type of build, options are: None Debug Release."
      FORCE)
endif (NOT CMAKE_BUILD_TYPE)

# default installation
get_filename_component (default_prefix "../install" ABSOLUTE)
set (CMAKE_INSTALL_PREFIX ${default_prefix} CACHE STRING
      "Choose the installation directory; by default it installs in install."
      FORCE)

message ("CMAKE_Fortran_COMPILER full path: " ${CMAKE_Fortran_COMPILER})

#HPX
cmake_minimum_required(VERSION 2.8.4 FATAL_ERROR)
find_package(HPX REQUIRED)
include_directories(${HPX_INCLUDE_DIRS})
link_directories(${HPX_LIBRARY_DIRS})
message("HPX_INCLUDE_DIRS:" ${HPX_INCLUDE_DIRS})
message("HPX_LIBRARY_DIR:" ${HPX_LIBRARY_DIR})

get_property(inc_dirs DIRECTORY PROPERTY INCLUDE_DIRECTORIES)
message ("INCLUDE_DIRECTORIES: " ${inc_dirs})

if(NOT (CMAKE_BUILD_TYPE STREQUAL HPX_BUILD_TYPE))
  message(FATAL_ERROR "CMAKE_BUILD_TYPE doesn't match HPX_BUILD_TYPE (${HPX_BUILD_TYPE})")
endif()

if(PROFILE)
	set (CMAKE_CXX_FLAGS "-p -g ${CMAKE_CXX_FLAGS}")
	message("CXX Profiling flags enabled")
endif()

message("CMAKE_CXX_COMPILER:" ${CMAKE_CXX_COMPILER})
message("CMAKE_BUILD_TYPE:" ${CMAKE_BUILD_TYPE})

message("CMAKE_CXX_FLAGS:" ${CMAKE_CXX_FLAGS})
message("CMAKE_INCLUDE_DIRECTORIES:" ${CMAKE_INCLUDE_DIRECTORIES})

IF(CMAKE_BUILD_TYPE STREQUAL "Debug")
  message("Debug mode")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg")
ENDIF(CMAKE_BUILD_TYPE STREQUAL "Debug")

include_directories(src)

list(APPEND ARC_SRC
)

add_executable(
    test_stepper
    src/main.cpp
    src/stepper/run.cpp
    ${ARC_SRC}
)

target_link_libraries(test_stepper hpx hpx_init)

hpx_setup_target(
    test_stepper
    COMPONENT_DEPENDENCIES iostreams
    )


#add_subdirectory(src)
#add_subdirectory(include)
