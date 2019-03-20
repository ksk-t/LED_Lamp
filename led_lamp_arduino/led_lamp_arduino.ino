/*
* Project: LED Lamp
* Author: Keisuke Tomizawa
* Description: This Arduino project is used to control two PWM controlled LEDs.
*              The main purpose of this project was to practice developing embedded 
*              software in C. Thus, where possible, C was used in favor of C++. Exceptions 
*              include functions specific to interactions with Arduino hardware such as
*              the Serial class and analog read/write functions. 
*              Additionally, the code is written to reflect the Google C\C++ style 
*              guide as much as possible.
* Revisions:
*   March 6, 2019:
*     -Initial Build
*/
typedef struct
{
  uint8_t pin;
  uint8_t last_button_status;
}Button;

typedef struct
{
  uint8_t pin;
  uint8_t brightness;
}Led;

typedef struct 
{
  uint8_t active_profile_index;
  Led led_warm;
  Led led_cool;
}LedController;

Button btn_rotate_prof;
Button btn_power;
LedController controller;

void setup()
{ 
  const int kPwmFrequency = 25000;
  analogWriteFrequency(kPwmFrequency);
  
  const uint8_t kPinLedWarm       = 5;
  const uint8_t kPinLedCool       = 6;
  const uint8_t kPinBtnRotateProf = 7;
  const uint8_t kPinBtnPower      = 4;
  button_init(&btn_rotate_prof, kPinBtnRotateProf);
  button_init(&btn_power, kPinBtnPower);
  led_controller_init(&controller, kPinLedWarm, kPinLedCool);
}

void loop() 
{
  if (button_pressed(&btn_power))
    led_set_off(&controller);
    
  if (button_pressed(&btn_rotate_prof))
    rotate_profile(&controller);
}

void button_init(Button *new_btn, uint8_t pin)
{
  const uint8_t start_status = 0;
  new_btn->pin = pin;
  new_btn->last_button_status = start_status;
  pinMode(pin, INPUT);
}

bool button_pressed(Button *btn)
{
  uint8_t curr_button_status = digitalRead(btn->pin);
  bool ret;

  // Return true on a RISING edge
  if (curr_button_status && !btn->last_button_status)
    ret = true;
  else
    ret = false;
  btn->last_button_status = curr_button_status;
  return ret;
}

void led_init(Led*new_led, uint8_t led_pin)
{
  const uint8_t start_brightness = 0;
  new_led->pin = led_pin;
  new_led->brightness = start_brightness;
  pinMode(led_pin, OUTPUT);
  analogWrite(led_pin, start_brightness);
}

void led_set(Led *led, uint8_t set_brightness)
{
  led->brightness = set_brightness;
  analogWrite(led->pin, set_brightness);
}

void led_set(Led *led1, Led *led2, uint8_t led1_set_brightness, uint8_t led2_set_brightness)
{
  static const int kStepTime = 3; // Time between each change in brightness in miliseconds

  int led1_modifier;
  int led2_modifier;

  if (led1->brightness > led1_set_brightness)
    led1_modifier = -1;
  else
    led1_modifier = +1;
  
  if (led2->brightness > led2_set_brightness)
    led2_modifier = -1;
  else
    led2_modifier = +1;
  
  while (led1->brightness != led1_set_brightness || led2->brightness != led2_set_brightness)
  {
    if (led1->brightness != led1_set_brightness)
      led_set(led1, led1->brightness + led1_modifier);
   
    if (led2->brightness != led2_set_brightness)
      led_set(led2, led2->brightness + led2_modifier);

    delay(kStepTime);
  }
}

void led_set_off(LedController *controller)
{
  uint8_t kOffBrightness = 0;
  led_set(&(controller->led_warm), &(controller->led_cool), kOffBrightness, kOffBrightness);
  controller->active_profile_index = 0;
}

void rotate_profile(LedController *controller)
{
  // Default profiles
  const static uint8_t default_profiles[][2] = 
  {
    {50, 10},  // { warm_led_val, cool_led_val }
    {255, 75}  // Second profile is set first on startup
  };
  const static int num_profiles = sizeof (default_profiles) / sizeof (default_profiles[0]);
  
  int prof_index = (controller->active_profile_index + 1) % num_profiles;

  led_set(&(controller->led_warm), &(controller->led_cool), 
    default_profiles[prof_index][0], default_profiles[prof_index][1]);
  
  controller->active_profile_index = prof_index;
}

void led_controller_init(LedController *controller, uint8_t pin_led_warm, uint8_t pin_led_cool)
{
  controller->active_profile_index = 0;
  led_init(&(controller->led_warm), pin_led_warm);
  led_init(&(controller->led_cool), pin_led_cool);
}
