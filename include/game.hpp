#include<vector>
#include"card.hpp"
#include<string>
#pragma once

class Board
{
    public:
        Board(Deck& deck);
        int GetRowPlacement(Card card);
        void PlaceCard(Card& card, std::vector<Card>& row);
        void CollectRow(std::vector<Card>& row, std::vector<Card>& plrCollects);
        std::vector<Card> row1;
        std::vector<Card> row2;
        std::vector<Card> row3;
        std::vector<Card> row4;
};

void sortCards(std::vector<Card>& arr);
void sortCards_Names_Collects(std::vector<Card>& arr, std::vector<std::string>& str_arr, std::vector<std::vector<Card>>& collect_arr);