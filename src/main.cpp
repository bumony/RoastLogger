#include <Arduino.h>
#include "max6675.h"
#include "ModbusRtu.h"

int btCS = 1;
int etCS = 5;
int CLK = 2;
int SO = 3;
uint16_t au16data[16] = { 
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

/**
 *  Modbus object declaration
 *  u8id : node id = 0 for master, = 1..247 for slave
 *  u8serno : serial port (use 0 for Serial)
 *  u8txenpin : 0 for RS-232 and USB-FTDI 
 *               or any pin number > 1 for RS-485
 */
Modbus slave(1,0,0); // this is slave @1 and RS-232 or USB-FTDI

MAX6675 bt(CLK, btCS, SO);
MAX6675 et(CLK, etCS, SO);
int relay = 9;  
// data array for modbus network sharing


// MAX6675 et(CLK, etCS, btMISO);

void setup() {
// write your initialization code here

Serial.begin(19200);
// Serial.println("test");
delay(500);

}

void loop() {

//write current thermocouple value
  au16data[2] = ((uint16_t) bt.readCelsius()*100);
  au16data[3] = ((uint16_t) et.readCelsius()*100);

  //poll modbus registers
  slave.poll( au16data, 16 );

   //write relay value using pwm
  analogWrite(relay, (au16data[4]/100.0)*255);
delay(500);

}


