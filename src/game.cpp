#include<vector>
#include"../include/game.hpp"
#include"../include/card.hpp"

void sortCards(std::vector<Card>& arr) { 
  
    // Bubble sort in ascending order
    size_t n = arr.size();
    for (size_t i = 0; i < n - 1; i++) {
        for (size_t j = 0; j < n - 1 - i; j++) {
            if (arr[j].value > arr[j + 1].value) {
                std::swap(arr[j], arr[j + 1]);
                
            }
        }
    }
}

void sortCardsAndName(std::vector<Card>& arr, std::vector<std::string>& str_arr) { 
    // Same ascending bubble sort but sorts an Adjacent string array too
    size_t n = arr.size();
    for (size_t i = 0; i < n - 1; i++) {
        for (size_t j = 0; j < n - 1 - i; j++) {
            if (arr[j].value > arr[j + 1].value) {
                std::swap(arr[j], arr[j + 1]);
                std::swap(str_arr[i], str_arr[j+1]);
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

