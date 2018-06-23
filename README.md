# EdselTransmissionCytronMotorController

The included Arduino code allows an Arduino Uno to interact with a [Cytron Shield MD-10](https://www.cytron.io/p-shield-md10) R2 motor controller. The motor controller is then connected to an [HDA4-50](https://www.servocity.com/hda4-50) linear actuator from ServoCity. This produces 115lbs of thrust for a 4" stroke. 

The 115lbs of thrust is likely not necessary to shift the Edsel transmission, however it was chosen as a good starting point. It's unclear whether 25lbs of thrust would have been enough while parked on a hill.

The motor is connected to an auxliary fuse panel with a 10A fuse.

The Arduino is connected to USB car chargers which accept 12V input and produce 5V output for the Arduino.

The USB chargers are each connected to one terminal of the Edsel push buttons with [Tap Splice connectors](https://www.homedepot.com/p/Gardner-Bender-22-10-AWG-Tap-Splice-Assortment-10-Pack-15-2210/202522123) to prevent the original wires from being cut.

![Everything connected on test bench](https://raw.githubusercontent.com/ajbogh/EdselTransmissionCytronMotorController/master/Screenshot%20from%202018-06-22 22-26-47.jpg)

![PWM Example](https://raw.githubusercontent.com/ajbogh/EdselTransmissionCytronMotorController/master/Selection_010.png)