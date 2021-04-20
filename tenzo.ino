#include "HX711.h"
#include "GyverButton.h"

#define DT1  A0
#define SCK1 A1
#define DT2  A2
#define SCK2 A3
#define DT3  A4
#define SCK3 A5
#define DT4  A6
#define SCK4 A7
#define BTN_PIN 3

float konvert = 0.035274 * 0.3404255319;
float calibration_factor = -14.15;

class TENZO : public HX711 {
public:
    float get_delta(float m_last_count) {
      return m_last_count - this->get_units() * konvert;
   }
    float get_last_count() {
      return m_last_count;
  }
protected:
  HX711 m_hx711;
  float m_last_count;
};

TENZO gage1;
TENZO gage2;
TENZO gage3;
TENZO gage4;
GButton butt1(BTN_PIN);

bool flag;
int exp_count = 0;

float units1;
float units2;
float units3;
float units4;

float delta1;
float delta2;
float delta3;
float delta4;





String print_value;

void setup() {
  Serial.begin(9600);
  butt1.setDebounce(50);
  butt1.setTimeout(300);
  butt1.setClickTimeout(600);
  butt1.setType(HIGH_PULL);
  butt1.setDirection(NORM_OPEN);
  
  gage1.begin(DT1, SCK1);
  gage1.set_scale();
  gage1.tare();
  gage1.set_scale(calibration_factor);

  gage2.begin(DT2, SCK2);
  gage2.set_scale();
  gage2.tare();
  gage2.set_scale(calibration_factor);

  gage3.begin(DT3, SCK3);
  gage3.set_scale();
  gage3.tare();
  gage3.set_scale(calibration_factor);

  gage4.begin(DT4, SCK4);
  gage4.set_scale();
  gage4.tare();
  gage4.set_scale(calibration_factor);

  Serial.println("Gages are ready to work!");
}

void loop() {
    butt1.tick();
    units1 = gage1.get_units() * konvert;
    units2 = gage2.get_units() * konvert;
    units3 = gage3.get_units() * konvert;
    units4 = gage4.get_units() * konvert;
    print_value = "Gage #1:   " + (String)units1 + "    " + "Gage #2: " + (String)units2 + "    " + "Gage #3: " + (String)units3 + "    " + "Gage #4: " + (String)units4; 
    Serial.println(print_value);
    if (butt1.isSingle()) {
      flag = true;
      exp_count++;
      Serial.println("Start Experiment # " + exp_count);
      while (flag == true){
        butt1.tick();
        
        delta1 = gage1.get_delta(units1);
        delta2 = gage2.get_delta(units2);
        delta3 = gage3.get_delta(units3);
        delta4 = gage4.get_delta(units4);
        print_value = "Expiriment #" + (String)exp_count + " | " + "Gage #1:   " + (String)delta1 + "    " + "Gage #2: " + (String)delta2 + "    " + "Gage #3: " + (String)delta3 + "    " + "Gage #4: " + (String)delta4;
        Serial.println(print_value);
        if (butt1.isSingle()){
        flag = false;
        print_value = "Expiriment # " + (String)exp_count + " has ended";
        Serial.println(print_value);
        }
      }
    }
}
