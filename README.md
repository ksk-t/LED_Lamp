# LED Lamp

## Software
### Todo
- User can set profiles via commands
  - Values between 0-255
  - Save on external FRAM via I2C
    - Create custom library to read/write specific profile from/to I2C
  - Smooth transition between profiles
  - Load default profiles if no profiles in FRAM 
- Sleep timer. Slowly dim lights for set amount of time 
- Morning light. Slowly brighten lights in the morning 
- Auto brightness

## Hardware
### Immediate
1. Switching MOSFETS
2. All pin passthrough
3. 3 physical buttons  
    - Power
    - Cycle through profiles (left)
    - Cycle through profiles (right)
### Later
1. FRAM
2. Light\Lux sensor?
3. Rotary Encoder with push + 2 physical buttons
4. Real Time Clock and Calender (RTCC)  
    - Requires 32KHz crystal
### Eagle Guides
- [Schematic](https://learn.sparkfun.com/tutorials/using-eagle-schematic/all)
- [Board Layout](https://learn.sparkfun.com/tutorials/using-eagle-board-layout/all)
### uC32 Reference Eagle Project
[Download Link](https://reference.digilentinc.com/_media/reference/microprocessor/uc32/chipkit_uc32_bysa_a.zip)
