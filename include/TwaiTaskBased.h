#ifndef TWAI_TASK_BASED_HEADER_H
#define TWAI_TASK_BASED_HEADER_H

#pragma once
#include <driver/twai.h>
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include <freertos/task.h>
#include <Arduino.h>
// Not instance based since we want to have on set of handle events and call
// send from multiple classes easily.
class TwaiTaskBased {
public:
  using RxCallback = void (*)(const twai_message_t &msg);
  using TxCallback = void (*)(bool success);
  // Method used to initialize the CAN tranceiver
  static bool begin(gpio_num_t txPin, gpio_num_t rxPin,
                    uint32_t baudrate = 500000, uint32_t rxTaskStack = 4096,
                    UBaseType_t rxTaskPrio = 10, UBaseType_t txTaskPrio = 9);
  // Method to register callbacks for both transmit and receive.
  static void onReceive(RxCallback cb);
  static void onTransmit(TxCallback cb);
  // Method used to send CAN bus messages.
  static bool send(const twai_message_t &msg,
                   TickType_t timeout = portMAX_DELAY);

private:
  static bool _started;
  // Private references to task handles
  static TaskHandle_t _rxTaskHandle;
  static TaskHandle_t _txTaskHandle;
  // Private reference to the transmit queue
  static QueueHandle_t _txQueue;
  // Private references to the callbacks
  static RxCallback _rxCallback;
  static TxCallback _txCallback;
  // Private references to the recieve and transmit methods
  static void rxTask(void *);
  static void txTask(void *);
  // Private reference to the timing config object.
  static twai_timing_config_t timingFromBaudrate(uint32_t baudrate);
};
#endif