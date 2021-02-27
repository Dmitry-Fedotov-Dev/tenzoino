#include "HX711.h"



#define DT  A0
#define SCK A1
HX711 scale;

bool flag;
int i;
float unit;
float following_unit;
float last_unit;
float delta;


float konvert = 0.035274 * 0.3404255319;
float calibration_factor = -14.15;

char print_value;

void setup() {
  Serial.begin(9600);
  scale.begin(DT, SCK);
  scale.set_scale();
  scale.tare();
  scale.set_scale(calibration_factor);
  Serial.println("Gage is ready to work.");
}

void loop() {
  unit = scale.get_units() * konvert;
  Serial.print(unit);
  Serial.println(" x 10^(-6)");
  following_unit = scale.get_units() * konvert;
  delta = abs((unit - following_unit) * 100 / unit);
  delay(1000);
  if (delta > 100) {
    last_unit = unit;
    i++;
    get_delta(i, last_unit);
  }
  else {
    return 0;
  }
}

void get_delta(int i, float last_unit) {
  flag = true;
  while (flag == true) {
    delta = abs((scale.get_units() * konvert - last_unit) * 100 / unit);
    if (delta > 100) {
      Serial.print("Expirement № ");
      Serial.print(i);
      Serial.print(" ");
      Serial.print(abs(scale.get_units() * konvert - last_unit));
      Serial.println(" x 10^(-6)");
    }
    else {
      flag = !flag;
    }
  }

  return 0;
}
