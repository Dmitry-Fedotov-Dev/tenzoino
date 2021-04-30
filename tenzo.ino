#include "HX711.h"
#include "GyverButton.h"

#define BTN_PIN 3
// Определяем пины DT и SCK:
byte dt[4];
byte sck[4];

dt[0] = A0;
dt[1] = A2;
dt[2] = A4;
dt[3] = A6;

sck[0] = A1;
sck[1] = A3;
sck[2] = A5;
sck[3] = A7;

float konvert = 0.035274 * 0.3404255319;
float calibration_factor = -14.15;
bool flag;
int exp_count = 0;

float units[4];
float delta[4];

// Делаем свою надстройку над классом HX711:
class TENZO : public HX711 {
public:
    void get_ready(byte dt, byte sck, float calibration_factor){
      this->begin(dt, sck);
      this->set_scale();
      this->tare();
      this->set_scale(calibration_factor);
      if (this->is_ready()){
        return "Gage is ready to work";
      } else {
        return "Gage is not ready";
      }
    }
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

TENZO gages[4];
GButton butt1(BTN_PIN);

String print_value;
int array_size = sizeof(gages[]);
void setup() {
  Serial.begin(9600);
  butt1.setDebounce(50);
  butt1.setTimeout(300);
  butt1.setClickTimeout(600);
  butt1.setType(HIGH_PULL);
  butt1.setDirection(NORM_OPEN);

  for (i = 0; i < array_size; i++){
    gages[i].get_ready(dt[i], sck[i], calibration_factor);
  }
  
  Serial.println("Gages are ready to work!");
}

void loop() {
    butt1.tick();
    for (i = 0; i < array_size; i++){
      units[i] = gages[i].get_units(); 
    }
    print_value = "Gage #1:   " + (String)units[0] + "    " + "Gage #2: " + (String)units[1] + "    " + "Gage #3: " + (String)units[2] + "    " + "Gage #4: " + (String)units[3]; 
    Serial.println(print_value);
    if (butt1.isSingle()) {
      flag = true;
      exp_count++;
      Serial.println("Start Experiment # " + exp_count);
      while (flag == true){
        butt1.tick();
        
        for (i = 0; i < array_size; i++){
          delta[i] = gages[i].get_delta(units[i]);
        }
        print_value = "Expiriment #" + (String)exp_count + " | " + "Gage #1:   " + (String)delta[0] + "    " + "Gage #2: " + (String)delta[1] + "    " + "Gage #3: " + (String)delta[2] + "    " + "Gage #4: " + (String)delta[3];
        Serial.println(print_value);
        if (butt1.isSingle()){
        flag = false;
        print_value = "Expiriment # " + (String)exp_count + " has ended";
        Serial.println(print_value);
        }
      }
    }
}
