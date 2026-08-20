#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <vector>
#include "../include/tui.hpp"
#include "../include/card.hpp"
#include "../include/game.hpp"

#define WINDOW_WIDTH 80
#define WINDOW_HEIGHT 22

int integerInput()
{
    int n;
    std::cout << "PLR INPUT >> ";
    std::cin >> n;
    return n;
}

std::string stringInput()
{
    std::string s;
    std::cout << "PLR INPUT >> ";
    std::cin >> s;
    return s;
}

int rangedIntegerInput(int lower_limit, int upper_limit){
    int n = integerInput();
    while (n < lower_limit || n > upper_limit)
    {
        std::cout << "ERROR: Invalid Input\n";
        n = integerInput();
    }
    return n;
}

int openHomeMenu(TUI& game_window)
{
    game_window.flush();
    game_window.appendEmptyBox();
        
    game_window.appendText(1, 38, "HOME");
    game_window.appendText(14, 10, "[0]: New game");
    game_window.appendText(15, 10, "[1]: Quit game");

    game_window.appendText(5, 31, "   ______   _____");
    game_window.appendText(6, 31, "  / ____/  / ___/");
    game_window.appendText(7, 31, " / /      / __ \\ ");
    game_window.appendText(8, 31, "/ /___   ____/ /");
    game_window.appendText(9, 31, "\\____/   \\____/  ");

    game_window.draw();

    int state = integerInput();
    while (state < 0 || state > 1)
    {
        std::cout << "Out of range\n";
        state = integerInput();
    }

    return state;
}

std::vector<std::string> openGetPlayers(TUI& game_window)
{
    std::vector<std::string> plr_names;

    game_window.flush();
    game_window.appendEmptyBox();
    game_window.appendText(1, 39, "C5");
    game_window.appendText(9,10, "Enter Number of Human Players (1-3)");
    game_window.draw();
    
    int human_plr_num = rangedIntegerInput(1,3);

    for (int i = 0; i < human_plr_num; i++)
    {
        game_window.flush();
        game_window.appendEmptyBox();
        game_window.appendText(1,39,std::to_string(i+1));
        game_window.appendText(9,10, "Enter Name of Human Player No. " + std::to_string(i+1));
        game_window.draw();
        std::string name = stringInput();
        plr_names.push_back(name);
    } 

    return plr_names;
}

bool openGetAIPlayers(TUI& game_window)
{
    game_window.flush();
    game_window.appendEmptyBox();

    game_window.appendText(1, 33, "ADD COMPUTER PLAYER");
    
    game_window.appendText(7, 10, "Would you like to add a computer player?");
    game_window.appendText(9, 15, "[1] Yes");
    game_window.appendText(10, 15, "[2] No");
    
    game_window.draw();

    int choice = rangedIntegerInput(1,2);

    return (choice == 1);  // Returns true if Yes, false if No
}

void printRows(const Board& board, TUI& game_window, int CurrRound){
    // print all rows and their respective cards

    game_window.flush();
    game_window.appendEmptyBox();
    game_window.appendText(1, 35, "ROUND "+std::to_string(CurrRound));
    
    std::vector<std::vector<Card>> rows = {board.row1, board.row2, board.row3, board.row4};
    
    for (int i = 0; i < 4; i++) {
        std::string row_str = "[ROW " + std::to_string(i+1) + "]: ";
        for (const auto& card : rows[i]) {
            row_str += std::to_string(card.value) + "(" + std::to_string(card.cat_faces) + ")" + " ";
        }
        game_window.appendText(4 + 2*i, 10, row_str);
    }
    
    game_window.draw();
}

void initHands(std::vector<std::vector<Card>>& CURRENT_HANDS, Deck& deck){
    for (auto& hand : CURRENT_HANDS){ // Loop through each hand of each plr
        for (int i=0;i<10;i++){ // Draw ten cards each hand
            hand.push_back(drawCard(deck));
        }
    }
}

