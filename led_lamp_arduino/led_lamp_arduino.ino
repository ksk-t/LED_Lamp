/*
* Project: LED Lamp
* Author: Keisuke Tomizawa
*/

// Serial input
#define MAX_CMD_SIZE    16
#define SERIAL_BAUD   9600
#define CMD_TERM_CHAR '\n'

// Pin configuration
#define PIN_LED_WARM     5
#define PIN_LED_COOL     6
#define PIN_PROFILE      7
#define ANALOG_WRITE_MAX 255
#define ANALOG_WRITE_MIN 0
#define PWM_FREQUENCY    25000

// Status returns
#define FAILURE          -1
#define SUCCESS          0

// Transition Effect
#define STEP_TIME 3  // Time (in ms) between a step of brightness during transitions                                                               

// Structures
typedef struct 
{
  int led_warm_val;
  int led_cool_val;
}led_profile;

// Global Varialbes
int curr_led_warm_val  = 0;
int curr_led_cool_val  = 0;
int curr_profile_index = 0;
int last_button_status = 0;

// Default profiles
#define NUM_PROFILES 5
led_profile default_profiles[] = 
{
  {255, 0}, // { warm_led_val, cool_led_val }
  {255, 50},
  {100, 100},
  {50 , 255},
  {0  , 0}
};

void setup()
{
  // Setup Serial
  Serial.begin(SERIAL_BAUD);

  // Configure pins
  analogWriteFrequency(PWM_FREQUENCY);
  pinMode(PIN_LED_WARM, OUTPUT);
  pinMode(PIN_LED_COOL, OUTPUT);
  pinMode(PIN_PROFILE, INPUT);
  // Synchronize LED state with global variable
  analogWrite(PIN_LED_WARM, curr_led_warm_val);
  analogWrite(PIN_LED_COOL, curr_led_cool_val);
  // Load profile
  set_profile(0);
}

void loop() 
{
  // Check serial buffer
  if (Serial.available() > 0)
  {
    // Get cmd from serial buffer
    // One extra byte to hold string termination char
    char input_array[MAX_CMD_SIZE + 1];
    size_t arr_size = Serial.readBytesUntil(CMD_TERM_CHAR, input_array, MAX_CMD_SIZE);
    input_array[arr_size] = 0; // Add terminal char

    if (interprete_cmd(input_array) == FAILURE)
      Serial.println("Invalid cmd");
  }
  
  // Rotate profile if button is pressed
  int curr_button_status = digitalRead(PIN_PROFILE);
  // Execute if last button was low, and is now high (rising-edge detection)
  if (curr_button_status && !last_button_status)
  {
    rotate_profile();
  }
  last_button_status = curr_button_status;
}
// Execute function associated with cmd with given value
// 
// @param input_cmd - command to interprete. Should contain only a single command
//
// @return true if cmd successfully executed, false otherwise
int interprete_cmd(char *input_array)
{   
    // Split input into cmd and value
    char* seperator = strchr(input_array, ' ');
    if (seperator == 0)
    {
      return FAILURE;
    }else
    {
       *seperator = 0; // Replace seperator with terminal character. input_array now split into cmd and value
       char cmd[MAX_CMD_SIZE];
       size_t len = strlen(input_array);
       strncpy(cmd, input_array, len + 1);
       ++seperator; // Point to first character of value
       len = strlen(seperator);
       char val[MAX_CMD_SIZE];
       strncpy(val, seperator, len);

       // Execute command
       if (strcmp(cmd, "warm") == 0)
       {
        // Set warm led brightness
        int val_int = atoi(val);
        return set_led(PIN_LED_WARM, &curr_led_warm_val, val_int);
       }else if(strcmp(cmd, "cool") == 0)
       {
        // Set cool led brightness
        int val_int = atoi(val);
        return set_led(PIN_LED_COOL ,&curr_led_cool_val, val_int);
       }else if (strcmp(cmd, "all") == 0)
       {
        int val_int = atoi(val);
        int ret_status;
        ret_status  = set_led(PIN_LED_COOL ,&curr_led_cool_val, val_int);
        ret_status |= set_led(PIN_LED_WARM, &curr_led_warm_val, val_int);
        return ret_status;
       }
       {
        return FAILURE;
       }
    }
}

// Set the brightness of the warm LED
//
// @param val - brightness (0 - 255) to set the LED to
//
// @return SUCCESS if value is in range, FAILURE otherwise
int set_led(int pin,int *current_val ,int new_val)
{
  if (new_val < ANALOG_WRITE_MIN || new_val > ANALOG_WRITE_MAX)
  {
    return FAILURE;
  }
  else
  {
    int modifier;
    if (*current_val < new_val)
      modifier = 1;
    else if (*current_val > new_val)
      modifier = -1;
    else
      return SUCCESS; // new val is the same as the currently set value
      
    do 
    {
      *current_val += modifier;
      analogWrite(pin, *current_val);
      delay(STEP_TIME);
    }while (*current_val != new_val);
    return SUCCESS;
  }
}

int set_profile(int profile_index)
{
  int ret_status;
  ret_status  = set_led(PIN_LED_WARM, &curr_led_warm_val, default_profiles[profile_index].led_warm_val);
  ret_status |= set_led(PIN_LED_COOL, &curr_led_cool_val, default_profiles[profile_index].led_cool_val);
  curr_profile_index = profile_index;
  return ret_status;
}

void rotate_profile()
{
  set_profile((curr_profile_index + 1) % NUM_PROFILES);
}
