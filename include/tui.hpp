#pragma once
#include <vector>
#include <string>

class TUI
{
    public:
        TUI(int h, int w);

        void flush();
        void appendEmptyBox();
        void appendText(int row, int start, std::string text);
        void draw(); // for less flicker, we put everything into buffer, and draw at last
    private:
        int height, width;
        std::vector<std::string> buffer; // a vector of strings, each string is 1 row
};
