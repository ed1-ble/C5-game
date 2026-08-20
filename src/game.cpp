#include<iostream>
#include<vector>
#include"../include/game.hpp"
#include"../include/card.hpp"

void sortCards(std::vector<Card>& arr) { 
    // Bubble sort in ascending order
    size_t n = arr.size();
    if (n == 0) {std::cout << "EMPTY ARRAY FOR SORTING";}
    else{
        for (size_t i = 0; i < n - 1; i++) {
            for (size_t j = 0; j < n - 1 - i; j++) {
                if (arr[j].value > arr[j + 1].value) {
                    std::swap(arr[j], arr[j + 1]);
                    
                }
            }
        }
    }
}

void sortCards_Names_Collects(std::vector<Card>& arr, 
    std::vector<std::string>& str_arr,
    std::vector<std::vector<Card>>& collect_arr) { 

    // Same ascending bubble sort but sorts Adjacent arrays too
    size_t n = arr.size();
    if (n == 0) {std::cout << "EMPTY ARRAY FOR SORTING";} else {
        for (size_t i = 0; i < n - 1; i++) {
            for (size_t j = 0; j < n - 1 - i; j++) {
                if (arr[j].value > arr[j + 1].value) {
                    std::swap(arr[j], arr[j + 1]);
                    std::swap(str_arr[j], str_arr[j+1]);
                    std::swap(collect_arr[j], collect_arr[j+1]);
                }
            }
        }
    }
}

Board::Board(Deck& deck){
    std::vector<Card> c;
    for (int i=0;i<4;i++){
        c.push_back(drawCard(deck));
    }
    
    sortCards(c); 

    row1.push_back(c[0]);
    row2.push_back(c[1]);
    row3.push_back(c[2]);
    row4.push_back(c[3]); // init first card for each row, in ascending order
}

int Board::GetRowPlacement(Card card){ 
  
    // Calculate and return the row to be placed in for the card
    int corr_row; // means CORRECT ROW
    // For this else case, it means that there MUST be at least one row with positive difference
    int minPosDiff = 1000; // Minimum positive difference, larger than, and smallest possible no.
    // Since row1-4 are vectors of cards, we can easily find the largest no. in each r
    // which is row[row.size()-1]
    int idx = 1;
    while (idx <= 4){ // loop thru all rows
        int diff;
        switch(idx){
            case 1: diff = card.value - row1[row1.size()-1].value; break;
            case 2: diff = card.value - row2[row2.size()-1].value; break;
            case 3: diff = card.value - row3[row3.size()-1].value; break;
            case 4: diff = card.value - row4[row4.size()-1].value; break;
        }
        if (diff > 0 and diff < minPosDiff){
            minPosDiff = diff;
            corr_row = idx;
        }
        idx++;
    }    

    if (minPosDiff == 1000) {corr_row = -1;}; // Which means minPosDiff wasnt modified, as there r only negative differences
    
    return corr_row;
}

void Board::PlaceCard(Card& card, std::vector<Card>& row){
    if (row.size() >= 5) {std::cout << "BUG: ROW IS FULL";} else {
        row.push_back(card);
    };
}

// Add all cards from row to collect, delete all cards in that row.
void Board::CollectRow(std::vector<Card>& row, std::vector<Card>& plr_collect_pile){
    for (auto& card : row){
        plr_collect_pile.push_back(card);
    }
    // seperate clearing from loop, because we dont want to delete cards from row while iterating
    row.clear();
}