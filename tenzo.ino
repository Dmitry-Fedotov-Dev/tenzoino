#include "HX711.h"
#include "GyverButton.h"

#define BTN_PIN 3

// Определяем пины DT и SCK:
byte dt[] = {A0, A2, A4, 10};
byte sck[] = {A1, A3, A5, 9};

// Определяем переменные:
const uint8_t number_of_gages = sizeof(dt) / sizeof(dt[0]);
const float konvert = 0.035274 * 0.3404255319; // Временная константа для конвертирования необработанных попугаев с HX711
const float calibration_factor = -14.15;
bool flag_exp;
bool flag_debug;
uint16_t exp_count = 0;
uint16_t current_gage = 0;
float units[number_of_gages];
float delta[number_of_gages];
String print_value;

// Делаем свою надстройку над классом HX711:
class TENZO : public HX711
{
public:
  void get_ready(byte dt, byte sck, float calibration_factor)
  {
    this->begin(dt, sck);
    this->set_scale();
    this->tare();
    this->set_scale(calibration_factor);
  }
  float get_delta(float last_count)
  {
    return m_last_count - this->get_units() * konvert;
  }

protected:
  float last_count;
  
};

TENZO gages[number_of_gages];
GButton butt1(BTN_PIN);

void setup()
{
  Serial.begin(9600);
  butt1.setDebounce(50);
  butt1.setTimeout(300);
  butt1.setClickTimeout(600);
  butt1.setType(HIGH_PULL);
  butt1.setDirection(NORM_OPEN);

  for (int i = 0; i < number_of_gages; i++)
  {
    gages[i].get_ready(dt[i], sck[i], calibration_factor);
  }

  Serial.println("Gages are ready to work!");
}

void loop()
{
  butt1.tick();
  for (int i = 0; i < number_of_gages; i++)
  {
    units[i] = gages[i].get_units();
  }
  print_value = "Gage #1:   " + (String)units[0] + "    " + "Gage #2: " + (String)units[1] + "    " + "Gage #3: " + (String)units[2] + "    " + "Gage #4: " + (String)units[3];
  Serial.println(print_value);
  if (butt1.isTriple())
  {
    get_debug();
  }
  if (butt1.isSingle())
  {
    get_expiriment(units);
  }
}

void get_debug()
{
  flag_debug = true;
  Serial.println("Start DEBUG MODE | Gage #: " + (String)current_gage);
  while (flag_debug == true)
  {
    butt1.tick();
    for (int i = 0; i < number_of_gages; i++)
    {
      units[i] = gages[i].get_units();
    }
    if (butt1.isDouble())
    {
      current_gage++;
      if (current_gage >= number_of_gages)
      {
        current_gage = 0;
      }
    }
    print_value = "Gage # " + (String)current_gage + "  |  Value:" + (String)units[current_gage];
    Serial.println(print_value);
    if (butt1.isTriple())
    {
      flag_debug = false;
      print_value = "DEBUG MODE has canceled";
      Serial.println(print_value);
    }
  }
  return;
}

void get_expiriment(float last_units[number_of_gages])
{
  flag_exp = true;
  exp_count++;
  Serial.println("Start Experiment # " + (String)exp_count);
  while (flag_exp == true)
  {
    butt1.tick();

    for (int i = 0; i < number_of_gages; i++)
    {
      delta[i] = gages[i].get_delta(last_units[i]);
    }
    print_value = "Expiriment #" + (String)exp_count + " | " + "Gage #1:   " + (String)delta[0] + "    " + "Gage #2: " + (String)delta[1] + "    " + "Gage #3: " + (String)delta[2] + "    " + "Gage #4: " + (String)delta[3];
    Serial.println(print_value);
    if (butt1.isSingle())
    {
      flag_exp = false;
      print_value = "Expiriment # " + (String)exp_count + " has ended";
      Serial.println(print_value);
    }
  }
  return;
}