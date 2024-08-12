#ifdef USE_ESP32
#include <ESP8266WiFi.h>
#include "ModbusTCP.h"
#include <WiFiManager.h>
#include <WiFi.h>
#endif
#include "ModbusRtu.h"
#include <Arduino.h>
#include "platform_config.h"
#include "max6675.h"

uint16_t au16data[16] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

MAX6675 bt(CLK, btCS, SO);
MAX6675 et(CLK, etCS, SO);

/**
 *  Modbus object declaration
 *  u8id : node id = 0 for master, = 1..247 for slave
 *  u8serno : serial port (use 0 for Serial)
 *  u8txenpin : 0 for RS-232 and USB-FTDI
 *               or any pin number > 1 for RS-485
 */
#ifdef USE_PICO
Modbus slave(1, 0, 0); // this is slave @1 and RS-232 or USB-FTDI
#elif defined(USE_ESP32)
ModbusTCP modbusTCP;
#endif

// data array for modbus network sharing

// MAX6675 et(CLK, etCS, btMISO);

void setup()
{
// write your initialization code here
#ifdef USE_ESP32
  WiFiManager wm;
  wm.resetSettings();            // todo 可能需要修改
  wm.autoConnect("RoastLogger"); // ssid
                                 // WiFi连接成功后将通过串口监视器输出连接成功信息
  Serial.println("");
  Serial.print("ESP32 Connected to ");
  Serial.println(WiFi.SSID()); // WiFi名称
  // Serial.println(WiFi.psk());  // WiFi密码

  // WiFi.begin(WiFi.SSID(), WiFi.psk();
  //   while (WiFi.status() != WL_CONNECTED) {
  //   delay(1000);
  //   Serial.println("Connecting to WiFi...");
  //   }
  //   Serial.println("Connected to WiFi");
  Serial.println();
  Serial.print("Connected,IP Address:");
  Serial.println(WiFi.localIP());

  // 初始化Modbus TCP
  modbusTCP.server();

  // 配置保持寄存器数量和地址范围
  modbusTCP.addHreg(0, 0); // 地址0，初始值1234
#endif
  Serial.begin(19200);
  // Serial.println("test");
  delay(500);
}

void loop()
{

  // write current thermocouple value
  au16data[2] = ((uint16_t)bt.readCelsius() * 100);
  au16data[3] = ((uint16_t)et.readCelsius() * 100);

#ifdef USE_ESP32
  modbusTCP.task();
  modbusTCP.Hreg(2, au16data[2]);
  modbusTCP.Hreg(3, au16data[3]);
#elif defined(USE_PICO)
  slave.poll(au16data, 16);
#endif
  // write relay value using pwm
  analogWrite(relay, (au16data[4] / 100.0) * 255);
  delay(500);
}
