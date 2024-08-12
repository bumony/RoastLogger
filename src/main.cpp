#ifdef ESP32
#include <WiFi.h>
#include <ModbusTCP.h>
#include <WiFiManager.h>
#elif defined(USE_PICO)
#include <ModbusRtu.h>
#endif
#include <Arduino.h>
#include "max6675.h"
#include "platform_config.h"

// Modbus 数据数组，存储要共享的温度数据
uint16_t au16data[16] = {0};

// 初始化两个 MAX6675 温度传感器
MAX6675 bt(CLK, btCS, SO);
MAX6675 et(CLK, etCS, SO);

#ifdef ESP32
// 创建 ModbusTCP 对象
ModbusTCP modbusTCP;
#elif defined(USE_PICO)
// 创建 Modbus 从站对象
Modbus slave(1, 0, 0);
#endif

void setup() {
  Serial.begin(115200);

#ifdef ESP32
  // 使用 WiFiManager 管理 Wi-Fi 连接
  WiFiManager wm;
  wm.autoConnect("RoastLogger");

  Serial.println("");
  Serial.print("ESP32 Connected to ");
  Serial.println(WiFi.SSID());
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // 初始化 Modbus TCP 服务器
  modbusTCP.server();
  
  // 配置 Modbus 保持寄存器
  modbusTCP.addHreg(2, 0);  // 地址 2
  modbusTCP.addHreg(3, 0);  // 地址 3

#elif defined(USE_PICO)
  // 初始化 Modbus RTU 从站
  slave.begin(19200);
#endif
}

void loop() {
  // 读取温度传感器数据
  au16data[2] = static_cast<uint16_t>(bt.readCelsius() * 100);
  au16data[3] = static_cast<uint16_t>(et.readCelsius() * 100);

#ifdef ESP32
  // 更新 Modbus 保持寄存器的值
  modbusTCP.task();
  modbusTCP.Hreg(2, au16data[2]);
  modbusTCP.Hreg(3, au16data[3]);

#elif defined(USE_PICO)
  // 处理 Modbus RTU 请求
  slave.poll(au16data, 16);
#endif

  delay(500);
}