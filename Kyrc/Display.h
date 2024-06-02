#pragma once

#include <vector>
#include <string>
#include <conio.h>
#include "Table.h"
#include "Menu.h"
#include "SchoolMenu.h"

class Display
{
    public:
    Display(const std::wstring, int, int, int);
    void run();

    private:
    std::wstring way;
    int col, row, mline;
    Menu* cur;
    Table* table;
    void print();
    void upload();
    int read();
};