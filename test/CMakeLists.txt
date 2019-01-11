cmake_minimum_required(VERSION 3.9)
project(mcu_test)

enable_testing()

find_package (Boost COMPONENTS unit_test_framework REQUIRED)
find_package (Threads)

# set (CMAKE_RUNTIME_OUTPUT_DIRECTORY build)
set (CMAKE_BUILD_TYPE Debug)

include_directories (
   ${PROJECT_SOURCE_DIR}/mocks
   ${PROJECT_SOURCE_DIR}/../src
   ${PROJECT_SOURCE_DIR}/../src/mock_class
   ${PROJECT_SOURCE_DIR}/../src/bits
   ${PROJECT_SOURCE_DIR}/../src/periph
   ${Boost_INCLUDE_DIR}
)

set (CMAKE_CXX_FLAGS "-Wno-register -Wno-overflow")


function(test MCU name)
   add_executable (${name} ${name}.cpp)
   if (MCU STREQUAL STM32F0)
      target_include_directories (${name} PRIVATE
         ${PROJECT_SOURCE_DIR}/../STM32F0_files
         ${PROJECT_SOURCE_DIR}/../STM32F0_files/CMSIS
      )
   elseif (MCU STREQUAL STM32F1)
   target_include_directories (${name} PRIVATE
      ${PROJECT_SOURCE_DIR}/../STM32F1_files
      ${PROJECT_SOURCE_DIR}/../STM32F1_files/CMSIS
   )
   elseif (MCU STREQUAL STM32F4)
      target_include_directories (${name} PRIVATE
         ${PROJECT_SOURCE_DIR}/../STM32F4_files
         ${PROJECT_SOURCE_DIR}/../STM32F4_files/CMSIS
      )
   elseif (MCU STREQUAL STM32F7)
      target_include_directories (${name} PRIVATE
         ${PROJECT_SOURCE_DIR}/../STM32F7_files
         ${PROJECT_SOURCE_DIR}/../STM32F7_files/CMSIS
      )
   endif()

   target_link_libraries (${name} 
      ${Boost_LIBRARIES}
      ${CMAKE_THREAD_LIBS_INIT}
   )

   set_target_properties (${name} PROPERTIES
      CXX_STANDARD 17
      CXX_STANDARD_REQUIRED ON
      COMPILE_FLAGS -O0
      COMPILE_OPTIONS -Wno-pedantic -Wall -Wextra
      COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK
   )
   add_test(${name} build/${name})
endfunction(test)

test (STM32F0 f0_test_periph_rcc)
test (STM32F0 f0_test_periph_flash)
test (STM32F0 f0_test_periph_gpio)
test (STM32F0 f0_test_periph_dma)
test (STM32F0 f0_test_periph_dma_stream)
test (STM32F0 f0_test_periph_usart)
test (STM32F0 f0_test_make_reference)
test (STM32F0 f0_test_flash)
test (STM32F0 f0_test_pin)

test (STM32F1 f1_test_periph_rcc)
test (STM32F1 f1_test_periph_gpio)
test (STM32F1 f1_test_periph_dma)
test (STM32F1 f1_test_periph_dma_stream)
test (STM32F1 f1_test_periph_usart)
test (STM32F1 f1_test_periph_usart_2)
test (STM32F1 f1_test_make_reference)
test (STM32F1 f1_test_pin)
# test (STM32F1 f1_test_uart)
test (STM32F1 test_modbus_slave)
test (STM32F1 test_modbus_master)
test (STM32F1 test_button)

test (STM32F4 f4_test_periph_rcc)
test (STM32F4 f4_test_periph_flash)
test (STM32F4 f4_test_periph_gpio)
test (STM32F4 f4_test_periph_dma)
test (STM32F4 f4_test_periph_dma_stream)
test (STM32F4 f4_test_periph_usart)
test (STM32F4 f4_test_make_reference)
test (STM32F4 f4_test_flash)
test (STM32F4 f4_test_pin)

test (STM32F7 f7_test_periph_rcc)
test (STM32F7 f7_test_init_clock)
test (STM32F7 f7_test_periph_flash)
test (STM32F7 f7_test_periph_gpio)
test (STM32F7 f7_test_periph_dma)
test (STM32F7 f7_test_periph_dma_stream)
test (STM32F7 f7_test_make_reference)
test (STM32F7 f7_test_flash)
test (STM32F7 f7_test_pin)

test (NULL test_reflect)
test (NULL test_table_crc)




