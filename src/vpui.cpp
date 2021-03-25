#ifndef _WIN32
#include "config.h"

#include <LilyGoWatch.h>
#include <Arduino.h>
#include <TTGO.h>

#include "vpUI.h"
#include "vp.h"

#include <iostream>

// TODO : Select cards to keep / hold instead of opposite
// TODO : disable buttons on bet screen

LV_FONT_DECLARE(ArimoSubset);

static lv_obj_t *lvglpage = NULL;
static void event_handler(lv_obj_t *obj, lv_event_t event);

static PokerUI* thePokerUI = nullptr;

PokerUI::PokerUI()
{
   thePokerUI = this;
}

PokerUI::~PokerUI()
{
}

void PokerUI::showCredits()
{
   lv_label_set_text_fmt(lblCredits, "Credits %d", credits);
}

void PokerUI::showHand( bool cardsClickable )
{
   for (int i = 0; i < 5; i++)
   {
      #define HEART "\xE2\x99\xA5"
      #define SPADE "\xE2\x99\xA0"
      #define CLUB  "\xE2\x99\xA3"
      #define DIAMOND "\xE2\x99\xA6"
      char cardText[10] = "";
      cardText[0] = hand[i][0];
      cardText[1] = 0;
      if ( cardText[0]=='T') strcpy( cardText, "10" );
      char suit = hand[i][1];
      strcat( cardText,"\n\n");
      if ( suit == 'h') strcat( cardText, HEART );
      if ( suit == 'd') strcat( cardText, DIAMOND );
      if ( suit == 'c') strcat( cardText, CLUB );
      if ( suit == 's') strcat( cardText, SPADE );
      lv_obj_set_style_local_text_font( lblCard[i], LV_LABEL_PART_MAIN,LV_STATE_DEFAULT,&ArimoSubset);
      auto suitColor = LV_COLOR_BLACK;
      if ( suit == 'h' || suit == 'd' ) suitColor = LV_COLOR_RED;
      lv_obj_set_style_local_text_color( lblCard[i], LV_LABEL_PART_MAIN,LV_STATE_DEFAULT, suitColor);
      lv_obj_set_style_local_radius( btnCard[i], LV_LABEL_PART_MAIN,LV_STATE_DEFAULT, 15);

      lv_obj_set_style_local_border_side( btnCard[i], LV_BTN_PART_MAIN,LV_BTN_STATE_RELEASED, LV_BORDER_SIDE_FULL);
      lv_obj_set_style_local_border_side( btnCard[i], LV_BTN_PART_MAIN,LV_BTN_STATE_PRESSED, LV_BORDER_SIDE_FULL);
      lv_obj_set_style_local_border_side( btnCard[i], LV_BTN_PART_MAIN,LV_BTN_STATE_CHECKED_RELEASED, LV_BORDER_SIDE_FULL);
      lv_obj_set_style_local_border_side( btnCard[i], LV_BTN_PART_MAIN,LV_BTN_STATE_CHECKED_PRESSED, LV_BORDER_SIDE_FULL);

      lv_obj_set_style_local_border_color( btnCard[i], LV_BTN_PART_MAIN,LV_BTN_STATE_RELEASED, LV_COLOR_GREEN);
      lv_obj_set_style_local_border_color( btnCard[i], LV_BTN_PART_MAIN,LV_BTN_STATE_PRESSED, LV_COLOR_RED);
      lv_obj_set_style_local_border_color( btnCard[i], LV_BTN_PART_MAIN,LV_BTN_STATE_CHECKED_RELEASED, LV_COLOR_RED);
      lv_obj_set_style_local_border_color( btnCard[i], LV_BTN_PART_MAIN,LV_BTN_STATE_CHECKED_PRESSED, LV_COLOR_RED);

      lv_obj_set_style_local_border_width( btnCard[i], LV_BTN_PART_MAIN,LV_BTN_STATE_RELEASED, 6);
      lv_obj_set_style_local_border_width( btnCard[i], LV_BTN_PART_MAIN,LV_BTN_STATE_PRESSED, 6);
      lv_obj_set_style_local_border_width( btnCard[i], LV_BTN_PART_MAIN,LV_BTN_STATE_CHECKED_RELEASED, 6);
      lv_obj_set_style_local_border_width( btnCard[i], LV_BTN_PART_MAIN,LV_BTN_STATE_CHECKED_PRESSED, 6);


      #define LIGHT_GRAY LV_COLOR_MAKE( 200, 200, 200 )
      // lv_obj_set_style_local_bg_color( btnCard[i], LV_BTN_PART_MAIN,LV_BTN_STATE_RELEASED, LV_COLOR_WHITE);
      lv_obj_set_style_local_bg_color( btnCard[i], LV_BTN_PART_MAIN,LV_BTN_STATE_PRESSED, LIGHT_GRAY);
      lv_obj_set_style_local_bg_color( btnCard[i], LV_BTN_PART_MAIN,LV_BTN_STATE_CHECKED_RELEASED, LIGHT_GRAY);
      lv_obj_set_style_local_bg_color( btnCard[i], LV_BTN_PART_MAIN,LV_BTN_STATE_CHECKED_PRESSED, LIGHT_GRAY);
    
      lv_label_set_text(lblCard[i], cardText);
      lv_btn_set_state(btnCard[i], LV_BTN_STATE_RELEASED);

      lv_obj_set_click( btnCard[i], cardsClickable );      
   }
}

