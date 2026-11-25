#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <vector>
#include "../include/tui.hpp"
#include "../include/card.hpp"
#include "../include/game.hpp"
#include <fstream>
#include <iomanip>
#include <sstream>

#define WINDOW_WIDTH 80
#define WINDOW_HEIGHT 22

int integerInput()
{
    int n;
    std::cout << "PLAYER INPUT >> ";
    std::cin >> n;

    if (std::cin.fail()) { // Type check, std::cin tries to convert n to INT, if fails, triggers std::cin.fail() = true
        std::cin.clear(); // Clear error state
        std::cin.ignore(10000, '\n'); // Discard invalid input til \n (trash in std::cin after failed convert)
        std::cout << "ERROR: Invalid Input (type)\n";
        return integerInput();
    }
    return n;
}

std::string stringInput()
{
    std::string s;
    std::cout << "PLAYER INPUT >> ";
    std::cin >> s;
    return s;
}

int rangedIntegerInput(int lower_limit, int upper_limit){
    int n = integerInput();
    while (n < lower_limit || n > upper_limit)
    {
        std::cout << "ERROR: Invalid Input (range)\n";
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

    int state = rangedIntegerInput(0, 1);

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
            if (idx != 4 && idx != Hand.size()-1){hand_str1 += ", ";};
        } else {
            hand_str2 += "[" + std::to_string(idx+1) + "] " +
            std::to_string(c.value) + "(" + std::to_string(c.cat_faces) + ")";
            if (idx != 9 && idx != Hand.size()-1){hand_str2 += ", ";};
        }
        idx++;
    }
    std::string border = "|=======================";
    border.append(plrName.length()+8, '=');  // Add '=' for each character in name
    border.append("=======================|");
    if (Hand.size() <= 5) {
        game_window.appendText(16,9, border);
    } else {
        game_window.appendText(17,9, border);
    }
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

// a seperate function from showRoundSummary (which displays each card with catfaces),
// this function tracks catface totals for checking whether or not the game should have ended
std::vector<int> getCatfaceTotals(std::vector<std::vector<Card>> CURRENT_COLLECTS){
    std::vector<int> catface_totals;
    
    for (auto& coll_pile : CURRENT_COLLECTS){
        int catfaceSum = 0;
        for (auto& card : coll_pile){
            catfaceSum += card.cat_faces;
        }
        catface_totals.push_back(catfaceSum);
    }

    return catface_totals;
}

void showRoundSummary(TUI& game_window,
    std::vector<std::string>& plr_names_order,
    std::vector<std::vector<Card>>& CURRENT_COLLECTS,
    int roundNo, std::string lead_plr_name) {

    game_window.flush();
    game_window.appendEmptyBox();
    game_window.appendText(1, 33, "ROUND " + std::to_string(roundNo) + " SUMMARY");

    // --- Build the table as a single string, then split into lines ---
    std::ostringstream oss;
    oss << std::left;

    const std::string separator = "+" + std::string(8, '-') + "+"
                                      + std::string(15, '-') + "+";

    oss << separator << "\n"
        << std::setw(9)  << "|PLAYER"
        << std::setw(16) << "|CATFACE" << "|\n"
        << separator << "\n";

    for (size_t i = 0; i < CURRENT_COLLECTS.size(); ++i) {
        int catfaceSum = 0;
        for (const auto& card : CURRENT_COLLECTS[i])
            catfaceSum += card.cat_faces;

        oss << '|' << std::setw(8)  << plr_names_order[i]
            << '|' << std::setw(15) << catfaceSum
            << "|\n"
            << separator << "\n";
    }

    std::istringstream iss(oss.str()); // Turn the string into input stream
    std::string line;
    int row = 4;
    while (std::getline(iss, line)) { //  Reads til \n
        game_window.appendText(row++, 5, line);
    }

    game_window.appendText(row + 1, 5, "Leading Player: " + lead_plr_name);
    game_window.appendText(19, 5, "[0-9] Input any number to continue...");
    game_window.draw();
    int dummy_input = integerInput();
}

