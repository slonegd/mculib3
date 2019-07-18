#include "literals.h"

BOOST_AUTO_TEST_SUITE (test_suite_main)

struct Data {
   uint16_t d1 {1};
   uint16_t d2 {2};
};

void wait_ms (size_t ms) { while (ms--) tickUpdater.notify(); }
template<class Function>
void do_every_ms (size_t ms, Function f)
{
   while (ms--) {
      tickUpdater.notify();
      f();
   }
}

BOOST_AUTO_TEST_CASE (ctor)
{
   mock::erase (mock::Sector::_7);
   mock::erase (mock::Sector::_6);
   Flash<Data, mock::Sector::_7, mock::Sector::_6> flash {};

   wait_ms (100);

   BOOST_CHECK_EQUAL (flash.d1, 1);
   BOOST_CHECK_EQUAL (flash.d2, 2);
   BOOST_CHECK_EQUAL (mock::memory<mock::Sector::_6>[0], 0);
   BOOST_CHECK_EQUAL (mock::memory<mock::Sector::_6>[1], 1);
   BOOST_CHECK_EQUAL (mock::memory<mock::Sector::_6>[2], 1);
   BOOST_CHECK_EQUAL (mock::memory<mock::Sector::_6>[3], 0);
   BOOST_CHECK_EQUAL (mock::memory<mock::Sector::_6>[4], 2);
   BOOST_CHECK_EQUAL (mock::memory<mock::Sector::_6>[5], 2);
   BOOST_CHECK_EQUAL (mock::memory<mock::Sector::_6>[6], 3);
   BOOST_CHECK_EQUAL (mock::memory<mock::Sector::_6>[7], 0);
}

BOOST_AUTO_TEST_CASE (change)
{
   mock::erase (mock::Sector::_7);
   mock::erase (mock::Sector::_6);
   Flash<Data, mock::Sector::_7, mock::Sector::_6> flash {};
   wait_ms (100);

   flash.d1 = 0x0301;
   wait_ms (100);

   BOOST_CHECK_EQUAL (flash.d1, 0x0301);
   BOOST_CHECK_EQUAL (flash.d2, 2);
   BOOST_CHECK_EQUAL (mock::memory<mock::Sector::_6>[0], 0);
   BOOST_CHECK_EQUAL (mock::memory<mock::Sector::_6>[1], 1);
   BOOST_CHECK_EQUAL (mock::memory<mock::Sector::_6>[2], 1);
   BOOST_CHECK_EQUAL (mock::memory<mock::Sector::_6>[3], 0);
   BOOST_CHECK_EQUAL (mock::memory<mock::Sector::_6>[4], 2);
   BOOST_CHECK_EQUAL (mock::memory<mock::Sector::_6>[5], 2);
   BOOST_CHECK_EQUAL (mock::memory<mock::Sector::_6>[6], 3);
   BOOST_CHECK_EQUAL (mock::memory<mock::Sector::_6>[7], 0);
   BOOST_CHECK_EQUAL (mock::memory<mock::Sector::_6>[8], 1);
   BOOST_CHECK_EQUAL (mock::memory<mock::Sector::_6>[9], 3);
}

BOOST_AUTO_TEST_CASE (ctor_after_change)
{
   {
      mock::erase (mock::Sector::_7);
      mock::erase (mock::Sector::_6);
      Flash<Data, mock::Sector::_7, mock::Sector::_6> flash {};
      wait_ms (100);
      flash.d1 = 0x0301;
      wait_ms (100);
   }

   Flash<Data, mock::Sector::_7, mock::Sector::_6> flash {};

   wait_ms (100);

   BOOST_CHECK_EQUAL (flash.d1, 0x0301);
   BOOST_CHECK_EQUAL (flash.d2, 2);
   BOOST_CHECK_EQUAL (mock::memory<mock::Sector::_6>[0], 0);
   BOOST_CHECK_EQUAL (mock::memory<mock::Sector::_6>[1], 1);
   BOOST_CHECK_EQUAL (mock::memory<mock::Sector::_6>[2], 1);
   BOOST_CHECK_EQUAL (mock::memory<mock::Sector::_6>[3], 0);
   BOOST_CHECK_EQUAL (mock::memory<mock::Sector::_6>[4], 2);
   BOOST_CHECK_EQUAL (mock::memory<mock::Sector::_6>[5], 2);
   BOOST_CHECK_EQUAL (mock::memory<mock::Sector::_6>[6], 3);
   BOOST_CHECK_EQUAL (mock::memory<mock::Sector::_6>[7], 0);
   BOOST_CHECK_EQUAL (mock::memory<mock::Sector::_6>[8], 1);
   BOOST_CHECK_EQUAL (mock::memory<mock::Sector::_6>[9], 3);
}

