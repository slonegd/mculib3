#pragma once

#define USE_MOCK_ADC
#include "process.h"
#include "periph_adc.h"

namespace mock {

std::ostream& operator<< (std::ostream& s, mcu::ADC::DMA_mode v)
{
	return
		v == mcu::ADC::DMA_mode::one_shot ? s << "одиночный" :
		v == mcu::ADC::DMA_mode::circular ? s << "по кругу"  : s;
}

std::ostream& operator<< (std::ostream& s, mcu::ADC::Clock v)
{
	return
		v == mcu::ADC::Clock::dedicated_14MHz_clock ? s << "14MHz"  :
		v == mcu::ADC::Clock::PCLKdiv2              ? s << "PCLK/2" :
		  v == mcu::ADC::Clock::PCLKdiv4              ? s << "PCLK/4" : s;
}

std::ostream& operator<< (std::ostream& s, mcu::ADC::Resolution v)
{
	return
		v == mcu::ADC::Resolution::_12_bit ? s << "12 бит" :
		v == mcu::ADC::Resolution::_10_bit ? s << "10 бит" :
		  v == mcu::ADC::Resolution::_8_bit  ? s << "8 бит"  :
		  v == mcu::ADC::Resolution::_6_bit  ? s << "6 бит"  : s;
}

std::ostream& operator<< (std::ostream& s, mcu::ADC::Sample_time v)
{
	return
		v == mcu::ADC::Sample_time::_1_5_CLK   ? s << "1,5 такта"    :
		v == mcu::ADC::Sample_time::_7_5_CLK   ? s << "7,5 тактов"   :
		  v == mcu::ADC::Sample_time::_13_5_CLK  ? s << "13,5 тактов"  :
		  v == mcu::ADC::Sample_time::_28_5_CLK  ? s << "28,5 тактов"  : 
		  v == mcu::ADC::Sample_time::_41_5_CLK  ? s << "41,5 такт"    : 
		  v == mcu::ADC::Sample_time::_55_5_CLK  ? s << "55,5 тактов"  : 
		  v == mcu::ADC::Sample_time::_71_5_CLK  ? s << "71,5 тактов"  : 
		  v == mcu::ADC::Sample_time::_239_5_CLK ? s << "239,5 тактов" : s;
}





class ADC : public mcu::ADC
{
	Process& process { Process::make() };
	ADC() = default;
public:
	template<mcu::Periph p> static ADC& make()
	{
		static ADC adc;
		return adc;
	}
	friend std::ostream& operator<< (std::ostream&, const ADC&);

	auto& base() { return *static_cast<mcu::ADC*>(this); }

	ADC& enable()
	{
		process << *this << ": Разрешение работы" << std::endl;
		base().enable();
		return *this;     
	}

	ADC& disable()
	{
		process << *this << ": Запрет работы" << std::endl;
		base().disable();
		return *this;     
	}

	ADC& start()
	{
		process << *this << ": Запуск преобразования" << std::endl;
		base().start();
		return *this;     
	}

	ADC& dma_enable()
	{
		process << *this << ": Разрешение DMA" << std::endl;
		base().dma_enable();
		return *this;     
	}

	ADC& set (DMA_mode v)
	{
		process << *this << ": Установка режима DMA: " << v << std::endl;
		base().set(v);
		return *this;
	}

	ADC& set_continuous()
	{
		process << *this << ": Автоматический старт следующего преобразования" << std::endl;
		base().set_continuous();
		return *this;
	}

	ADC& set (Clock v)
	{
		process << *this << ": Установка частоты тактирования: " << v << std::endl;
		base().set(v);
		return *this;
	}

	ADC& set (Resolution v) 
	{
		process << *this << ": Установка разрешения: " << v << std::endl;
		base().set(v);
		return *this;
	}

	ADC& set (Sample_time v)
	{
		process << *this << ": Установка времени семплирования " << v << std::endl;
		base().set (v);
		return *this;
	}

    ADC& set_busy()
	{
		process << *this << ": Пометить как работающий" << std::endl;
		base().set_busy();
		return *this;
	}

    ADC& stop()
	{
		process << *this << ": Остановка преобразования" << std::endl;
		base().set_busy();
		return *this;
	}

    template <class Pin> 
    int set_channel(mcu::Periph p)
	{
        auto res = base().set_channel<Pin>(p);
		process << *this << ": Включение канала " << res << std::endl;
		return res;
	}


};

std::ostream& operator<< (std::ostream& s, const ADC& v)
{
	return
		&v == &ADC::make<mcu::Periph::ADC1>() ? s << "ADC1" :
		s;
}

} // namespace mock {

namespace mcu {
	SFINAE(ADC1,mock::ADC) make_reference() {return mock::ADC::make<mcu::Periph::ADC1>();}
}

namespace mock {
	auto& adc1 = REF(ADC1);
}