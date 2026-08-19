#include<iostream>
#include<string>
#include<vector>
#include "include/tui.hpp"
#pragma once

TUI::TUI(int h, int w) : height(h), width(w) // colon list for optimization (just good habit)
{
    buffer.assign(height, std::string(width, ' ')); // included in <vector> / <string>
}

void TUI::flush()
{
    std::cout << "\033[2J\033[H" << std::flush << "\n";
    // \033[2J is an esc sequence for flushing the screen
    // \033[H is for moving cursor to top left
    buffer.assign(height, std::string(width, ' '));
}

void TUI::appendEmptyBox()
{
    TUI::appendText(0, 0, "+" + std::string(width-2, '-') + "+"); 
    
    TUI::appendText(1,0,"|");
    TUI::appendText(1,width - 1,"|");

    TUI::appendText(2,0,"+" + std::string(width-2, '-') + "+");

    for (int i = 3; i < height - 1; i++)
    {
        TUI::appendText(i, 0, "|");
        TUI::appendText(i, width - 1, "|");
    }

    TUI::appendText(height-1, 0, "+" + std::string(width-2, '-') + "+");
}

void TUI::appendText(int row, int start, std::string text)
{
    if (row < 0 || row >= height || start < 0) return; // dont write past edges

   /* if (start + text.length() > width) 
    {
        omitted checking for if text exceeds horizontal limit (should be trimmed if so)
    } */

    buffer[row].replace(start, text.length(), text);
}

void TUI::draw()
{
    std::cout << "\033[H";
    
    for (int i = 0; i < height; i++)
    {
        std::cout << buffer[i] << "\n";
    }
}
