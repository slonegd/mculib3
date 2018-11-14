#pragma once

std::stringstream result;
uint8_t buffer[2];



struct IRQn_Type{};
IRQn_Type USART1_IRQn;
IRQn_Type DMA1_Channel1_IRQn;

void NVIC_EnableIRQ(IRQn_Type IRQn){result << "Включили прерывание переферии" << '\n';}

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
   // static bool pin_;
   bool operator=  (bool v) {result << 
   (v ? "устанавливаем значение пина true" : "устанавливаем значение пина fasle") << '\n'; return v; }
   template<class Pin_, PinMode mode> static auto make()
   {Pin pin; 

      if (std::is_same_v<Pin_, TX>) {
         result << "создали пин TX" << '\n';
      } else if (std::is_same_v<Pin_, RX>) {
         result << "создали пин RX" << '\n';
      } else if (std::is_same_v<Pin_, RTS>) {
         result << "создали пин RTS" << '\n';
      } else if (std::is_same_v<Pin_, LED>) {
         result << "создали пин LED" << '\n';
      }

      return pin;
   }
};


// bool Pin::pin_{false};

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
public:
   enum DataSize      { byte8 = 0b00, word16, dword32 };
   enum Priority      { low = 0b00, medium, high, veryHi = 0b11 };
   enum DataDirection { PerToMem = 0b0, MemToPer };
   
   
   bool set_qty_transactions_{false};

   template <Periph dma = Periph::TEST_DMA> DMA_stream& clock_enable(){result << "Включаем тактирование переферии DMA" << '\n'; return *this;}

   DMA_stream& enable() {result << "Разрешаем работу DMA потока" << '\n'; return *this;}
   DMA_stream& disable(){result << "Запрещаем работу DMA потока" << '\n'; return *this;}
   DMA_stream& direction(DataDirection d)
   {
      if (d == DataDirection::MemToPer) {
         result << "Задаем направление DMA в переферию" << '\n';
      } else if (d == DataDirection::PerToMem) {
         result << "Задаем направление DMA в память" << '\n';
      }
      return *this;
   }
   DMA_stream& set_memory_adr(uint32_t){result << "Задаем адрес памяти" << '\n'; return *this;}
   DMA_stream& set_periph_adr(uint32_t){result << "Задаем адрес переферии" << '\n'; return *this;}
   DMA_stream& inc_memory(){result << "Инкременнтируем адрес памяти" << '\n'; return *this;}
   DMA_stream& size_memory(DataSize){result << "Задаем размер для памяти 8 бит" << '\n'; return *this;}
   DMA_stream& size_periph(DataSize){result << "Задаем размер для переферии 8 бит" << '\n'; return *this;}
   DMA_stream& enable_transfer_complete_interrupt(){result << "Разрешаем прерывание по отправке" << '\n'; return *this;}
   DMA_stream& set_qty_transactions(uint16_t v){result << "Устанавливаем колв-во транзакций" << '\n'; return *this;}

   IRQn_Type IRQn(Periph dma_stream){result << "Определяем номер прерывания DMA потока" << '\n'; return DMA1_Channel1_IRQn;}
};

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