#pragma once

#if defined(USE_MOCK_DMA)
using DMA_stream_t = mock::DMA_stream;
#elif defined(PERIPH_DMA_STREAM_H_)
using DMA_stream_t = mcu::DMA_stream;
#endif

#if defined(USE_MOCK_USART)
using USART_t = mock::USART;
#elif defined(PERIPH_USART_H_)
using USART_t = mcu::USART;
#endif

#if defined(USE_MOCK_FLASH)
using FLASH_ = mock::FLASH;
using mock::FLASH;
#elif defined(PERIPH_FLASH_H_)
using FLASH_ = mcu::FLASH;
using mcu::FLASH;
#endif

#if defined(USE_MOCK_NVIC)
auto& NVIC_EnableIRQ_t = mock::NVIC_EnableIRQ;
#else
auto& NVIC_EnableIRQ_t = ::NVIC_EnableIRQ;
#endif

