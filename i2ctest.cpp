#include <iostream>
#include <errno.h>
#include <stdio.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>

#define PID_ENGINE_LOAD 0x04
#define PID_COOLANT_TEMP 0x05
#define PID_SHORT_TERM_FUEL_TRIM_1 0x06
#define PID_LONG_TERM_FUEL_TRIM_1 0x07
#define PID_SHORT_TERM_FUEL_TRIM_2 0x08
#define PID_LONG_TERM_FUEL_TRIM_2 0x09
#define PID_FUEL_PRESSURE 0x0A
#define PID_INTAKE_MAP 0x0B
#define PID_RPM 0x0C
#define PID_SPEED 0x0D
#define PID_TIMING_ADVANCE 0x0E
#define PID_INTAKE_TEMP 0x0F
#define PID_MAF_FLOW 0x10
#define PID_THROTTLE 0x11
#define PID_AUX_INPUT 0x1E
#define PID_RUNTIME 0x1F
#define PID_DISTANCE_WITH_MIL 0x21
#define PID_COMMANDED_EGR 0x2C
#define PID_EGR_ERROR 0x2D
#define PID_COMMANDED_EVAPORATIVE_PURGE 0x2E
#define PID_FUEL_LEVEL 0x2F
#define PID_WARMS_UPS 0x30
#define PID_DISTANCE 0x31
#define PID_EVAP_SYS_VAPOR_PRESSURE 0x32
#define PID_BAROMETRIC 0x33
#define PID_CATALYST_TEMP_B1S1 0x3C
#define PID_CATALYST_TEMP_B2S1 0x3D
#define PID_CATALYST_TEMP_B1S2 0x3E
#define PID_CATALYST_TEMP_B2S2 0x3F
#define PID_CONTROL_MODULE_VOLTAGE 0x42
#define PID_ABSOLUTE_ENGINE_LOAD 0x43
#define PID_AIR_FUEL_EQUIV_RATIO 0x44
#define PID_RELATIVE_THROTTLE_POS 0x45
#define PID_AMBIENT_TEMP 0x46
#define PID_ABSOLUTE_THROTTLE_POS_B 0x47
#define PID_ABSOLUTE_THROTTLE_POS_C 0x48
#define PID_ACC_PEDAL_POS_D 0x49
#define PID_ACC_PEDAL_POS_E 0x4A
#define PID_ACC_PEDAL_POS_F 0x4B
#define PID_COMMANDED_THROTTLE_ACTUATOR 0x4C
#define PID_TIME_WITH_MIL 0x4D
#define PID_TIME_SINCE_CODES_CLEARED 0x4E
#define PID_ETHANOL_FUEL 0x52
#define PID_FUEL_RAIL_PRESSURE 0x59
#define PID_HYBRID_BATTERY_PERCENTAGE 0x5B
#define PID_ENGINE_OIL_TEMP 0x5C
#define PID_FUEL_INJECTION_TIMING 0x5D
#define PID_ENGINE_FUEL_RATE 0x5E
#define PID_ENGINE_TORQUE_DEMANDED 0x61
#define PID_ENGINE_TORQUE_PERCENTAGE 0x62
#define PID_ENGINE_REF_TORQUE 0x63

#define seatbelt 0
#define seatSensor 3
#define Buzzer 2

using namespace std;
int ReadPID(int PID);
bool checkConnection();
bool checkDriverStatus();

int fd;
string initial_read = "";

int main()
{
   if (wiringPiSetup () == -1)
      return 1 ;
   pinMode(seatbelt, INPUT);
   pinMode(seatSensor, INPUT);
   pinMode(Buzzer, OUTPUT);
   pullUpDnControl(seatSensor, PUD_UP);
   pullUpDnControl(seatbelt, PUD_UP);
   digitalWrite(Buzzer, LOW);
   fd = wiringPiI2CSetup(0x68);
   while(true)
   {
      bool isConnected = false;
      bool isDriverPresent = false;
      isDriverPresent = checkDriverStatus();
      isConnected = checkConnection();
      if((digitalRead(seatbelt) != 0) && digitalRead(seatSensor) == 1)
         initial_read = "";
      if((digitalRead(seatbelt) != 1) && (digitalRead(seatSensor) == 1))
      {
         if(isDriverPresent)
         {
            digitalWrite(Buzzer, LOW);
            printf("%s\n","Driver is Present");
         }
         else
         {
	    printf("%s\n","Driver is not present");
            if(isConnected)
            {
               digitalWrite(Buzzer, LOW);
               printf("%s\n","Bluetooth is Connected");
            }
            else
	    {
               digitalWrite(Buzzer, HIGH);
	       printf("%s\n","Bluetooth is Not Connected");
            }
         }
      }
   }
}

bool checkConnection()
{
	FILE* pipe = popen("sudo hcitool info 40:4E:36:04:F5:E2", "r");
	if (!pipe)
	   return "ERROR";
	
	char buffer[128];
	string result = "";
	while(!feof(pipe))
	{
	   if(fgets(buffer, 128, pipe) != NULL)
			result += buffer;
	}
	pclose(pipe);
        if(initial_read == "")
	{
	   initial_read = result;
	}
	if(initial_read == result)
	   return true;
	else
	   return false;
}

bool checkDriverStatus()
{
//This is where we pull data from the OBD II
//to check if the driver has the key in
   //if(false)
   //{
       return false;
   //}
   //else
   //{
       return true;
   //}
}

int ReadPID(int PID)
{
   int result = 0;   

   result = wiringPiI2CReadReg16(fd, PID);

   return result;
}

