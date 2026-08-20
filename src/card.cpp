#include "../include/card.hpp"
#include<cstdlib>
#pragma once

Deck::Deck() {
    for (int i = 1; i <= 104; i++)
    {
        Card c;
        c.value = i;
        c.cat_faces = i%7 + 1;
        cards.push_back(c);
    }
}

Card drawCard(Deck& deck){
    int n = rand() % deck.cards.size(); // gen a number from 0 to 103 
    // remove the card from deck and return it
    Card drawnCard = deck.cards[n];
    deck.cards.erase(deck.cards.begin() + n);      // Remove card at index n   
    return drawnCard;
}

void deleteCard(Deck& deck, int cardIndex){
    deck.cards.erase(deck.cards.begin() + cardIndex); 
}

void deleteCardFromVec(std::vector<Card>& card_vector, Card& card){
    for (int i =0;i<card_vector.size();i++){
        if (card_vector[i].value == card.value){
            card_vector.erase(card_vector.begin() + i); 
            return;
        }
    }
}