void showTurnOrder(TUI& game_window, std::vector<std::string>plr_names_order, std::vector<Card>CHOSEN_CARDS){
    game_window.flush();
    game_window.appendEmptyBox();
    int idx = 0;
    for (auto& name : plr_names_order){
        std::string st = "Turn "+ std::to_string(idx+1) + ": " + name
        + ", Pick: " + std::to_string(CHOSEN_CARDS[idx].value) + "("+ std::to_string(CHOSEN_CARDS[idx].cat_faces)+")";
        game_window.appendText(6 + idx*2, 10, st);
        idx++;
    }
    game_window.appendText(1, 33,"TURN ORDER");
    game_window.appendText(19, 10, "[0-9] Input any number to continue...");
    game_window.draw();
}

void gameEndAlert(TUI& game_window) { // Simple func for showing player that the game has ended
    game_window.flush();
    game_window.appendEmptyBox();
    game_window.appendText(1, 35, "GAME END");
    game_window.appendText(9,  29, " _____   _   __  ____  ");
    game_window.appendText(10, 29, "| ____| | \\ | | |  _ \\ ");
    game_window.appendText(11, 29, "| |__   |  \\| | | | | |");
    game_window.appendText(12, 29, "|  __|  |     | | | | |");
    game_window.appendText(13, 29, "| |___  | |\\  | | |_| |");
    game_window.appendText(14, 29, "|_____| |_| \\_| |____/ ");
    game_window.appendText(19, 10, "[0-9] Input any number to continue.");
    game_window.draw();
    
    int dummy_input = integerInput();
}

std::vector<int> leading_catface_totals;

std::string genSummaryTable(std::vector<std::string> leading_plr_list){

    std::ostringstream oss;
    oss << std::left; //set alignment
    const std::string separator = "+" + std::string(8, '-') + "+" + std::string(15,'-')
    + "+" + std::string(8, '-') + "+";

    // Header
    oss << separator << "\n";
    oss << std::setw(9) << "|ROUND"
        << std::setw(16) << "|LEAD"
        << std::setw(9) << "|CATFACE" << "|\n";
    oss << separator << "\n";

    for (size_t i=0;i<leading_plr_list.size();i++){
        oss << std::setw(9) << "|" + std::to_string(i+1)
            << std::setw(16) << "|" + leading_plr_list[i]
            << std::setw(9) << "|" + std::to_string(leading_catface_totals[i]) << "|\n";
    }
    oss << separator << "\n";

    oss << "WINNER: " + leading_plr_list[leading_plr_list.size()-1] + "\n";
    
    return oss.str();
}

void writeSummaryToFile(std::vector<std::string> leading_plr_list){

    std::string summary = genSummaryTable(leading_plr_list);

    std::ofstream file("summary.txt");
    if (file.is_open()){
        file << summary;
        file.close();
    }
}


void showGameSummary(TUI& game_window, std::vector<std::string> leading_plr_list, int roundNum){
    // TUI function for printing the game summary display which shows leading players and the winner

    // roundNum here should be a multiple of ten
    game_window.flush();
    game_window.appendEmptyBox();
    game_window.appendText(1, 35, "GAME SUMMARY");

    std::string game_summary = genSummaryTable(leading_plr_list);

    size_t i = 0;
    int line_cnt = 0;
    std::string curr_line;
    while (i < game_summary.size()) {
        if (game_summary[i] == '\n'){
            game_window.appendText(4 + line_cnt, 5 , curr_line);
            line_cnt++;
            curr_line = "";
        } else {
            curr_line.push_back(game_summary[i]);
        }
        i++;
    }
   
    game_window.appendText(19, 5, "[0-9] Input any number to continue. (Save summary.txt)");
    game_window.draw();
    int dummy_input = integerInput();
}

#define MAX_ROUNDS 100
#define MAX_CARDS_PER_ROW 5
#define MAX_CATFACES 66

