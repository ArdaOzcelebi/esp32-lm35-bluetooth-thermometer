#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

#define LM35_PIN 34

const float ADC_OFFSET = -20.0;

float filtered_temp = 0.0;
const float alpha = 0.05;
bool first_read = true;

void setup() {
  Serial.begin(115200);

  analogReadResolution(12);
  analogSetPinAttenuation(LM35_PIN, ADC_11db);

  SerialBT.begin("ESP32_Termometre");
}

void loop() {

  long total_mv = 0;

  for (int i = 0; i < 50; i++) {
    total_mv += analogReadMilliVolts(LM35_PIN);
    delay(2);
  }

  float voltage = total_mv / 50.0;
  float avg_mv = voltage + ADC_OFFSET;

  float raw_temp = avg_mv / 10.0;

  if (first_read) {
    filtered_temp = raw_temp;
    first_read = false;
  } else {
    filtered_temp = alpha * raw_temp + (1 - alpha) * filtered_temp;
  }

  SerialBT.print("Raw: ");
  SerialBT.print(raw_temp, 1);
  SerialBT.print(" C  --->  Filtered: ");
  SerialBT.print(filtered_temp, 2);
  SerialBT.println(" C");

  Serial.print("Voltage = ");
  Serial.print(avg_mv);
  Serial.println(" mV");

  delay(1000);
}
