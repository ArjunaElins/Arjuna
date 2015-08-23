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

#define MOTOR_COUNT   10

Motor motor[MOTOR_COUNT];
int motorPin[MOTOR_COUNT] = {1, 0, 2, 3, 4, 5, 6, 7, 8, 9};

void setup()
{
  Serial.begin(9600);

  // Initializing Motor
  for (int i = 0; i < MOTOR_COUNT; i++)
  {
    motor[i].init(motorPin[i]);
  }
  allMotorOff();
}

void loop()
{

  
  // Run Motor Timeout Controller
  for (int i = 0; i < MOTOR_COUNT; i++)
  {
    motor[i].timeoutControl();
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