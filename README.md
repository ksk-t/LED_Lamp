# LED Lamp

## Software Ideas
- User can set profiles via commands
  - Values between 0-255
  - Save on external FRAM via I2C
    - Create custom library to read/write specific profile from/to I2C
  - Smooth transition between profiles
  - Load default profiles if no profiles in FRAM 
- Sleep timer. Slowly dim lights for set amount of time 
- Morning light. Slowly brighten lights in the morning 
- Auto brightness?

## Hardware Ideas
### Immediate
1. 12V Connector
2. Switching MOSFETS
3. All Pin passthrough

### Later
1. FRAM
2. RTC (real time clock)
3. Light\Light sensor?
4. Rotary Encoder with push + 2 physical buttons

