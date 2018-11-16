#pragma once

std::stringstream result;
uint8_t buffer[2];
constexpr int qty = 8;

enum IRQn_Type
{
   USART1_IRQn, DMA1_Channel1_IRQn, DMA1_Channel2_IRQn, Error
};


void NVIC_EnableIRQ(IRQn_Type IRQn)
{
   if (IRQn == USART1_IRQn) {
      result << "Включили прерывание переферии USART" << '\n';
   } else if (IRQn == DMA1_Channel1_IRQn) {
      result << "Включили прерывание переферии DMA TX_stream" << '\n';
   } else if (IRQn == DMA1_Channel2_IRQn) {
      result << "Включили прерывание переферии DMA RX_stream" << '\n';
   }

}

namespace mcu {

enum class Periph {TEST_RCC, TEST_AFIO, TEST_DMA, TEST_USART, TEST_DMA_stream1, TEST_DMA_stream2};

enum class PinMode {Input, Output, USART1_TX, USART1_RX, USART2_TX, USART2_RX, USART3_TX, USART3_RX};

struct TX{};
struct RX{};
struct RTS{};
struct LED{};

class Pin 
{
public:
   enum Pins {tx, rx, rts, led};
   static int counter;
   Pins pin;
   Pin(){
      ++counter; 
      if      (counter == 1) pin = tx;
      else if (counter == 2) pin = rx;
      else if (counter == 3) pin = rts;
      else if (counter == 4) pin = led;
   }
   bool operator= (bool v) 
   {
      if (pin == tx) {
         result << (v ? "устанавливаем значение пина TX true"   : "устанавливаем значение пина TX false")  << '\n';
      } else if (pin == rx) {
          result << (v ? "устанавливаем значение пина RX true"  : "устанавливаем значение пина RX false")  << '\n';
      } else if (pin == rts){
         result << (v ? "устанавливаем значение пина RTS true"  : "устанавливаем значение пина RTS false") << '\n';
      } else if (pin == led) {
          result << (v ? "устанавливаем значение пина LED true" : "устанавливаем значение пина LED false") << '\n';
      } 
      return v;
   }

   template<class Pin_, PinMode mode> static auto make()
   {Pin pin; 

      if (std::is_same_v<Pin_, TX> and mode == PinMode::USART1_TX) {
         result << "создали пин TX в альтернативном режиме" << '\n';
      } else if (std::is_same_v<Pin_, RX> and mode == PinMode::USART1_RX) {
         result << "создали пин RX в альтернативном режиме" << '\n';
      } else if (std::is_same_v<Pin_, RTS> and mode == PinMode::Output) {
         result << "создали пин RTS в режиме выхода" << '\n';
      } else if (std::is_same_v<Pin_, LED> and mode == PinMode::Output) {
         result << "создали пин LED в режиме выхода" << '\n';
      }

      return pin;
   }
};

int Pin::counter{0};

class USART
{
public:
   enum Parity         { even = 0b0, odd     };
   enum WakeMethod     { idle = 0b0, address };
   enum DataBits       { _8   = 0b0, _9      };
   enum BreakDetection { _10bit = 0b0, _11bit};
   enum StopBits       { _1 = 0b00, _0_5, _2, _1_5 };

   enum Baudrate {
      BR9600   = 0b000,
      BR14400  = 0b001,
      BR19200  = 0b010,
      BR28800  = 0b011,
      BR38400  = 0b100,
      BR57600  = 0b101,
      BR76800  = 0b110,
      BR115200 = 0b111
   };

   static size_t clock_;

   USART& set (Parity v)  {result << (v == Parity::even ? "Задана проверка на четность" : 
                                                          "Задана проверка на нечетность") 
                                                          << '\n'; return *this;}
   USART& set (DataBits v){result << (v == DataBits::_8 ? "Установлен размер пакета 8 бит" :
                                                          "Установлен размер пакета 9 бит") 
                                                          << '\n'; return *this;}
   USART& set (StopBits v)
   { 
      if (v == StopBits::_1) {
         result << "УСтановлен один стоповый бит" << '\n';
      } else if (v == StopBits::_2) {
         result << "УСтановлено два стоповых бит" << '\n';
      }
      return *this;
   }
   USART& set (Baudrate v, size_t)
   {
      if (v == Baudrate::BR9600) {
         result << "Установлена скорость 9600 бит/с" << '\n';
      } else if (v == Baudrate::BR14400) {
         result << "Установлена скорость 14400 бит/с" << '\n';
      }
      return *this;
   }