void printHand(std::vector<Card>& Hand, std::string plrName,
    Board& board, TUI& game_window){ // References a specific hand in CURRENT_HANDS
    std::string hand_str1;
    std::string hand_str2;
    game_window.appendText(12,9, "|====================== " + plrName + "'s Hand: " + "======================|");
    int idx = 0;
    for (auto& c : Hand){
        if (idx < 5){
            hand_str1 += "[" + std::to_string(idx+1) + "] " +
            std::to_string(c.value) + "(" + std::to_string(c.cat_faces) + ")";
            if (idx != 4){hand_str1 += ", ";};
        } else {
            hand_str2 += "[" + std::to_string(idx+1) + "] " +
            std::to_string(c.value) + "(" + std::to_string(c.cat_faces) + ")";
            if (idx != 9){hand_str2 += ", ";};
        }
        idx++;
    }
    std::string border = "|=======================";
    border.append(plrName.length()+8, '=');  // Add '=' for each character in name
    border.append("=======================|");
    game_window.appendText(17,9, border);
    game_window.appendText(14,10,hand_str1);
    game_window.appendText(15,10,hand_str2);
    game_window.appendText(19,10,"[1-10] Choose a card to play");
    game_window.draw();
}

int pickRow(TUI& game_window, std::string plrName){
    std::string spaces;
    spaces.append(WINDOW_WIDTH-2,' '); // Char: ' ' * WINDOW_WIDTH
    game_window.appendText(19, 1, spaces); // Flush out the 19th row
    game_window.draw();
    game_window.appendText(19, 10, "[1-4] Pick a row to collect, "+plrName);
    game_window.draw();

    int plrInput = rangedIntegerInput(1, 4);
    return plrInput;
}

void showRoundSummary(TUI& game_window,
    std::vector<std::string>& plr_names_order,
    std::vector<std::vector<Card>>& CURRENT_COLLECTS, int roundNo){
    
    game_window.flush();
    game_window.appendEmptyBox();
    game_window.appendText(1, 33, "ROUND "+std::to_string(roundNo)+" SUMMARY");
    int idx = 0;
    for (auto& coll_pile : CURRENT_COLLECTS){
        int catfaceSum = 0; // Sum of catfaces
        std::string string1 = plr_names_order[idx] + "'s pile: "; 
        std::string string2;
        // Stores the player's individual performance (No. of cf, sum of cf) as one string to display
        
        for (auto& card : coll_pile){
            catfaceSum += card.cat_faces;
            string2 += std::to_string(card.value) + "(" + std::to_string(card.cat_faces) + ") ";
        }

        if (catfaceSum == 0) {string2 = "(Empty)";};

        std::string string3 = plr_names_order[idx] + "'s Catface total: " + std::to_string(catfaceSum);
        std::string string4 = "|";
        string4.append(35, '='); string4 += "|";

        game_window.appendText(4 + 4*idx, 5, string1);
        game_window.appendText(5 + 4*idx, 5, string2);
        game_window.appendText(6 + 4*idx, 5, string3);
        game_window.appendText(7 + 4*idx, 4, string4);

        idx++;
    }
    game_window.draw();

}

void showTurnOrder(TUI& game_window, std::vector<std::string>plr_names_order){
    game_window.flush();
    game_window.appendEmptyBox();
    int idx = 0;
    for (auto& name : plr_names_order){
        std::string st = "Turn "+ std::to_string(idx+1) + ": " + name;
        game_window.appendText(6 + idx*2, 10, st);
        idx++;
    }
    game_window.appendText(14, 10, "Turn Order. Input any number to continue...");
    game_window.draw();
}

#define MAX_TURNS 10
#define MAX_CARDS_PER_ROW 5
#define MAX_CATFACES 66

