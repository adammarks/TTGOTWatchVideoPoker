#pragma once

#include <ctime>
#include <list>
#include <vector>
#include <map>

class PokerHand
{
public:
   bool isRoyalFlush = false;    // 250
   bool isStraightFlush = false; // 50
   bool is4OfAKind = false;      // 25
   bool isFullHouse = false;     // 9
   bool isFlush = false;         // 6
   bool isStraight = false;      // 4
   bool is3OfAKind = false;      // 3
   bool is2Pair = false;         // 2
   bool isHighPair = false;      // 1
   int payout()
   {
      if (isRoyalFlush) return 250;
      else if (isStraightFlush) return 50;
      else if (is4OfAKind) return 25;
      else if (isFullHouse) return 9;
      else if (isFlush) return 6;
      else if (isStraight) return 4;
      else if (is3OfAKind) return 3;
      else if (is2Pair) return 2;
      else if (isHighPair) return 1;
      return 0;
   }
   std::string string()
   {
      if (isRoyalFlush) return "Royal Flush";
      else if (isStraightFlush) return "Straight Flush";
      else if (is4OfAKind) return "4 of a kind";
      else if (isFullHouse) return "Full House";
      else if (isFlush) return "Flush";
      else if (isStraight) return "Straight";
      else if (is3OfAKind) return "3 of a kind";
      else if (is2Pair) return "2 Pair";
      else if (isHighPair) return "Jacks or Higher";
      return 0;
   }

};


class PokerGame
{
public:
   PokerGame();
   ~PokerGame() {};
   PokerHand DetectWinningHands(std::vector<std::string> &hand);

   void CreateDeck(std::vector<std::string> &deck);


   void DealNewCard(
      std::vector<std::string> &deck,
      std::vector<std::string> &hand,
      int indexOfNewCardToDeal);


   void DealHand(
      std::vector<std::string> &deck,
      std::vector<std::string> &hand,
      int numberOfCardsToDeal);


};
