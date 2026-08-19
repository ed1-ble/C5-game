#include<iostream>
#include<vector>
#include"card.hpp"
#pragma once

class Board
{
    public:
        Board(Deck& deck);
        std::vector<Card> row1;
        std::vector<Card> row2;
        std::vector<Card> row3;
        std::vector<Card> row4;
};

void sortCards(std::vector<Card>& arr);
void sortCardsAndName(std::vector<Card>& arr, std::vector<std::string>& str_arr);