   USART& rx_enable (){result << "USART Receiver enable"     << '\n'; return *this;}
   USART& rx_disable(){result << "USART Receiver disable"    << '\n'; return *this;}
   USART& tx_enable (){result << "USART Transmitter enable"  << '\n'; return *this;}
   USART& tx_disable(){result << "USART Transmitter disable" << '\n'; return *this;}

   USART& DMA_tx_enable (){result << "DMA enable transmitter" << '\n'; return *this;}
   USART& DMA_rx_enable (){result << "DMA enable receiver"    << '\n'; return *this;}

   USART& enable_IDLE_interrupt(){result << "USART IDLE interrupt enable" << '\n'; return *this;}
   USART& enable(){result << "USART enable" << '\n'; return *this;}

   size_t transmit_data_adr(){result << "Передаем адрес регистра данных" << '\n'; return 0xff;}

   IRQn_Type IRQn(Periph usart){result << "Определили номер прерывания USART" << '\n'; return USART1_IRQn;}
   template<Periph usart, Periph rcc = Periph::TEST_RCC> USART& clock_enable(){result << "Включаем тактирование переферии usart" << '\n'; return *this;}
   template<Periph usart, Periph rcc = Periph::TEST_RCC> static size_t clock(){result << "Передаем значение clock" << '\n'; return clock_;}

   template<Periph usart, class Pin> static constexpr Periph stream()
   {
      if constexpr (usart == Periph::TEST_USART) {
         if      constexpr (std::is_same_v<Pin, TX>) {/*result << "Определили поток №1" << '\n';*/ return Periph::TEST_DMA_stream1;}
         else if constexpr (std::is_same_v<Pin, RX>) {/*result << "Определили поток №2" << '\n';*/ return Periph::TEST_DMA_stream2;}
      }
   }
   template<Periph usart, class Pin> static constexpr PinMode pin_mode()
   {
      if constexpr (usart == Periph::TEST_USART) {
      if      constexpr (std::is_same_v<Pin, TX>) return PinMode::USART1_TX; 
      else if constexpr (std::is_same_v<Pin, RX>) return PinMode::USART1_RX;
      else return PinMode::Input;
   }
   }
   template<Periph usart, class TXpin, class RXpin> static void pin_static_assert(){result << "Проверяем назначение пинов" << '\n';}


}mock_usart;

size_t USART::clock_{1000};

template <Periph p> std::enable_if_t<p == Periph::TEST_USART, USART&> make_reference()
{
   result << "Создали ссылку на переферию usart" << '\n';
   return mock_usart;}

class DMA_stream 
{
   enum Stream {TX_stream, RX_stream};
   static int counter;
   Stream stream;
   
public:
   enum DataSize      { byte8 = 0b00, word16, dword32 };
   enum Priority      { low = 0b00, medium, high, veryHi = 0b11 };
   enum DataDirection { PerToMem = 0b0, MemToPer };
   
   DMA_stream(){
      ++counter; 
      if      (counter == 1) stream = TX_stream;
      else if (counter == 2) stream = RX_stream;
   }

   template <Periph dma = Periph::TEST_DMA> DMA_stream& clock_enable(){result << "Включаем тактирование переферии DMA" << '\n'; return *this;}

