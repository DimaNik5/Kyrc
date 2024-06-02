#pragma once
#include<iostream>
#include<vector>
#include<string>

#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77
#define STYLE_SIZE 3
#define START -1

class Table
{
    public:
    int getColumn();
    int getRow();
    int getMlines();
    int getCursor();
    Table(int, int, int, std::wstring);
    void rename(std::wstring, int k = -1);
    void recontent(std::vector<std::wstring>, int k = -1);
    void regrid(std::wstring);
    std::wstring getGrid();
    void clearContent();
    void setStyleText(int, int);
    void setStyleGraf(int, int);
    void setStyleArrow(int, int);
    void getStyleText(int*, int*);
    void getStyleGraf(int*, int*);
    void getStyleArrow(int*, int*);
    
    int moveCursor(int, int);
    void printTable(int);


    private:
    int column, row, mlines, mchars;
    std::wstring grid;
    std::vector<std::wstring> name;
    std::vector<std::vector<std::wstring>> content;
    int style[STYLE_SIZE][2];

    struct Cursor {
        int column, row, line;
        std::wstring arrow;
    }cursor;

    void printEl(wchar_t, int n = 1);
    void printCenter(std::wstring, int k = 0);
    const wchar_t* colorize(int*);
    void nulStyle();
};