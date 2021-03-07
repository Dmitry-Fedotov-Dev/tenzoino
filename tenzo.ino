#include "HX711.h"

#define DT1  A0
#define SCK1 A1
#define DT2  A2
#define SCK2 A2

HX711 scale1;
HX711 scale2;

bool flag;
float unit1;
float unit2;
float following_unit;
float konvert = 0.035274 * 0.3404255319;
float calibration_factor = -14.15;

void setup() {
  Serial.begin(9600);
  scale1.begin(DT1, SCK1);
  scale1.set_scale();
  scale1.tare();
  scale1.set_scale(calibration_factor);
  scale2.begin(DT2, SCK2);
  scale2.set_scale();
  scale2.tare();
  scale2.set_scale(calibration_factor);
  Serial.println("Gages is ready to work.");
}

void loop() {
  unit1 = scale1.get_units() * konvert;
  unit2 = scale2.get_units() * konvert;
  Serial.print("Gage #1: ")
  Serial.print(unit1);
  Serial.print("        Gage #2:");
  Serial.println(unit2);
}