// CURRENT_COLLECTS and CURRENT_HAND needs to persist, so return sorted player names instead
std::vector<std::string> makeRound(Board& board, TUI& game_window, int roundNum, 
    const std::vector<std::string> plr_names,
    std::vector<std::vector<Card>>& CURRENT_HANDS,
    std::vector<std::vector<Card>>& CURRENT_COLLECTS){ // Takes care of everything in a round
    // OK 1: Print the board 
    // OK 2: For each player, print their hand, ask for input (check if Computer, rng if need)
    // OK 3: Determine turn order (sort the cards AND the players arrays in same func.)
    // OK 4: Place the cards (find correct placement), remove from hand, ask for input if needed, clear row if need (check if is Computer, then rng it if so)
    // 5: Calculate catfaces
    
    int idx = 0;
    std::vector<std::string> plr_names_order = plr_names;
    std::vector<Card> CHOSEN_CARDS;
    for (auto& plrName : plr_names){ // all players
        int plrInput;
        if(plrName == "Computer"){
            plrInput = rand() % CURRENT_HANDS[idx].size();

            printRows(board, game_window,roundNum);
            printHand(CURRENT_HANDS[idx], plrName, board, game_window);
            game_window.appendText(19, 10, "Computer's Hand. Input any number to continue.");
            game_window.draw();
            int dummy_input = integerInput();
        } else {
            printRows(board, game_window,roundNum);
            printHand(CURRENT_HANDS[idx], plrName, board, game_window);

            plrInput = rangedIntegerInput(1,CURRENT_HANDS[idx].size());
        }
        CHOSEN_CARDS.push_back(CURRENT_HANDS[idx][plrInput-1]); // Add Corresponding card
        deleteCardFromVec(CURRENT_HANDS[idx], CURRENT_HANDS[idx][plrInput-1]);
        // Remove card from hand, now card is in CHOSEN_CARDS, which is a temporary Vector
        idx++;
    }
    sortCards_Names_Collects(CHOSEN_CARDS, plr_names_order, CURRENT_COLLECTS);
    // Sort the plr_names_order & Chosen cards & Collects order in ASCENDING

    showTurnOrder(game_window, plr_names_order);
    int dummy_input = integerInput();

    std::vector<std::vector<Card>*> rows = {&board.row1, &board.row2, &board.row3, &board.row4};
    int idx2 = 0; // Arbitrary index for counting
    for (auto& c : CHOSEN_CARDS){
        int corr_row = board.GetRowPlacement(c);

        if (corr_row == -1){ // Code -1: Pick a row, Collect the row , place the card
            int plrInputRow;
            if (plr_names_order[idx2] == "Computer") {
                plrInputRow = rand()%4 + 1; // RNG pick if computer player
            } else {
                printRows(board, game_window, roundNum);
                plrInputRow = pickRow(game_window, plr_names_order[idx2]);
            }

            board.CollectRow(*rows[plrInputRow-1], CURRENT_COLLECTS[idx2]);
            board.PlaceCard(c,*rows[plrInputRow-1]);

        } else {
            // Check row to see if full
            // the following if statement: Gets pointer to corr_row, access it, gets its size, compare with MAX_CARDS_PER_ROW
            if (rows[corr_row-1]->size() == MAX_CARDS_PER_ROW) { // Row is full: collect row, place the card
                board.CollectRow(*rows[corr_row-1], CURRENT_COLLECTS[idx2]);
                board.PlaceCard(c,*rows[corr_row-1]);

            } else { // OK, place the card
                board.PlaceCard(c,*rows[corr_row-1]);
            }
        }

        // Redraw the Rows everytime after placement
        printRows(board, game_window, roundNum);
        game_window.appendText(18, 10, "Turn "+std::to_string(idx2+1)+" Results. Input any number to continue...");
        game_window.draw();
        dummy_input = integerInput();

        idx2++;
    }

    // ROUND IS COMPLETED HERE: Calculate Catfaces
    // 1. Show Collect piles of each player on one screen
    // 2. Show sum of catfaces
    // Basically a summary screen for each round end
    showRoundSummary(game_window, plr_names_order, CURRENT_COLLECTS, roundNum);
    return plr_names_order; //  POTENTIAL PROBLEM: CURRENT_COLLECTS is SORTED, but plr_names ISNT
                            //  ANOTHER PROBLEM: CURRENT_HANDS is SORTED + needs to persist too!
}

int main(){
    
    std::srand(std::time(0));   // Generate time seed in order to use rand() function from cstdlib
    TUI game_window(WINDOW_HEIGHT, WINDOW_WIDTH);
    
    int state = openHomeMenu(game_window);
  
    if (state == 1){
        std::cout << "Exiting..." << std::endl;
    } else {
        // Handles all pre-game set up, such as names, and distribuing 10 cards

        std::vector<std::string> plr_names = openGetPlayers(game_window);
        bool have_comp_plr = openGetAIPlayers(game_window); 
        
        int plrNum = plr_names.size();
        if (have_comp_plr){plrNum++; plr_names.push_back("Computer");};  // Kinda hardcoded, just gives total no. of players
                                                                        // And adds a comp player if have

        Deck card_deck; // Initialize a deck with 104 cards and respective catfaces
        Board board(card_deck); // Initialize board by drawing 4 cards and placing them in the rows
        std::vector<std::vector<Card>> CURRENT_HANDS(plrNum); // Stores all card hands of human and computer players combined
        initHands(CURRENT_HANDS, card_deck);

        std::vector<std::vector<Card>> CURRENT_COLLECTS(plrNum);

        // makeRound returns CURRENT_COLLECT to keep track of all collected cards of all players
        plr_names = makeRound(board, game_window, 1, plr_names, CURRENT_HANDS, CURRENT_COLLECTS); // Make ONE round for now, loop this LATER
    }     
    return 0;
}
