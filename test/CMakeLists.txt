cmake_minimum_required(VERSION 3.9)
project(mcu_test)

enable_testing()

find_package (Boost COMPONENTS unit_test_framework REQUIRED)
# find_package (Boost COMPONENTS  REQUIRED)
find_package (Threads)

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY build)
set (CMAKE_BUILD_TYPE Debug)

include_directories (
   ${PROJECT_SOURCE_DIR}/mocks
   ${PROJECT_SOURCE_DIR}/../src
   ${PROJECT_SOURCE_DIR}/../src/mock_class
   ${PROJECT_SOURCE_DIR}/../src/bits
   ${PROJECT_SOURCE_DIR}/../src/periph
   ${Boost_INCLUDE_DIRS}
   # ${Boost_INCLUDE_DIRS}
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

# test (STM32F0 test_periph_rcc_f0)
# test (STM32F1 test_periph_rcc_f1)
# test (STM32F4 test_periph_rcc_f4)
# test (STM32F7 test_periph_rcc_f7)

# test (STM32F7 test_init_clock_f7)

# test (STM32F0 test_periph_flash_f0)
# test (STM32F1 test_periph_flash_f1)
# test (STM32F4 test_periph_flash_f4)
# test (STM32F7 test_periph_flash_f7)

# test (STM32F0 test_periph_gpio_f0)
# test (STM32F1 test_periph_gpio_f1)
# test (STM32F4 test_periph_gpio_f4)
# test (STM32F7 test_periph_gpio_f7)

# test (STM32F0 test_periph_dma_f0)
# test (STM32F1 test_periph_dma_f1)
# test (STM32F4 test_periph_dma_f4)
# test (STM32F7 test_periph_dma_f7)

# test (STM32F0 test_periph_dma_stream_f0)
# test (STM32F1 test_periph_dma_stream_f1)
# test (STM32F4 test_periph_dma_stream_f4)
# test (STM32F7 test_periph_dma_stream_f7)

# test (STM32F0 test_periph_usart_f0)
# test (STM32F1 test_periph_usart_f1)
# test (STM32F1 test_periph_usart_2_f1)
# test (STM32F4 test_periph_usart_f4)

# test (STM32F4 test_periph_tim_f4)
# test (STM32F0 test_periph_adc_f0)

# test (STM32F0 test_make_reference_f0)
# test (STM32F1 test_make_reference_f1)
# test (STM32F4 test_make_reference_f4)
# test (STM32F7 test_make_reference_f7)

# test (STM32F0 test_pin_f0)
# test (STM32F1 test_pin_f1)
# test (STM32F4 test_pin_f4)
# test (STM32F7 test_pin_f7)

# test (STM32F0 test_flash_f0)
# test (STM32F1 test_flash_f1)
# test (STM32F4 test_flash_f4)
# test (STM32F7 test_flash_f7)

# test (STM32F0 test_adc_f0)

# test (STM32F1 test_uart_f1)

# test (STM32F1 test_modbus_slave)
# test (STM32F1 test_modbus_master)
# test (STM32F1 test_button)


# test (STM32F1 test_hd44780)
# test (STM32F1 test_timers)
# test (STM32F1 test_delay)

# test (STM32F4 test_pwm)

# test (NULL test_reflect)
# test (NULL test_table_crc)
# test (NULL test_string_buffer)
# test (NULL test_net_buffer)
# test (NULL test_vector)
# test (NULL test_heap_new)
# test (NULL test_heap_multinew)
# test (NULL test_heap_delete)
# test (NULL test_heap_fragmentation)
# test (NULL test_list)
# test (NULL test_function)
# test (NULL test_dynarray)

# test (NULL test_)



