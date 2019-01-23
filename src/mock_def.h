#if defined(USE_MOCK_DMA)
using DMA_stream_t = mock::DMA_stream;
#else
using DMA_stream_t = mcu::DMA_stream;
#endif

#if defined(USE_MOCK_USART)
using USART_t = mock::USART;
#else
using USART_t = mcu::USART;
#endif

#if defined(USE_MOCK_NVIC)
auto& NVIC_EnableIRQ_t = mock::NVIC_EnableIRQ;
#else
auto& NVIC_EnableIRQ_t = ::NVIC_EnableIRQ;
#endif

