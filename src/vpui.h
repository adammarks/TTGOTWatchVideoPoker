#ifndef _WIN32
#pragma once
#include "vp.h"

class PokerUI
{
public:
   PokerUI();
   ~PokerUI();
   void showInitialUI();

   void OnBetDealClicked();

private:
   void showCredits();
   void showHand();
   void firstDeal();
   void secondDeal();

   void createButton(
       char *text,
       char c,
       int x, int y, int sx = 48, int sy = 90,
       lv_obj_t **plabel = nullptr,
       lv_obj_t **pbtn = nullptr,
       bool checkable = false);

   std::vector<std::string> origDeck;

   lv_obj_t *lblWonOrLost = nullptr;
   lv_obj_t *lblDeal = nullptr;
   lv_obj_t *lblCredits = nullptr;

   lv_obj_t *btnCard[5] = {nullptr, nullptr, nullptr, nullptr, nullptr};
   lv_obj_t *lblCard[5] = {nullptr, nullptr, nullptr, nullptr, nullptr};

   std::vector<std::string> deck;
   std::vector<std::string> hand;

   int deal = 1;
   int credits = 100;

   PokerGame pokerGame;
};
#endif