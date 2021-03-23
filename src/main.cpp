#ifndef _WIN32
#include "config.h"
#include <LilyGoWatch.h>
#include <Arduino.h>
#include <TTGO.h>

#include "vpui.h"

#include <iostream>

TTGOClass *ttgo = nullptr;

PokerUI pokerUI;

void setup()
{
   Serial.begin(115200);

   ttgo = TTGOClass::getWatch();
   ttgo->begin();
   ttgo->lvgl_begin();
   ttgo->openBL();
   ttgo->setBrightness(100);

   srand(esp_random());

   pokerUI.showInitialUI();
}

void loop()
{
   // LGVL handler
   lv_task_handler();
}

#endif