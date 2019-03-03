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
#define ANALOG_WRITE_MAX 255
#define ANALOG_WRITE_MIN 0
#define PWM_FREQUENCY    25000

// Status returns
#define FAILURE          -1
#define SUCCESS          1

void setup()
{
  Serial.begin(SERIAL_BAUD);

  // Configure pins
  pinMode(PIN_LED_WARM, OUTPUT);
  pinMode(PIN_LED_COOL, OUTPUT);
  analogWriteFrequency(PWM_FREQUENCY);
}

void loop() 
{
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
}

// Execute function assocated with cmd with pass given value
// 
// @param input_cmd - command to interprete. Should contain a single command
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
        return set_led_warm(val_int);
       }else if(strcmp(cmd, "cool") == 0)
       {
        
        // Set cool led brightness
        int val_int = atoi(val);
        return set_led_cool(val_int);
       }else
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
int set_led_warm(int val)
{
  if (val < ANALOG_WRITE_MIN || val > ANALOG_WRITE_MAX)
  {
    return FAILURE;
  }
  else
  {
    analogWrite(PIN_LED_WARM, val);
    return SUCCESS;
  }
}

// Set the brightness of the cool LED
//
// @param val - brightness (0 - 255) to set the LED to
//
// @return SUCCESS if value is in range, FAILURE otherwise
int set_led_cool(int val)
{
  if (val < ANALOG_WRITE_MIN || val > ANALOG_WRITE_MAX)
  {
    return FAILURE;
  }
  else
  {
    analogWrite(PIN_LED_COOL, val);
    return SUCCESS;
  }
}