   DMA_stream& enable()
   {
      stream == TX_stream ? result << "Разрешаем работу TX_stream" << '\n':
      stream == RX_stream ? result << "Разрешаем работу RX_stream" << '\n':
      result << "Невыбран поток"   << '\n';
      return *this;
   }
   DMA_stream& disable()
   {
      stream == TX_stream ? result << "Запрещаем работу TX_stream" << '\n':
      stream == RX_stream ? result << "Запрещаем работу RX_stream" << '\n':
      result << "Невыбран поток"   << '\n';
      return *this;
   }
   DMA_stream& direction(DataDirection d)
   {
      stream == TX_stream ? (
         d == DataDirection::MemToPer ? result << "Задаем направление TX_stream в переферию" << '\n' :
         d == DataDirection::PerToMem ? result << "Задаем направление TX_stream в память" << '\n' :
         result << "Невыбрано направление"     << '\n') :
      stream == RX_stream ? (
         d == DataDirection::MemToPer ? result << "Задаем направление RX_stream в переферию" << '\n' :
         d == DataDirection::PerToMem ? result << "Задаем направление RX_stream в память" << '\n' :
         result << "Невыбрано направление"     << '\n') :
      result << "Невыбран поток"   << '\n';
      return *this;
   }
   DMA_stream& set_memory_adr(uint32_t)
   {
      stream == TX_stream ? result << "Задаем адрес памяти TX_stream" << '\n':
      stream == RX_stream ? result << "Задаем адрес памяти RX_stream" << '\n':
      result << "Невыбран поток"   << '\n';
       return *this;
   }
   DMA_stream& set_periph_adr(uint32_t)
   {
      stream == TX_stream ? result << "Задаем адрес переферии TX_stream" << '\n':
      stream == RX_stream ? result << "Задаем адрес переферии RX_stream" << '\n':
      result << "Невыбран поток"   << '\n';
      return *this;
   }
   DMA_stream& inc_memory()
   {
      stream == TX_stream ? result << "Инкременнтируем адрес памяти TX_stream" << '\n':
      stream == RX_stream ? result << "Инкременнтируем адрес памяти RX_stream" << '\n':
      result << "Невыбран поток"   << '\n';
      return *this;
   }
   DMA_stream& size_memory(DataSize v)
   {
      v == DataSize::byte8 ? (
         stream == TX_stream ? result << "Задаем размер для памяти TX_stream 8 бит" << '\n':
         stream == RX_stream ? result << "Задаем размер для памяти RX_stream 8 бит" << '\n':
         result << "Невыбран поток"   << '\n') :
      result << "Невыбран размер"  << '\n';
      return *this;
   }
   DMA_stream& size_periph(DataSize v)
   {
      v == DataSize::byte8 ? (
         stream == TX_stream ? result << "Задаем размер для переферии TX_stream 8 бит" << '\n':
         stream == RX_stream ? result << "Задаем размер для переферии RX_stream 8 бит" << '\n':
         result << "Невыбран поток"   << '\n') :
      result << "Невыбран размер"  << '\n';
      return *this;
   }
   DMA_stream& enable_transfer_complete_interrupt()
   {
      stream == TX_stream ? result << "Разрешаем прерывание TX_stream по окончании отправки" << '\n':
      result << "Невыбран поток"   << '\n';
      return *this;
   }
   DMA_stream& set_qty_transactions(uint16_t v)
   {
      stream == TX_stream ? result << "Устанавливаем колв-во транзакций для TX_stream" << '\n':
      stream == RX_stream ? result << "Устанавливаем колв-во транзакций для RX_stream" << '\n':
      result << "Невыбран поток"   << '\n';
      return *this;}

   IRQn_Type IRQn(Periph dma_stream)
   {
      if (stream == TX_stream) {
         result << "Определяем номер прерывания TX_stream" << '\n'; return DMA1_Channel1_IRQn;
      } else if (stream == RX_stream) {
         result << "Определяем номер прерывания RX_stream" << '\n'; return DMA1_Channel2_IRQn;
      }

      return Error;
   }

   uint16_t qty_transactions_left()
   {
      return 0;
   }
};

int DMA_stream::counter{0};

DMA_stream tx_stream;
DMA_stream rx_stream;

template <Periph dma_stream> std::enable_if_t<dma_stream == Periph::TEST_DMA_stream1, DMA_stream&> make_reference()
{
   result << "Создали ссылку на переферию tx stream" << '\n';
   return tx_stream;
}
template <Periph dma_stream> std::enable_if_t<dma_stream == Periph::TEST_DMA_stream2, DMA_stream&> make_reference() 
{
   result << "Создали ссылку на переферию rx stream" << '\n';
   return rx_stream;}

} //namespace mcu