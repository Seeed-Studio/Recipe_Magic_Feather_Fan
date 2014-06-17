/*****************************************************************************/
//Introduction:this sketch can be used to test gesture recognition.3-Axis Digital 
//   Accelerometer(16g) is in need. Start to test, you should press the button and
//   draw your graphics in the air.Matching results will be seen in serial output.
//   Any problems, please feel free to contact me !
//   now we only support 15 kinds of gestures. It is as follow
//
/*********************************************************************************************************************************************
 *|   0   |   1   |   2	 |   3   |    4   |	5   |	6    |    7  |    8  |    9    |    10   |   11  |   12  |   13    |    14   |   15  |*
 *|   *   |   *   |  *   |   *   |  * * * |	  * |* * *   |*      | *     |* * * *  |  * * * *|    *  |*      |  *      |      *  |      *|*
 *|  ***  |   *   | *    |    *  | * *	  |	 *  |   * *  | *     |* * * *|      *  |  *      |* * * *|*      |* * *    |    * * *|      *|*
 *| * * * | * * * |******|****** |*   *	  | *   *   |  *   * |  *   *| *    *|      *  |  *      |*   *  |*   *  |  *      |      *  |  *   *|*
 *|   *   |  ***  | *    |    *  |     *  |  * *    | *      |   * * |      *|    * * *|* * *    |*      |* * * *|  *      |      *  |* * * *|*
 *|   *   |   *   |  *   |   *   |      * |   * * * |*       |* * *  |      *|      *  |  *      |*      |    *  |  * * * *|* * * *  |  *    |*
 *********************************************************************************************************************************************
//
//  Hardware:3-Axis Digital Accelerometer(16g)
//  Arduino IDE: Arduino-1.0
//  Author:lawliet.zou(lawliet.zou@gmail.com)		
//  Date: Dec 24,2013
//  Version: v1.0
//  by www.seeedstudio.com
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2.1 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//
/*******************************************************************************/
#include <EEPROM.h>
#include <IRSendRev.h>
#include <IRSendRevInt.h>
//#include <MagicBracelet.h>
#include <gesture.h>
#include <Wire.h>
#include <ADXL345.h>

#define IR_STRUCT_LEN	32
Gesture gesture;
static unsigned char IRRawData[][IR_STRUCT_LEN] = {
  {1,'A','I','R','\0','\0',19,72,31,9,25,14,136,136,136,136,136,4,192,96,48,0,0,0,0,81}, //turn off
  {2,'A','I','R','\0','\0',19,72,31,9,25,14,136,136,136,136,136,36,192,96,48,0,0,0,0,113}, //turn on	
};

void setup()
{
    Serial.begin(9600);
    vibrate_init();
    gesture.init();
    Wire.begin();	
}

void loop()
{
    gesture.wakeUp();
    delay(50);
    if(gesture.gestureWakeUp){
        vibrate();
        gesture.gestureWakeUp = 0;
        enterGestureMode();
    }
}

void vibrate_init()
{
    DDRF |= 0x01;	
    DDRB |= 0x04;	
}

void vibrate()
{
  //turn on the vibration motor
    PORTF |= 0x01;		
    PORTB |= 0x04;	
    delay(1000);	
  //turn off the vibration motor	
    PORTF &= ~(0x01);	
    PORTB |= 0x04;	
    delay(500);	  
}

void enterGestureMode(void)
{
    if(!gesture.samplingAccelerateData){       
	gesture.checkMoveStart();
    }
    if(gesture.samplingAccelerateData){
        gesture.getAccelerateData();
    }
    if(gesture.calculatingAccelerateData){
        int matchingResult = gesture.calculateAccelerateData();
        Serial.println(matchingResult);
        if((matchingResult >= 2) && (matchingResult <= 3)){
            for(int i = 0; i < 3; i++){
                IR.Send(&IRRawData[matchingResult-2][6],38);
                delay(1000);
            }
        }
    }    
}

