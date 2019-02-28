# LED Lamp

## Software Ideas
1. User can set profiles via commands
  a. values between 0-255
  b. save on external FRAM via I2C
    i. Create custom library to read/write specific profile from/to I2C
  c. smooth transition between profiles
  d. Load default profiles if no profiles in FRAM 
  e. Sleep timer. Slowly dim lights for set amount of time. 
  f. Morning light. Slowly brighten lights in the morning. 
  g. Auto brightness?

## Hardware Ideas
1. 12V Connector
2. Switching MOSFETS
3. All Pin passthrough

### Later
1. FRAM
2. RTC (real time clock)
3. Light\Light sensor?
4. Rotary Encoder with push + 2 physical buttons

