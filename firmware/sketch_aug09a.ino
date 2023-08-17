/* Sleep Demo Serial
 * -----------------
 * Example code to demonstrate the sleep functions in a Arduino. Arduino will wake up
 * when new data is received in the serial port USART
 * Based on Sleep Demo Serial from http://www.arduino.cc/playground/Learning/ArduinoSleepCode
 *
 * Copyright © 2006 MacSimski 2006-12-30
 * Copyright © 2007 D. Cuartielles 2007-07-08 - Mexico DF
 *
 * With modifications from Ruben Laguna 2008-10-15
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <avr/power.h>
#include <avr/sleep.h>

#define USB      4
#define SHUTTER  2
#define FOCUS    3


int sleepStatus = 0; // variable to store a request for sleep
int count = 0; // counter

void setup()
{
  pinMode(USB, OUTPUT);
  digitalWrite(USB, LOW);
  pinMode(SHUTTER, OUTPUT);
  digitalWrite(SHUTTER, LOW);
  pinMode(FOCUS, OUTPUT);
  digitalWrite(FOCUS, LOW);
  
  Serial.setTimeout(20);
  Serial.begin(115200);
}

void sleepNow()
{
/* Now is the time to set the sleep mode. In the Atmega8 datasheet
 * http://www.atmel.com/dyn/resources/prod_documents/doc2486.pdf on page 35
 * there is a list of sleep modes which explains which clocks and
 * wake up sources are available in which sleep modus.
 *
 * In the avr/sleep.h file, the call names of these sleep modus are to be found:
 *
 * The 5 different modes are:
 * SLEEP_MODE_IDLE -the least power savings
 * SLEEP_MODE_ADC
 * SLEEP_MODE_PWR_SAVE
 * SLEEP_MODE_STANDBY
 * SLEEP_MODE_PWR_DOWN -the most power savings
 *
 * the power reduction management <avr/power.h> is described in
 * http://www.nongnu.org/avr-libc/user-manual/group_avr_power.html
 */

  set_sleep_mode(SLEEP_MODE_IDLE); // sleep mode is set here
  
  sleep_enable(); // enables the sleep bit in the mcucr register
  // so sleep is possible. just a safety pin
  
  power_adc_disable();
  power_spi_disable();
  power_timer0_disable();
  power_timer1_disable();
  power_timer2_disable();
  power_twi_disable();
  
  sleep_mode(); // here the device is actually put to sleep!!
  
  // THE PROGRAM CONTINUES FROM HERE AFTER WAKING UP
  sleep_disable(); // first thing after waking from sleep:
  // disable sleep...
  
  power_all_enable();

}

void loop()
{

  // compute the serial input
  if (Serial.available())
  {
    String input = Serial.readString();
    bool done = false;

    // Strip any kind of EOL characters from command
    if (input.endsWith("\r\n"))  // this must come first
    {
      input = input.substring(0,input.length()-2);
    }
    if (input.endsWith("\n") || input.endsWith("\r"))
    {
      input = input.substring(0,input.length()-1);
    }
    
    // Process commands
    if (input == "U1")
    {
      Serial.println("Enabling USB");
      digitalWrite(USB, HIGH);
      done = true;
    }
    if (input == "U0")
    {
      Serial.println("Disabling USB");
      digitalWrite(USB, LOW);
      done = true;
    }
    if (input == "S1")
    {
      Serial.println("Enabling shutter");
      digitalWrite(SHUTTER, HIGH);
      done = true;
    }
    if (input == "S0")
    {
      Serial.println("Disabling shutter");
      digitalWrite(SHUTTER, LOW);
      done = true;
    }
    if (input == "F1")
    {
      Serial.println("Enabling focus");
      digitalWrite(FOCUS, HIGH);
      done = true;
    }
    if (input == "F0")
    {
      Serial.println("Disabling focus");
      digitalWrite(FOCUS, LOW);
      done = true;
    }
    if (input == "A0")
    {
      Serial.println("Disabling ALL");
      digitalWrite(USB, LOW);
      digitalWrite(FOCUS, LOW);
      digitalWrite(SHUTTER, LOW);
      done = true;
    }
    if (done != true)
    {
      Serial.println("Unknown command.");
      done = true;
    }
  }
  
  Serial.println("Timer: Entering Sleep mode");
  delay(100); // this delay is needed, the sleep
  //function will provoke a Serial error otherwise!!
  sleepNow(); // sleep function called here
}
