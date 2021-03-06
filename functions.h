#include "NXCDefs.h"
#include "HTSMUX-driver.h"

/*---Konstanten---*/
#define SpeedNorm 85
#define Schwarzwert 50
#define greenTime 2000

/*---Variablen---*/
long intzeroval;
byte gemachteSchleifenaufgaben;
int GreenAttempts;
int time;
bool DownFloor;
byte count;
bool DirectionRight;
byte SpeedLeft;
byte SpeedRight;
long sensorTime;
bool InTurn;
//Ultraschall-Test
byte distance;

/*---Funktionen---*/
void InitSensors()
{
     SetSensor(S1, SENSOR_LOWSPEED);
     SetSensorLight(IN_2);
     SetSensorLight(IN_3);
	 SetSensorColorFull(IN_4);
	 if(!HTSMUXscanPorts(S1))
	 {
		 TextOut(0, LCD_LINE1, "Scan failed!");
		 Wait(1000);
	 }
	 smuxSetSensorLegoTouch(msensor_S1_4);
	 smuxSensorLegoUS(msensor_S1_1);
}
 
void StartTiming()
{
     intzeroval = CurrentTick();
}

void HandleCounting()
{
     ++count;
     if(count >= 50)
     {
         count = 0;
     }
}

long ReturnTime()
{
     return (CurrentTick() - intzeroval);
}

void TurnRight()
{
     SpeedLeft = -100;
     SpeedRight = 100;
     DirectionRight = true;
	 InTurn = true;
	 StartTiming();
	 gemachteSchleifenaufgaben +=1;
}

void TurnLeft()
{
     SpeedLeft = 100;
     SpeedRight = -100;
     DirectionRight = false;
	 InTurn = true;
	 StartTiming();
	 gemachteSchleifenaufgaben +=1;
}

void StartGreenLine()
{
     sensorTime = CurrentTick();
}

void CheckGreen()
{
     int color = SENSOR_4;
     repeat(3)
	 {
		switch(color)
		{
			case 3:
				PlayTone(3000, 30);

				GreenAttempts++;
				if(GreenAttempts > 8)
				{
					StartGreenLine();
				}

			break;
			default:
				GreenAttempts = 0;
			break;
		}
	 }
}
void GreenAndWinkel()
{
	PlayTone(1000,20);
    GreenAttempts=0;
    repeat(30)
    {
		OnFwd(OUT_B,70);
		OnRev(OUT_C,70);
        Wait(10);
        if(SENSOR_4 == 3)
        {
			GreenAttempts++;
        }
    }
    repeat(60)
    {
		OnFwd(OUT_C,70);
		OnRev(OUT_B,70);
        Wait(10);
        if(SENSOR_4 == 3)
        {
			GreenAttempts++;
        }
    }
    repeat(30)
    {
		OnFwd(OUT_B,70);
		OnRev(OUT_C,70);
        Wait(10);
        if(SENSOR_4 == 3)
        {
			GreenAttempts++;
        }
    }
    repeat(40)
	{
		OnRev(OUT_BC,70);
		Wait(10);
		if(SENSOR_4 == 3)
		{
			GreenAttempts++;
		}
	}
	if(GreenAttempts > 10)
    {
		PlayTone(3000, 20);
        StartGreenLine();
    }
}

long ReturnSensorTime()
{
     return (CurrentTick() - sensorTime);
}
