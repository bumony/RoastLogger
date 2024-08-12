// platform_config.h

#pragma once

#ifdef USE_PICO
#define CLK 2
#define btCS 1
#define etCS 5
#define SO 3
#define relay 9
void platformSpecificMethod();
void setup();
#elif defined(USE_ESP32)
#include <WiFi.h>
#define CLK 2 //todo 需要修改具体的引脚
#define btCS 1
#define etCS 5
#define SO 3
#define relay 9
#define LED_PIN 2
void platformSpecificMethod();
#else
#error "Unsupported platform"
#endif