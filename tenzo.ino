#include "HX711.h"
#include "GyverButton.h"

#define BTN_PIN 3

// Определяем пины DT и SCK:
byte dt[] = {A0, A2, A4, 9};
byte sck[] = {A1, A3, A5, 10};

// Определяем переменные:
const uint8_t number_of_gages = sizeof(dt)/sizeof(dt[0]);
float konvert = 0.035274 * 0.3404255319;
float calibration_factor = -14.15;
bool flag;
uint16_t exp_count = 0;
float units[number_of_gages];
float delta[number_of_gages];
String print_value;


// Делаем свою надстройку над классом HX711:
class TENZO : public HX711 {
public:
    void get_ready(byte dt, byte sck, float calibration_factor){
      this->begin(dt, sck);
      this->set_scale();
      this->tare();
      this->set_scale(calibration_factor);
    }
    float get_delta(float m_last_count) {
      return m_last_count - this->get_units() * konvert;
   }
    float get_last_count() {
      return m_last_count;
  }
protected:
  float m_last_count;
};

TENZO gages[number_of_gages];
GButton butt1(BTN_PIN);

void setup() {
  Serial.begin(9600);
  butt1.setDebounce(50);
  butt1.setTimeout(300);
  butt1.setClickTimeout(600);
  butt1.setType(HIGH_PULL);
  butt1.setDirection(NORM_OPEN);

  for (int i = 0; i < number_of_gages; i++){
    gages[i].get_ready(dt[i], sck[i], calibration_factor);
  }
  
  Serial.println("Gages are ready to work!");
}

void loop() {
    butt1.tick();
    for (int i = 0; i < number_of_gages; i++){
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
        
        for (int i = 0; i < number_of_gages; i++){
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