void PokerUI::firstDeal()
{
   deal = 1;
   credits--;

   deck = origDeck;
   hand.clear();
   pokerGame.DealHand(deck, hand, 5);

   showHand( true );

   lv_label_set_text(lblDeal, "Deal");
   lv_label_set_text_fmt(lblWonOrLost,"Select Cards to Replace" );
 
   showCredits();
}

void PokerUI::secondDeal()
{
   deal = 2;

   for (int i = 0; i < 5; i++)
   {
      Serial.printf("Button %d state %d\n", i, lv_btn_get_state(btnCard[i]));
      if (lv_btn_get_state(btnCard[i]) == LV_BTN_STATE_CHECKED_RELEASED)
      {
         pokerGame.DealNewCard(deck, hand, i);
         Serial.printf("New card dealt Button %d card %s\n", i, hand[i].c_str());
      }
   }

   showHand( false );

   lv_label_set_text(lblDeal, "Bet");

   auto pokerHand = pokerGame.DetectWinningHands(hand);
   int payout = pokerHand.payout();
   if ( payout > 0 )
   {
      lv_label_set_text_fmt(lblWonOrLost,"Won %d with %s", payout, pokerHand.string().c_str() );
      credits += payout;
   }
   else
   {
      lv_label_set_text(lblWonOrLost,"No Winning Hands");
   }

   showCredits();
}

void PokerUI::showInitialUI()
{
   // Container that contain all displayed elements. Makes it easier to show or hide the page
   lvglpage = lv_cont_create(lv_scr_act(), NULL);
   lv_obj_set_width(lvglpage, lv_disp_get_hor_res(NULL));  // Horizontal resolution
   lv_obj_set_height(lvglpage, lv_disp_get_ver_res(NULL)); // Vertical resolution
   lv_obj_set_style_local_bg_color(lvglpage, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_GRAY );

   createButton("1", '1', 0, 10, 48, 90, &(lblCard[0]), &(btnCard[0]), true);
   createButton("2", '2', 48 * 1, 10, 48, 90, &(lblCard[1]), &(btnCard[1]), true);
   createButton("3", '3', 48 * 2, 10, 48, 90, &(lblCard[2]), &(btnCard[2]), true);
   createButton("4", '4', 48 * 3, 10, 48, 90, &(lblCard[3]), &(btnCard[3]), true);
   createButton("5", '5', 48 * 4, 10, 48, 90, &(lblCard[4]), &(btnCard[4]), true);

   createButton("Deal", 'd', 60, 120, 120, 40, &lblDeal);

   createButton("Won or Lost", ' ', 0, 180, 240, 30, &lblWonOrLost);
   createButton("Credits 100", ' ', 0, 210, 240, 30, &lblCredits);

   pokerGame.CreateDeck(origDeck);
   firstDeal();
}

void PokerUI::createButton(
   char *text,
   char c,
   int x, int y, int sx /*= 48*/, int sy /*= 90*/,
   lv_obj_t **plabel /*= nullptr*/,
   lv_obj_t **pbtn /*= nullptr*/,
   bool checkable /*= false*/)
{
   // Button in center of the screen
   lv_obj_t *btn1 = lv_btn_create(lvglpage, NULL);
   btn1->user_data = (lv_obj_user_data_t)c;
   lv_obj_set_event_cb(btn1, event_handler);
   lv_obj_set_pos(btn1, x, y);
   lv_obj_set_size(btn1, sx, sy);

   if (checkable)
   {
      lv_btn_set_checkable(btn1, true);
      lv_btn_set_state(btn1, LV_BTN_STATE_RELEASED);
   }

   // Button label
   lv_obj_t *label = lv_label_create(btn1, NULL);
   lv_label_set_text(label, text);
   if (plabel)
      *plabel = label;
   if (pbtn)
      *pbtn = btn1;
}

void PokerUI::OnBetDealClicked()
{
   if (deal == 1)
   {
      Serial.println("Calling secondDeal()");
      secondDeal();
   }
   else
   {
      Serial.println("Calling firstDeal()");
      firstDeal();
   }
}



// LVGL handler
static void event_handler(lv_obj_t *obj, lv_event_t event)
{
   // Important !! always test event to avoid multiple signals
   if (event == LV_EVENT_CLICKED)
   {
      //char* text = lv_label_get_text(obj);
      char c = (char)(int)obj->user_data;
      if (c == 'd')
      {
         if ( thePokerUI )
         {
            thePokerUI->OnBetDealClicked();
         }
      }

   }
}

#endif