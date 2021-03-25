#include "vp.h"
#include <iostream>
#include <stdlib.h>

static char ranks[] = { '2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K', 'A' };
static char suits[] = { 'h', 'd', 'c', 's' };

PokerGame::PokerGame()
{
}

PokerHand PokerGame::DetectWinningHands(std::vector<std::string> &hand)
{
   std::map<char, int> rankCounts;
   std::map<char, int> suitCounts;

   for (auto card : hand)
   {
      auto rank = card[0];
      auto suit = card[1];
      rankCounts[rank]++;
      suitCounts[suit]++;
   }

   PokerHand pokerHand;

   bool isStraight = false;
   bool isRoyalStraight = false;

   if (rankCounts['T'] &&
      rankCounts['J'] &&
      rankCounts['Q'] &&
      rankCounts['K'] &&
      rankCounts['A'])
   {
      isRoyalStraight = true;
   }
   else if (rankCounts['A'] &&
      rankCounts['2'] &&
      rankCounts['3'] &&
      rankCounts['4'] &&
      rankCounts['5'])
   {
      isStraight = true;
   }
   else
   {
      for (int pos = 0; pos < sizeof(ranks) - 5; pos++)
      {
         if (rankCounts[ranks[pos]] &&
            rankCounts[ranks[pos + 1]] &&
            rankCounts[ranks[pos + 2]] &&
            rankCounts[ranks[pos + 3]] &&
            rankCounts[ranks[pos + 4]])
         {
            isStraight = true;
         }
      }
   }

   int numPair = 0;
   int numHighPair = 0;
   int num3OfAKind = 0;
   int num4OfAKind = 0;
   for (auto rankCount : rankCounts)
   {
      if (rankCount.second == 2)
      {
         numPair++;

         char rank = rankCount.first;
         if (rank == 'J' || rank == 'Q' || rank == 'K' || rank == 'A')
         {
            std::cout << "Found a pair( J or higher ) of " << rankCount.first << ", ";
            numHighPair++;
         }
         else
         {
            std::cout << "Found a pair of " << rankCount.first << ", ";
         }
      }

      if (rankCount.second == 3)
      {
         std::cout << "Found 3 of a kind of " << rankCount.first << ", ";
         num3OfAKind++;
      }
      if (rankCount.second == 4)
      {
         std::cout << "Found 4 of a kind of " << rankCount.first << ", ";
         num4OfAKind++;
      }
   }

   if (num4OfAKind > 0)
   {
      pokerHand.is4OfAKind = true;
      return pokerHand;
   }

   if (numPair > 0 && num3OfAKind > 0)
   {
      std::cout << "Found Full House, ";
      pokerHand.isFullHouse = true;
      return pokerHand;
   }

   if (num3OfAKind > 0)
   {
      pokerHand.is3OfAKind = true;
      return pokerHand;
   }

   if (numPair == 2)
   {
      std::cout << "Found 2 Pair, ";
      pokerHand.is2Pair = true;
      return pokerHand;
   }

   if (numHighPair > 0)
   {
      pokerHand.isHighPair = true;
      return pokerHand;
   }

   for (auto suitCount : suitCounts)
   {
      if (suitCount.second == 5)
      {
         if (isRoyalStraight)
         {
            std::cout << "Found Royal Flush, ";
            pokerHand.isRoyalFlush = true;
            return pokerHand;
         }
         else if (isStraight)
         {
            std::cout << "Found Straight Flush, ";
            pokerHand.isStraightFlush = true;
            return pokerHand;
         }

         std::cout << "Found Flush of, ";
         pokerHand.isFlush = true;
         return pokerHand;
      }
   }

   if (isStraight || isRoyalStraight)
   {
      std::cout << "Found Straight, ";
      pokerHand.isStraight = true;
      return pokerHand;
   }

   return pokerHand;
}


// void runTests()
// {
//    {
//       std::vector<std::string> hand{ "As", "2h", "3s", "4c", "5h" };
//       auto ph = DetectPatterns(hand);
//       assert(ph.isStraight == true);
//    }

//    {
//       std::vector<std::string> hand{ "As", "2s", "3s", "4s", "5s" };
//       auto ph = DetectPatterns(hand);
//       assert(ph.isStraightFlush == true);
//    }

//    {
//       std::vector<std::string> hand{ "Ts", "Js", "Qs", "Ks", "As" };
//       auto ph = DetectPatterns(hand);
//       assert(ph.isRoyalFlush == true);
//    }

//    {
//       std::vector<std::string> hand{ "As", "5s", "8s", "Ts", "Js" };
//       auto ph = DetectPatterns(hand);
//       assert(ph.isFlush == true);
//    }

// }

void PokerGame::CreateDeck(std::vector<std::string> &deck)
{
   int cardnum = 0;
   for (auto rank : ranks)
   {
      for (auto suit : suits)
      {
         std::string card;
         card = rank;
         card += suit;
         deck.push_back(card);
         //std::wstring card(deck[cardnum].begin(), deck[cardnum].end() );
         std::cout << deck[cardnum].c_str();
         std::cout << ",";
         cardnum++;
      }
      std::cout << std::endl;
   }
}

void PokerGame::DealNewCard(
   std::vector<std::string> &deck,
   std::vector<std::string> &hand,
   int indexOfNewCardToDeal)
{
   int chosenCard = (rand() % deck.size());
   auto card = deck[chosenCard];
   deck.erase(deck.begin() + chosenCard);
   hand[indexOfNewCardToDeal] = card;
}

void PokerGame::DealHand(
   std::vector<std::string> &deck,
   std::vector<std::string> &hand,
   int numberOfCardsToDeal)
{
   while (numberOfCardsToDeal >= 1)
   {
      int chosenCard = (rand() % deck.size());
      auto card = deck[chosenCard];
      deck.erase(deck.begin() + chosenCard);
      hand.push_back(card);
      numberOfCardsToDeal--;
   }

   std::cout << std::endl
      << "Hand : ";
   for (auto card : hand)
   {
      std::cout << card.c_str();
      std::cout << ",";
   }
   std::cout << std::endl;
}
