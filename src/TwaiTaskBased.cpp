#include <TwaiTaskBased.h>

// Static member definitions
bool TwaiTaskBased::_started = false;
// Create task handles for both sending and receiving
TaskHandle_t TwaiTaskBased::_rxTaskHandle = nullptr;
TaskHandle_t TwaiTaskBased::_txTaskHandle = nullptr;
// Create the transmit queue
QueueHandle_t TwaiTaskBased::_txQueue = nullptr;

TwaiTaskBased::RxCallback TwaiTaskBased::_rxCallback = nullptr;
TwaiTaskBased::TxCallback TwaiTaskBased::_txCallback = nullptr;

bool TwaiTaskBased::begin(gpio_num_t txPin, gpio_num_t rxPin,
                       uint32_t baudrate,
                       uint32_t rxTaskStack,
                       UBaseType_t rxTaskPrio,
                       UBaseType_t txTaskPrio) {
  if (_started)
    return false;

  twai_general_config_t g_config =
      TWAI_GENERAL_CONFIG_DEFAULT(txPin, rxPin, TWAI_MODE_NORMAL);

  twai_timing_config_t t_config = timingFromBaudrate(baudrate);
  twai_filter_config_t f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL();

  if (twai_driver_install(&g_config, &t_config, &f_config) != ESP_OK)
    return false;

  if (twai_start() != ESP_OK)
    return false;

  _txQueue = xQueueCreate(16, sizeof(twai_message_t));
  if (!_txQueue)
    return false;

  xTaskCreatePinnedToCore(rxTask, "twai_rx",
                          rxTaskStack, nullptr,
                          rxTaskPrio, &_rxTaskHandle, 0);

  xTaskCreatePinnedToCore(txTask, "twai_tx",
                          4096, nullptr,
                          txTaskPrio, &_txTaskHandle, 0);

  _started = true;
  return true;
}

void TwaiTaskBased::onReceive(RxCallback cb) {
  _rxCallback = cb;
}

void TwaiTaskBased::onTransmit(TxCallback cb) {
  _txCallback = cb;
}

bool TwaiTaskBased::send(const twai_message_t &msg, TickType_t timeout) {
  if (!_started)
    return false;

  return xQueueSend(_txQueue, &msg, timeout) == pdTRUE;
}

void TwaiTaskBased::rxTask(void *) {
  twai_message_t msg;

  while (true) {
    esp_err_t res = twai_receive(&msg, portMAX_DELAY);
    if (res == ESP_OK && _rxCallback) {
      _rxCallback(msg);
    }
  }
}

void TwaiTaskBased::txTask(void *) {
  twai_message_t msg;

  while (true) {
    if (xQueueReceive(_txQueue, &msg, portMAX_DELAY) == pdTRUE) {
      esp_err_t res = twai_transmit(&msg, portMAX_DELAY);
      if (_txCallback) {
        _txCallback(res == ESP_OK);
      }
    }
  }
}

twai_timing_config_t TwaiTaskBased::timingFromBaudrate(uint32_t baudrate) {
  switch (baudrate) {
  case 125000:
    return TWAI_TIMING_CONFIG_125KBITS();
  case 250000:
    return TWAI_TIMING_CONFIG_250KBITS();
  case 500000:
    return TWAI_TIMING_CONFIG_500KBITS();
  case 1000000:
    return TWAI_TIMING_CONFIG_1MBITS();
  default:
    return TWAI_TIMING_CONFIG_500KBITS();
  }
}
