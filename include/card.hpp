#include <vector>
#pragma once

struct Card {
    int value;
    int cat_faces;
};

class Deck
{
    public:
        Deck();
        std::vector<Card> cards;
};

Card drawCard(Deck& deck);
void deleteCard(Deck& deck, int cardIndex);
void deleteCardFromVec(std::vector<Card>& card_vector, Card& card);