BOOST_AUTO_TEST_CASE (ctor_other_sector)
{
   mock::erase (mock::Sector::_7);
   mock::erase (mock::Sector::_6);
   mock::memory<mock::Sector::_6>[0] = 0;
   mock::memory<mock::Sector::_6>[1] = 1;
   mock::memory<mock::Sector::_6>[2] = 1;
   mock::memory<mock::Sector::_6>[3] = 0;
   mock::memory<mock::Sector::_6>[4] = 2;
   mock::memory<mock::Sector::_6>[5] = 2;
   mock::memory<mock::Sector::_6>[6] = 3;
   mock::memory<mock::Sector::_6>[7] = 0;
   mock::memory<mock::Sector::_6>[8] = 1;
   mock::memory<mock::Sector::_6>[9] = 3;

   Flash<Data, mock::Sector::_7, mock::Sector::_6> flash {};

   BOOST_CHECK_EQUAL (flash.d1, 0x0301);
   BOOST_CHECK_EQUAL (flash.d2, 2);
}

// тест на стирание плохой памяти после инициализации
BOOST_AUTO_TEST_CASE (ctor_bad_sector)
{
   mock::erase (mock::Sector::_7);
   mock::erase (mock::Sector::_6);
   mock::memory<mock::Sector::_6>[0] = 0;
   mock::memory<mock::Sector::_6>[1] = 1;
   mock::memory<mock::Sector::_6>[2] = 1;
   mock::memory<mock::Sector::_6>[3] = 0;
   mock::memory<mock::Sector::_6>[4] = 2;
   mock::memory<mock::Sector::_6>[5] = 2;
   mock::memory<mock::Sector::_6>[6] = 3;
   mock::memory<mock::Sector::_6>[7] = 0;
   mock::memory<mock::Sector::_6>[8] = 1;
   mock::memory<mock::Sector::_6>[9] = 3;

   mock::memory<mock::Sector::_7>[0] = 10;

   auto& periph_flash = mcu::make_reference<mcu::Periph::FLASH>();
   periph_flash.set_erase_function (mock::erase);

   Flash<Data, mock::Sector::_7, mock::Sector::_6> flash {};

   BOOST_CHECK_EQUAL (flash.d1, 0x0301);
   BOOST_CHECK_EQUAL (flash.d2, 2);

   wait_ms (50);
   BOOST_CHECK_EQUAL (mock::memory<mock::Sector::_7>[0], 0xFF);
}

/// если сектор кончается, то запись начинается с другого сектора
BOOST_AUTO_TEST_CASE (end_of_sector)
{
   struct Data {
      uint16_t d1 {1};
      uint8_t  d2 {2};
   };
   {
      mock::erase (mock::Sector::_7);
      mock::erase (mock::Sector::_6);
      Flash<Data, mock::Sector::_7, mock::Sector::_6> flash {};
      wait_ms (100);
      flash.d1 = 0x0301;
      wait_ms (100);
   }
   auto& periph_flash = mcu::make_reference<mcu::Periph::FLASH>();
   periph_flash.set_erase_function (mock::erase);
   auto qty {mock::sector_size<mock::Sector::_6> / 2 + 1};
   {
      Flash<Data, mock::Sector::_7, mock::Sector::_6> flash {};
      for (auto i = qty; i; i--) {
         flash.d2++;
         wait_ms (30);
      }
   } // отключили питание
   Flash<Data, mock::Sector::_7, mock::Sector::_6> flash {};
   BOOST_CHECK_EQUAL (flash.d1, 0x0301);
   BOOST_CHECK_EQUAL (flash.d2, (2 + qty) % (UINT8_MAX + 1));
   BOOST_CHECK_EQUAL (mock::memory<mock::Sector::_6>[0], 0xFF);
   BOOST_CHECK_EQUAL (mock::memory<mock::Sector::_6>[1], 0xFF);
}

// /// плохой вариант, потеря данных при отключеии питания при стирании сектора
// BOOST_AUTO_TEST_CASE (off_when_erase)
// {
//    auto& periph_flash = mcu::make_reference<mcu::Periph::FLASH>();
//    periph_flash.set_erase_function (mock::erase);
//    mock::erase (mock::Sector::_7);
//    mock::erase (mock::Sector::_6);

