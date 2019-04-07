# arcade_cabinet_control
Arduino control for arcade cabinet.  Inspired by iltms_arcade_lights

A number of users were having trouble with the Arduino control for the arcade cabinet for Bob Clagett's project: https://iliketomakestuff.com/how-to-make-an-arcade-cabinet-part-3/.  First and foremost, hats off to Bob as this is a totally amazing project.  However, I had some issues with the way that he implemented the Arduino control for turning on the RPi and the control of the LED lighting with motion control.  This is my spin on it.  

Differences are:
1. Schematic is slightly different.  I use a SPST switch and arduino internal pullup.  I'm also using a beafy toggle switch as it matches my pinball machine vs. the tiny thing that ILTMS used.  I believe original schematic to have a mistake in the switch portion as pin 8 is shorted to ground regardless of the switch condition.  I also do not power LEDs off of Arduino supply.  Use an external supply.  
2. Overall simplification.  
3. Does not automatically turn RPi on or off based on motion.  

There are essentially 4 modes:
1. When switch is on and game is off, the relay turns on, backlighting turns off, marquee turns on white.
2. When switch is off and game is on, the relay turns off, backlighting turns off, marquee turns off.
3. When switch is off and motion is detected, backlight and marque fade on and off for set number of cycles.  This mode is interrupted if the switch is turned on and the game goes to mode 1.  

This relies on the user to shut down the rPi through the console prior to turning the switch off if one is particular about shutting things down properly.  In the future, I may try to figure out how to have the arduino send a shutdown command to the pi, then delay the relay so that the RPi properly shuts down when the switch is thrown.  

A note on LEDs... These LEDs consume a lot of current.  If you have any appreciable number of pixels, which an arcade cabinet will, make sure to use an external supply.  I'm using a 10A, 5V supply.  

Also included is an STL file for a mount for the Arduino, breadboard, and relay board.  This keeps all the boards in one place.  Mounting hardware are 4-40 screws to attach circuit boards.  
