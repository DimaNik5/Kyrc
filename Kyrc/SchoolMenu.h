#pragma once

#include "Menu.h"
#include "Table.h"
#include <vector>
#include <string>
#include <codecvt>
#include <fstream>
#include <conio.h>
#include <Windows.h>


class SchoolMenu
{
public:
    static Menu* getMenu(int, Table*, std::wstring);

    static void update(std::vector<Menu*> menus);

    static int getUpLine();

    static void setUpLine(int);

private:

    typedef struct Schedule {
        std::wstring day;
        std::vector<std::wstring> lessons;
        Schedule* next;
        Schedule* last;
        friend std::wostream& operator<<(std::wostream& os, const Schedule& sh)
        {
            os << sh.day;
            for (int i = 0; i < sh.lessons.size(); i++)
            {
                os << L" " << sh.lessons[i];
            }
            os << L" |" << std::endl;
            return os;
        }
        friend std::wistream& operator>>(std::wistream& is, Schedule& sh)
        {
            std::wstring t;
            is >> sh.day >> t;
            while (t != L"|" && t != L"")
            {
                sh.lessons.push_back(t);
                is >> t;
            }
            return is;
        }
    };

    static std::locale utf8_locale;
    static int mline;
    static int curUpLine;
    static int person;
    static Table* curTable;
    static std::vector<Schedule*> schedule;
    static std::vector<std::wstring> lessons;

    static Menu* up(Menu* menus);

    static Menu* down(Menu* menus);

    static Menu* back(Menu* cur);

    static Menu* exitp(Menu* cur);

    static Menu* input(Menu* cur);

    static Menu* help(Menu* cur);

    static Menu* color(Menu* cur);

    static Menu* save(Menu* cur);

    static Menu* exita(Menu* cur);

    static Menu* grid(Menu* cur);

    static Menu* showSchedule(Menu* cur);

    static Menu* clearSchedule(Menu* cur); 

    static Menu* addSchedule(Menu* cur); 

    static Menu* saveSchedule(Menu* cur);

    static Menu* openFolder(Menu* cur);

    static Menu* openFile(Menu* cur);

    static Menu* addFile(Menu* cur);

    static Menu* delFile(Menu* cur);

    static Menu* delF(Menu* cur);

    static void addComand(Menu* m, Menu* m2);

    static void addMenu(Menu* m, int i);

    static Menu* createMenu(int i, Menu* p = nullptr);

    static void setInfo(std::vector<Schedule*>*);

    static void getInfo(std::vector<Schedule*>*);

    static void printSchedule(int);

    static void addWay(std::wstring*, Menu*, int);

    static void createWay(std::wstring*, Menu*, int k = 0);

    static int getLine();

    static bool getWorkF(Menu*);
};