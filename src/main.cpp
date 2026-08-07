#include <Arduino.h>
#include <GPIOESP32.h>
#include "GPIOConfig.h"
#include "UART.h"
#include "I2S/I2SESP32.h"

using namespace Sophia::HAL;
using namespace Sophia::Config;
Sophia::Platform::ArduinoESP32::GPIOESP32 gpio;
I2SESP32 i2s;


void setup() {
  Serial.begin(115200);
  delay(2000);

  UART::println("Sophia Embedded Boot");

  gpio.output(GPIOConfig::StatusLED);

  I2SConfig config;
  
  config.sampleRate =16000;

  auto result = i2s.begin(config);

  if ( result == I2SError::None ) {
    UART::println("I2S Initialized Successfully");

  } else {
    UART::println("Failed to initialize I2S");
  }

}


void loop() {

  gpio.toggle(GPIOConfig::StatusLED);
  UART::println("Sophia Capture Audio Heartbeat");
  delay(1000); 
}



















































































