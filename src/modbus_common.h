#pragma once

enum class Modbus_function : uint8_t {
      read_03       = 0x03
    , force_coil_05 = 0x05
    , write_16      = 0x10
};

enum class Modbus_error_code : uint8_t {
      wrong_func     = 0x01
    , wrong_reg      = 0x02
    , wrong_value    = 0x03
    , wrong_addr     = 0x09
    , wrong_crc      = 0x10
    , wrong_qty_byte = 0x11
    , time_out       = 0x12
};

