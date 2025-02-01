# ROBOT_GLADIATOR_LEAGUE
Location for code relating to the operation of Newtons attic's Robot Gladiator League pin resets

This repository includes 4 C files. MEGA_W_COMMS_FINAL, NANO_W_COMMS_FINAL, INTERMEDIATE_MEGA, and INTERMEDIATE_MEGA_TWO. 

MEGA_W_COMMS_FINAL will be uploaded to the main controller which connects to two other Arduino Megas.

NANO_W_COMMS_FINAL will be uploaded to the Arduino nano located on each target module, for a total of 7. 6 in play and 1 spare.

INTERMEDIATE_MEGA will be uploaded to the Arduino mega that communicates with target modules 1-3

INTERMEDIATE_MEGA_TWO will be uploaded to the Arduino mega that communicates with target modules 4-6

The system uses a finite state machine design driven by the main controller. It moves the nanos into the appropriate state where an interrupt is relayed back to the main controller to alert it of a pin being knocked down. The intermediate Arduino Megas are required because the Arduino Mega is limited by 4 serial communication lines, and we have a need of 6. There code is simple and they serve only to bridge communications between the main controller and the Arduino Nanos located on the target modules.