//    uint16_t copy_d1;

//    {
//       mock::start_erase = false;
//       Flash<Data, mock::Sector::_7, mock::Sector::_6> flash {};

//       copy_d1 = flash.d1;

//       wait_ms (100);

//       auto time {0};
//       while (not mock::start_erase) {
//          tickUpdater.notify();
//          if (not (++time % 90)) { // every 100 ms
//             flash.d1++;
//             copy_d1++;
//          }
//       }
//       BOOST_CHECK_EQUAL (flash.d1, copy_d1);
//    } // отключили питание
//    Flash<Data, mock::Sector::_7, mock::Sector::_6> flash {};
//    BOOST_CHECK_EQUAL (flash.d1, 1);
//    BOOST_CHECK (flash.d1 != copy_d1);
// }

// BOOST_AUTO_TEST_CASE (new_data)
// {
//    {
//       Flash<Data, mock::Sector::_7, mock::Sector::_6> flash {};
//       flash.d2 = 100;
//       wait_ms (100);
//    } // изменение программы, перепрошивка на устройстве

//    struct NewData {
//       uint16_t d1 {1};
//       uint16_t d2 {2};
//       uint16_t d3 {0};
//    };
//    Flash<NewData, mock::Sector::_7> flash {};

//    wait_ms (100);

//    BOOST_CHECK_EQUAL (flash.d1, 1);
//    BOOST_CHECK_EQUAL (flash.d2, 2);
//    BOOST_CHECK_EQUAL (flash.d3, 0);
// }

// BOOST_AUTO_TEST_CASE (process)
// {
//    mock::erase (mock::Sector::_7);
//    mock::erase (mock::Sector::_6);

//    auto& process = mock::Process::make();
//    process.clear();

//    Flash<Data, mock::Sector::_7, mock::Sector::_6> flash {};

//    BOOST_CHECK_EQUAL (process.str(), "");

//    auto write = [&]() {
//       BOOST_CHECK_EQUAL (process.str(), 
//          "разблокировка памяти для записи\n"
//          "переключение в режим записи\n"
//       #if defined(STM32F4) or defined(STM32F7)
//          "установка размера записи 16 бит\n"
//          "разрешение прерывания по окончанию записи\n"
//       #endif
//       );
//       process.clear();
//       // условное время записи от начала 3 мс для теста
//       do_every_ms (2_ms, [&](){ BOOST_CHECK_EQUAL (process.str(), ""); });
//       wait_ms (1);
//       BOOST_CHECK_EQUAL (process.str(), 
//          "сброс флага окончания записи\n"
//          "блокировка памяти для записи\n"
//       );
//       process.clear();
//    };

//    auto first_write_data_to_flash = [&]() {
//       auto repeat {sizeof(Data)};
//       while (repeat--) {
//          wait_ms (1);
//          write();
//          do_every_ms (1_ms, [&](){ BOOST_CHECK_EQUAL (process.str(), ""); });
//       }
//    };

//    first_write_data_to_flash();
//    do_every_ms (100_ms, [&](){ BOOST_CHECK_EQUAL (process.str(), ""); });


//    flash.d1++;

//    do {
//       wait_ms (1);
//    } while ( not process.str().size() );

//    write();

//    do_every_ms (100_ms, [&](){ BOOST_CHECK_EQUAL (process.str(), ""); });

//    mock::start_erase = false;
//    auto time {0};
//    while (not mock::start_erase) {
//       process.clear();
//       wait_ms (1);
//       if (not (++time % 100)) { // every 100 ms
//          flash.d1++;
//       }
//    }

//    BOOST_CHECK_EQUAL (process.str(),
//       "разблокировка памяти для записи\n"
//       "запуск стирания сектора 7\n"
//    );
//    process.clear();

//    do_every_ms (2_ms, [&](){ BOOST_CHECK_EQUAL (process.str(), ""); });
//    wait_ms (1);
//    BOOST_CHECK_EQUAL (process.str(), 
//       "сброс флага окончания записи\n"
//       "блокировка памяти для записи\n"
//    );
//    process.clear();

//    first_write_data_to_flash();

//    do_every_ms (100_ms, [&](){ BOOST_CHECK_EQUAL (process.str(), ""); });
// }


BOOST_AUTO_TEST_SUITE_END()
