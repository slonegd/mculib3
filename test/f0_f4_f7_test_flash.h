BOOST_AUTO_TEST_SUITE (test_suite_main)

struct Data {
   uint16_t d1 {1};
   uint16_t d2 {2};
};

using Sector = mcu::FLASH::Sector;

template<Sector s>
auto constexpr sector_size = mcu::FLASH::size<s>();

template<Sector s>
uint8_t memory[sector_size<s>];

template<Sector s>
size_t address = reinterpret_cast<size_t>(&memory<s>);

template<Sector s>
void erase_memory()
{
   std::fill (std::begin(memory<s>), std::end(memory<s>), 0xFF);
}

bool start_erase {false};
void erase (Sector s)
{
   start_erase = true;
   s == Sector::_0 ? erase_memory<Sector::_0>() : 
   s == Sector::_1 ? erase_memory<Sector::_1>() : 
   s == Sector::_2 ? erase_memory<Sector::_2>() : 
   s == Sector::_3 ? erase_memory<Sector::_3>() : 
   s == Sector::_4 ? erase_memory<Sector::_4>() : 
   s == Sector::_5 ? erase_memory<Sector::_5>() : 
   s == Sector::_6 ? erase_memory<Sector::_6>() : erase_memory<Sector::_7>();
}

void wait_ms (size_t ms) { while (ms--) tickUpdater.notify(); }

BOOST_AUTO_TEST_CASE (ctor)
{
   erase_memory<Sector::_7>();
   Flash<Data, Sector::_7> flash { address<Sector::_7> };

   wait_ms (100);

   BOOST_CHECK_EQUAL (flash.d1, 1);
   BOOST_CHECK_EQUAL (flash.d2, 2);
   BOOST_CHECK_EQUAL (memory<Sector::_7>[0], 0);
   BOOST_CHECK_EQUAL (memory<Sector::_7>[1], 1);
   BOOST_CHECK_EQUAL (memory<Sector::_7>[2], 1);
   BOOST_CHECK_EQUAL (memory<Sector::_7>[3], 0);
   BOOST_CHECK_EQUAL (memory<Sector::_7>[4], 2);
   BOOST_CHECK_EQUAL (memory<Sector::_7>[5], 2);
   BOOST_CHECK_EQUAL (memory<Sector::_7>[6], 3);
   BOOST_CHECK_EQUAL (memory<Sector::_7>[7], 0);
   tickUpdater.clear();
}

BOOST_AUTO_TEST_CASE (change)
{
   Flash<Data, Sector::_7> flash { address<Sector::_7> };
   flash.d1 = 0x0301;

   wait_ms (100);

   BOOST_CHECK_EQUAL (flash.d1, 0x0301);
   BOOST_CHECK_EQUAL (flash.d2, 2);
   BOOST_CHECK_EQUAL (memory<Sector::_7>[0], 0);
   BOOST_CHECK_EQUAL (memory<Sector::_7>[1], 1);
   BOOST_CHECK_EQUAL (memory<Sector::_7>[2], 1);
   BOOST_CHECK_EQUAL (memory<Sector::_7>[3], 0);
   BOOST_CHECK_EQUAL (memory<Sector::_7>[4], 2);
   BOOST_CHECK_EQUAL (memory<Sector::_7>[5], 2);
   BOOST_CHECK_EQUAL (memory<Sector::_7>[6], 3);
   BOOST_CHECK_EQUAL (memory<Sector::_7>[7], 0);
   BOOST_CHECK_EQUAL (memory<Sector::_7>[8], 1);
   BOOST_CHECK_EQUAL (memory<Sector::_7>[9], 3);
   tickUpdater.clear();
}

BOOST_AUTO_TEST_CASE (ctor_after_change)
{
   Flash<Data, Sector::_7> flash { address<Sector::_7> };

   wait_ms (100);

   BOOST_CHECK_EQUAL (flash.d1, 0x0301);
   BOOST_CHECK_EQUAL (flash.d2, 2);
   BOOST_CHECK_EQUAL (memory<Sector::_7>[0], 0);
   BOOST_CHECK_EQUAL (memory<Sector::_7>[1], 1);
   BOOST_CHECK_EQUAL (memory<Sector::_7>[2], 1);
   BOOST_CHECK_EQUAL (memory<Sector::_7>[3], 0);
   BOOST_CHECK_EQUAL (memory<Sector::_7>[4], 2);
   BOOST_CHECK_EQUAL (memory<Sector::_7>[5], 2);
   BOOST_CHECK_EQUAL (memory<Sector::_7>[6], 3);
   BOOST_CHECK_EQUAL (memory<Sector::_7>[7], 0);
   BOOST_CHECK_EQUAL (memory<Sector::_7>[8], 1);
   BOOST_CHECK_EQUAL (memory<Sector::_7>[9], 3);
   tickUpdater.clear();
}

/// если сектор кончается, то запись начинается с начала памяти
BOOST_AUTO_TEST_CASE (end_of_sector)
{
   auto& periph_flash = mcu::make_reference<mcu::Periph::FLASH>();
   periph_flash.set_erase_function (erase);
   {
      Flash<Data, Sector::_7> flash { address<Sector::_7> };
      auto qty {sector_size<Sector::_7> + 1};
      while (qty--) {
         flash.d2++;

         wait_ms (20);

      }
      tickUpdater.clear();
   } // отключили питание
   Flash<Data, Sector::_7> flash { address<Sector::_7> };
   BOOST_CHECK_EQUAL (flash.d1, 0x0301);
   BOOST_CHECK_EQUAL (flash.d2, (2 + sector_size<Sector::_7> + 1) % (UINT16_MAX + 1));
   tickUpdater.clear();
}

/// плохой вариант, потеря данных при отключеии питания при стирании сектора
BOOST_AUTO_TEST_CASE (off_when_erase)
{
   auto& periph_flash = mcu::make_reference<mcu::Periph::FLASH>();
   periph_flash.set_erase_function (erase);
   erase_memory<Sector::_7>();

   uint16_t copy_d1;

   {
      start_erase = false;
      Flash<Data, Sector::_7> flash { address<Sector::_7> };

      copy_d1 = flash.d1;

      wait_ms (100);

      auto time {0};
      while (not start_erase) {
         tickUpdater.notify();
         if (not (++time % 100)) { // every 100 ms
            flash.d1++;
            copy_d1++;
         }
      }
      BOOST_CHECK_EQUAL (flash.d1, copy_d1);
      tickUpdater.clear();
   } // отключили питание
   Flash<Data, Sector::_7> flash { address<Sector::_7> };
   BOOST_CHECK_EQUAL (flash.d1, 1);
   BOOST_CHECK (flash.d1 != copy_d1);
   tickUpdater.clear();
}

BOOST_AUTO_TEST_CASE (new_data)
{
   {
      Flash<Data, Sector::_7> flash { address<Sector::_7> };
      flash.d2 = 100;
      wait_ms (100);
      tickUpdater.clear();
   } // изменение программы, перепрошивка на устройстве

   struct NewData {
      uint16_t d1 {1};
      uint16_t d2 {2};
      uint16_t d3 {0};
   };
   Flash<NewData, Sector::_7> flash { address<Sector::_7> };

   wait_ms (100);

   BOOST_CHECK_EQUAL (flash.d1, 1);
   BOOST_CHECK_EQUAL (flash.d2, 2);
   BOOST_CHECK_EQUAL (flash.d3, 0);
}


BOOST_AUTO_TEST_SUITE_END()