void endOf10Turns(std::vector<std::vector<Card>> CURRENT_COLLECTS, std::vector<std::string> plr_names_order,
TUI& game_window,std::vector<std::string>& leading_plr_list, int roundNum){

    // ROUND IS COMPLETED HERE: Calculate Catfaces
    // Check if game ends (> MAX_CATFACES) 
    // Track leading player (least cf)

    // If end, display end screen with:
    // 1. Leading players list (global var)
    // 2. Winner name
    // then write to file too
    std::vector<int> catface_totals;
    catface_totals = getCatfaceTotals(CURRENT_COLLECTS); // Sorted according to CURRENT_COLLECTS

    int minCF = catface_totals[0];
    std::string lead_plr_name = plr_names_order[0];
    for (int i = 1; i < catface_totals.size(); i++){
        if (catface_totals[i] < minCF) {
            minCF = catface_totals[i];
            lead_plr_name = plr_names_order[i];
        }
    }
    leading_catface_totals.push_back(minCF);
    leading_plr_list.push_back(lead_plr_name);

    // After catfaces have been calculated, and the game HASN'T ENDED...
    // 1. Show Collect piles of each player on one screen
    // 2. Show sum of catfaces
    // 3. Show Leading Player
    // Basically a summary screen for each round end
    showRoundSummary(game_window, plr_names_order, CURRENT_COLLECTS, roundNum, lead_plr_name);
    
    for (auto& cf : catface_totals) {
        if (cf >= MAX_CATFACES || roundNum == MAX_ROUNDS) { //  Game ends
            gameEndAlert(game_window);
            showGameSummary(game_window, leading_plr_list, roundNum);
            writeSummaryToFile(leading_plr_list);
            exit(0); // Terminate the whole program
        }
    }
}


// CURRENT_COLLECTS and CURRENT_HAND needs to persist, so return sorted player names instead
std::vector<std::string> makeRound(Board& board, TUI& game_window, int roundNum, 
    const std::vector<std::string> plr_names,
    std::vector<std::vector<Card>>& CURRENT_HANDS,
    std::vector<std::vector<Card>>& CURRENT_COLLECTS,
    std::vector<std::string>& leading_plr_list ){ // Takes care of everything in a round
    // OK 1: Print the board 
    // OK 2: For each player, print their hand, ask for input (check if Computer, rng if need)
    // OK 3: Determine turn order (sort the cards AND the players arrays in same func.)
    // OK 4: Place the cards (find correct placement), remove from hand, ask for input if needed, clear row if need (check if is Computer, then rng it if so)
    // OK 5: Calculate catfaces
    
    int idx = 0;
    std::vector<std::string> plr_names_order = plr_names;
    std::vector<Card> CHOSEN_CARDS;
    for (auto& plrName : plr_names){ // all players
        int plrInput;
        if(plrName == "Computer"){
            plrInput = rand() % CURRENT_HANDS[idx].size() + 1;

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
        Card card_copy = CURRENT_HANDS[idx][plrInput-1];
        CHOSEN_CARDS.push_back(card_copy); // Add Corresponding card
        deleteCardFromVec(CURRENT_HANDS[idx], card_copy);
        // Remove card from hand, now card is in CHOSEN_CARDS, which is a temporary Vector
        idx++;
    }
    sortCards_Names_Collects(CHOSEN_CARDS, plr_names_order, CURRENT_COLLECTS);
    // Sort the plr_names_order & Chosen cards & Collects order in ASCENDING

    showTurnOrder(game_window, plr_names_order, CHOSEN_CARDS);
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
        game_window.appendText(19, 10, "Turn "+std::to_string(idx2+1)+" Results. Input any number to continue...");
        game_window.draw();
        dummy_input = integerInput();

        idx2++;
    }


    return plr_names_order; //  POTENTIAL PROBLEM: CURRENT_COLLECTS is SORTED, but plr_names ISNT
                            //  ANOTHER PROBLEM: CURRENT_HANDS is SORTED + needs to persist too!
                            //  SOLUTION: Just return plr_names sorted, as HANDS and COLLECTS are sorted already
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
        std::vector<std::string> leading_plr_list; // Originally empty

        // ALL PRE-GAME SETUP SHOULD BE COMPLETED BY THIS LINE!!!

        // Start the 10-turned game
        for (int roundNum = 1;roundNum<= MAX_ROUNDS; roundNum++){
           // makeRound returns plrNames, keeping track of plr_names according to the already sorted CURRENT_HANDS, CURRENT_COLLECTS to ensure consistency
           // as the two are modified via PASS BY REFERENCE already
            plr_names = makeRound(board, game_window, roundNum, plr_names, CURRENT_HANDS, CURRENT_COLLECTS, leading_plr_list);
            if (roundNum%10 == 0){
                initHands(CURRENT_HANDS, card_deck);
                endOf10Turns(CURRENT_COLLECTS, plr_names, game_window, leading_plr_list, roundNum);
            }
        }
    }     
    return 0;
}
