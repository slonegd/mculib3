cmake_minimum_required(VERSION 3.5)
project(mcu_test)

# find_package (Threads)
# find_package (Boost COMPONENTS system REQUIRED)

# set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ../build)
# set (CMAKE_BUILD_TYPE Debug)

# add_executable(join_server main.cpp)

# include_directories(join_server ${Boost_INCLUDE_DIRS})

# set_target_properties (join_server PROPERTIES
#    CXX_STANDARD 14
#    CXX_STANDARD_REQUIRED ON
#    COMPILE_OPTIONS -Wpedantic -Wall -Wextra
#    COMPILE_FLAGS -O2
#    # COMPILE_DEFINITIONS BOOST_ASIO_ENABLE_HANDLER_TRACKING
#    INCLUDE_DIRECTORIES ${PROJECT_SOURCE_DIR}/otus12
# )


# target_link_libraries (join_server
#    ${Boost_LIBRARIES}
#    ${CMAKE_THREAD_LIBS_INIT}
# )

enable_testing()

find_package (Boost COMPONENTS unit_test_framework REQUIRED system REQUIRED)
find_package (Threads)

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY build)
set (CMAKE_BUILD_TYPE Debug)

include_directories (
   ${PROJECT_SOURCE_DIR}/../src
   ${PROJECT_SOURCE_DIR}/../src/bits
   ${PROJECT_SOURCE_DIR}/../src/periph
   ${Boost_INCLUDE_DIR}
)

set(CMAKE_CXX_FLAGS "-Wno-register")

function(test_F0 name)
   add_executable (${name} ${name}.cpp)

   include_directories (
      ${PROJECT_SOURCE_DIR}/../STM32F0_files
      ${PROJECT_SOURCE_DIR}/../STM32F0_files/CMSIS
   )

   target_link_libraries (${name} 
      ${Boost_LIBRARIES}
      ${CMAKE_THREAD_LIBS_INIT}
   )

   set_target_properties (${name} PROPERTIES
      CXX_STANDARD 17
      CXX_STANDARD_REQUIRED ON
      COMPILE_OPTIONS -Wpedantic -Wall -Wextra
      COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK
   )
   add_test(${name} build/${name})
endfunction(test)



test_F0(f0_boosttest_rcc)




