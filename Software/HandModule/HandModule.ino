/**
 * Arjuna Hand Module
 *
 * Copyright (c) 2015 Arjuna
 *
 * Developed by:
 *   Ilham Imaduddin
 *   Piquitha Della Audyna
 * 
 *   Gadjah Mada University
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
 
#include <SPI.h>
#include <printf.h>
#include <RF24.h>

class Motor
{
  enum MotorStatus{OFF, ON};
  const int TIMEOUT = 500;
  
  private:
    bool status;
    int pin;
    unsigned long activeOn;

  public:
    /**
     * Initialize Motor
     * 
     * Assign pin number and set it as output
     * 
     * @param pinNumber   Arduino Pin Number
     */
    void init(int pinNumber)
    {
      pin = pinNumber;
      pinMode(pin, OUTPUT);
    }

    /**
     * Turn On Motor
     */
    void turnOn()
    {
      digitalWrite(pin, HIGH);
      status = ON;
      activeOn = millis();
    }

    /**
     * Turn Off Motor
     */
    void turnOff()
    {
      digitalWrite(pin, LOW);
      status = OFF;
      activeOn = 0;
    }

    /**
     * Get Motor Status
     * 
     * @return  MotorStatus
     */
    bool getStatus()
    {
      return status;
    }

    /**
     * Control Motor Timeout
     */
    void timeoutControl()
    {
      if ((millis() - activeOn) > TIMEOUT)
      {
        turnOff();
      }
    }
};

#define MOTOR_COUNT     10
#define MOTOR_OFF       0x80
#define MOTOR_ON        0x90
#define ALL_MOTOR_OFF   0xC0
#define ALL_MOTOR_ON    0xD0

RF24 radio(18, 10);
byte data;
Motor motor[MOTOR_COUNT];
int motorPin[MOTOR_COUNT] = {1, 0, 2, 3, 4, 5, 6, 7, 8, 9};

void setup()
{
  /* Initialize RF Address */
  byte address[6] = "ArS01";

  /* Initialize serial communication */
  Serial.begin(9600);
  printf_begin();

  /* Initialize radio */
  Serial.println("Initializing radio...");
  radio.begin();
  radio.setPALevel(RF24_PA_LOW);
  radio.setPayloadSize(1);
  radio.setAutoAck(1);
  radio.openReadingPipe(1, address);
  radio.startListening();
  Serial.println("Radio is listening...");

  // Initializing Motor
  for (int i = 0; i < MOTOR_COUNT; i++)
  {
    motor[i].init(motorPin[i]);
  }
  allMotorOff();
}

void loop()
{
  if (radio.available())
  {
    while(radio.available());
    {
      radio.read(&data, 1);

      Serial.print("Data received: ");
      Serial.println(data, HEX);
      parseCommand(data);
    }
  }
  
  // Run Motor Timeout Controller
  for (int i = 0; i < MOTOR_COUNT; i++)
  {
    motor[i].timeoutControl();
  }
}

/**
 * Parse command
 * 
 * @param   message   command message
 */
void parseCommand(byte message)
{
  byte command = message & 0xF0;
  byte channel = message & 0x0F;

  if (channel > 9)
    return;

   switch(command)
   {
      case MOTOR_OFF:
        motor[channel].turnOff();
        break;
      case MOTOR_ON:
        motor[channel].turnOn();
        break;
      case ALL_MOTOR_OFF:
        allMotorOff();
        break;
      case ALL_MOTOR_ON:
        allMotorOn(); 
        break;
   }
}

/**
 * Turn Off All Motor
 */
void allMotorOn()
{
  for (int i = 0; i < MOTOR_COUNT; i++)
  {
    motor[i].turnOn();
  }
}

/**
 * Turn Off All Motor
 */
void allMotorOff()
{
  for (int i = 0; i < MOTOR_COUNT; i++)
  {
    motor[i].turnOff();
  }
}
