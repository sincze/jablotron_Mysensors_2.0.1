/**
 * The MySensors Arduino library handles the wireless radio link and protocol
 * between your home built sensors/actuators and HA controller of choice.
 * The sensors forms a self healing radio network with optional repeaters. Each
 * repeater and gateway builds a routing tables in EEPROM which keeps track of the
 * network topology allowing messages to be routed to nodes.
 *
 * Created by Henrik Ekblad <henrik.ekblad@mysensors.org>
 * Copyright (C) 2013-2015 Sensnology AB
 * Full contributor list: https://github.com/mysensors/Arduino/graphs/contributors
 *
 * Documentation: http://www.mysensors.org
 * Support Forum: http://forum.mysensors.org
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 *******************************
 *
 * DESCRIPTION
 *
 * Simple binary switch example 
 * Connect button or door/window reed switch between 
 * digitial I/O pin 3 (BUTTON_PIN below) and GND.
 * http://www.mysensors.org/build/binary
 * 
 * In addition we will be monitoring a Jablotron alarm system.
 * Wire 12v and PGX/PGY of the alarm via BUCK converter to D4 a max 4,45v for Digital 1. (I am using 5v arduino)
 */


// Enable debug prints to serial monitor
#define MY_DEBUG 

// Enable and select radio type attached
#define MY_RADIO_NRF24
//#define MY_RADIO_RFM69

// Flash leds on rx/tx/err

// Set blinking period (in milliseconds)
#define MY_DEFAULT_LED_BLINK_PERIOD 300

//#define MY_DEFAULT_ERR_LED_PIN 4
#define MY_DEFAULT_TX_LED_PIN 5               // I am using LED to display the activity TX on D5
#define MY_DEFAULT_RX_LED_PIN 6             // I am using LED to display the activity RX on D6

#include <SPI.h>
#include <MySensors.h>
#include <Bounce2.h>

#define NUMBER_OF_SWITCHES 2
int oldValue[NUMBER_OF_SWITCHES];

byte switchPin[NUMBER_OF_SWITCHES] = {3,4};   //<<<<<<<<<<< set your switch pins here D3 is door switch, D4 is Jablotron signal..

Bounce debouncer[NUMBER_OF_SWITCHES];

MyMessage msg(0,V_TRIPPED);

void setup()  
{  
  pinMode(switchPin[0],INPUT_PULLUP);         // D3 Door PIN needs pull up
  pinMode(switchPin[1],INPUT);               // D4 Jablotron PIN does NOT need pull up!! Danger Danger overvoltage.
  
  for (int i = 0; i < NUMBER_OF_SWITCHES; i++)
  {
    debouncer[i] = Bounce();
    debouncer[i].attach(switchPin[i]);
    debouncer[i].interval(5);
  }
}

void presentation() {
  // Register binary input sensor to gw (they will be created as child devices)
  // You can use S_DOOR, S_MOTION or S_LIGHT here depending on your usage. 
  // If S_LIGHT is used, remember to update variable type you send in. See "msg" above.
  // present(CHILD_ID, S_DOOR);  

  // Send the sketch version information to the gateway and Controller
  sendSketchInfo("OASIS and Door Detector", "1.0");  

  for (int i = 0; i < NUMBER_OF_SWITCHES; i++)
  {
    present(i, S_DOOR);
    delay(250);
  }

  
}


//  Check if digital input has changed and send in new value
void loop() 
{
   for (int i = 0; i < NUMBER_OF_SWITCHES; i++)
  {
    debouncer[i].update();
    int value = debouncer[i].read();
    if (value != oldValue[i]) 
    {
      send(msg.setSensor(i).set(value == HIGH? true : false), false); 
      //send(msg.setSensor[i].set(value?"1":"0")); 
    }
    oldValue[i] = value;
  }
}
