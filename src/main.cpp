//Reads all traffic on CAN0 and forwards it to CAN1 (and in the reverse direction) but modifies some frames first.
// Required libraries
#include "variant.h"
#include <due_can.h>

//REV Max Motor ID's
enum control_mode {
  Duty_Cycle_Set = 0x2050080,
  Speed_Set = 0x2050480,
  Smart_Velocity_Set = 0x20504C0,
  Position_Set = 0x2050C80,
  Voltage_Set = 0x2051080,
  Current_Set = 0x20510C0,
  Smart_Motion_Set = 0x2051480
};

void setup()
{
  Serial.begin(115200);
  
  // Initialize CAN0, Set the proper baud rates here
  Can0.begin(1000000);
  Can1.begin(1000000);
  
  Can0.watchFor();  
}

void sendData()
{
	CAN_FRAME outgoing;
	outgoing.id = Speed_Set;
	outgoing.extended = true;
	//outgoing.priority = 4; //0-15 lower is higher priority
  //create data
	outgoing.data.byte[0] = 0xFF;
	outgoing.data.byte[1] = 0xFF;
  outgoing.data.byte[2] = 0xFF;
	outgoing.data.byte[3] = 0xFF;
  outgoing.data.byte[4] = 0xFF;
	outgoing.data.byte[5] = 0xFF;
  outgoing.data.byte[6] = 0xFF;
	outgoing.data.byte[7] = 0xFF;
	outgoing.data.high = Speed_Set;
	Can0.sendFrame(outgoing);
}

void loop(){
  CAN_FRAME incoming;
  static unsigned long lastTime = 0;

  if (Can0.available() > 0) {
	  Can0.read(incoming);
	  Can1.sendFrame(incoming);
  }

  if (Can1.available() > 0) {
	  Can1.read(incoming);
	  Can0.sendFrame(incoming);
  }

  if ((millis() - lastTime) > 1000) 
  {
     lastTime = millis();
     sendData();    
  }
}
