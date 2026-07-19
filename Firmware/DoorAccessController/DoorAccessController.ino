/*
===============================================================================
 DoorAccessController
 Commit 0001

 Main controller skeleton

 Hardware:
 Arduino Nano
 PN532 V3 SPI
===============================================================================
*/

#include "LED.h"
#include "Buzzer.h"
#include "Lock.h"
#include "PN532Manager.h"
#include "UID.h"
#include <SPI.h>
#include <EEPROM.h>
#include <avr/wdt.h>

#include "Config.h"
#include "Version.h"
#include "Logger.h"


//=============================================================================
// Objects
//=============================================================================

uint32_t bootCounter = 0;


//=============================================================================
// LED
//=============================================================================

void greenOn()
{
    digitalWrite(
        Config::Pin::LedGreen,
        HIGH
    );
}


void greenOff()
{
    digitalWrite(
        Config::Pin::LedGreen,
        LOW
    );
}


void redOn()
{
    digitalWrite(
        Config::Pin::LedRed,
        HIGH
    );
}


void redOff()
{
    digitalWrite(
        Config::Pin::LedRed,
        LOW
    );
}


//=============================================================================
// Buzzer
//=============================================================================

void beepOK()
{
    tone(
        Config::Pin::Buzzer,
        Config::Sound::OkFreq,
        Config::Sound::OkTime
    );
}


//=============================================================================
// Startup test
//=============================================================================

void startupTest()
{
    greenOn();

    delay(300);

    greenOff();


    redOn();

    delay(300);

    redOff();


    beepOK();
}


//=============================================================================
// Setup
//=============================================================================

void setup()
{

    wdt_disable();


    pinMode(
        Config::Pin::LedGreen,
        OUTPUT
    );


    pinMode(
        Config::Pin::LedRed,
        OUTPUT
    );


    pinMode(
        Config::Pin::Buzzer,
        OUTPUT
    );


    pinMode(
        Config::Pin::LockPWM,
        OUTPUT
    );


    pinMode(
        Config::Pin::ButtonRec,
        INPUT_PULLUP
    );


    analogWrite(
        Config::Pin::LockPWM,
        0
    );


    Serial.begin(
        Config::Serial::Speed
    );


    delay(500);


    Logger.begin();

LED::begin();

Buzzer::begin();

Lock::begin();


if(PN532Manager::begin())
{
    Logger.println(
        F("PN532 OK")
    );
}
else
{
    Logger.println(
        F("PN532 ERROR")
    );
}


    Logger.println(
        F("================================")
    );


    Logger.println(
        F("DoorAccessController")
    );


    Logger.println(
        F("Commit 0001")
    );


    Logger.print(
        F("Version: ")
    );


    Logger.println(
        Version::String
    );


    Logger.println(
        F("Boot OK")
    );


    LED::greenPulse();

    Buzzer::ok();


    wdt_enable(
        WDTO_2S
    );


    Logger.println(
        F("System ready")
    );

}


//=============================================================================
// Loop
//=============================================================================

void loop()
{

    wdt_reset();


    Lock::update();


    UID card;


    if(PN532Manager::readCard(card))
    {

        Logger.println(
            F("CARD DETECTED")
        );


        for(uint8_t i=0;i<card.length;i++)
        {
            Serial.print(card.data[i],HEX);
            Serial.print(' ');
        }

        Serial.println();


        LED::greenPulse();

        Buzzer::ok();


        delay(500);
    }


    delay(10);
}
