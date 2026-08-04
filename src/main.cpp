#include <Arduino.h>
#include "GPIO.h"
#include "GPIOConfig.h"
#include "UART.h"

using namespace Sophia::HAL;
using namespace Sophia::Config;


void setup() {
  Serial.begin(115200);
  delay(2000);

  UART::println("Sophia Embedded Boot");

  GPIO::output(GPIOConfig::StatusLED);

}


void loop() {

  // GPIO::toggle(GPIOConfig::StatusLED);
  UART::println("Sophia Capture Audio Heartbeat");
  delay(1000); 
}



















































































