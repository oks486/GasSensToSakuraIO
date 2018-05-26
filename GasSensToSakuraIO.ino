// GasSensToSakuraIO

#include "MutichannelGasSensor.h"
#include "Wire.h"
#include <SakuraIO.h>

SakuraIO_I2C sakuraio;
uint32_t cnt = 0;

void setup() {
  Serial.begin(9600);

  // Sensor
  gas.begin(0x04);//the default I2C address of the slave is 0x04
  gas.powerOn();

  // SakuraIO
  Serial.print("Waiting to come online");
  for (;;) {
    if ( (sakuraio.getConnectionStatus() & 0x80) == 0x80 ) break;
    Serial.print(".");
    delay(1000);
  }
  Serial.println("");
}

void loop() {
  cnt++;

  // Get sensor value
  float val = gas.measure_CH4();

  // Sensor value outputs to serial
  Serial.print(cnt);
  Serial.print("  CH4 : ");
  if (val >= 0.0) {
    Serial.print(val);
  } else {
    Serial.print("invalid");
  }
  Serial.println(" ppm");

  // Sensor value transmits to SakuraIO
  if (sakuraio.enqueueTx(0, cnt) != CMD_ERROR_NONE) {
    Serial.println("[ERR] enqueue error");
  }
  if (sakuraio.enqueueTx(1, val) != CMD_ERROR_NONE) {
    Serial.println("[ERR] enqueue error");
  }

  sakuraio.send();
  delay(5000);

  uint8_t available;
  uint8_t queued;
  if (sakuraio.getTxQueueLength(&available, &queued) != CMD_ERROR_NONE) {
    Serial.println("[ERR] get tx queue length error");
  }

  Serial.print("Available :");
  Serial.print(available);
  Serial.print(" Queued :");
  Serial.println(queued);

}

