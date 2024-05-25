#include "Display.h"

#define DOWN 80
#define DOWN1 115
#define DOWN2 235
#define UP 72 
#define UP1 119
#define UP2 230
#define LEFT 75
#define LEFT1 97
#define LEFT2 228
#define RIGHT 77
#define RIGHT1 100
#define RIGHT2 162
#define ENTER 13

Display::Display(std::wstring w, int c, int r, int ml):way(w), col(c), row(r), mline(ml)
{
    table = new Table(col, row, mline, L"><");
    cur = SchoolMenu::getMenu(mline, table, way);
}

void Display::run()
{
    upload();
    print();
    while(true)
    {
        if (_kbhit() != 0)
        {
            if(read())
            {
                system("cls");
                print();
            }
        }
    }
}

void Display::print()
{
    table->printTable(SchoolMenu::getUpLine());
}

void Display::upload(int i)
{
    if(i > -1)
    {
        SchoolMenu::update(cur->getAfter(),i);
    }
    else
    {
        std::vector<Menu*> t = {cur, cur->getAfter()};
        SchoolMenu::update(t);
    }
}

int Display::read()
{
    wchar_t t;
    t = _getch();
    switch (t)
    {
    case DOWN:
    case DOWN1:
    case DOWN2:
        table->moveCursor(DOWN, SchoolMenu::getUpLine());
        return 1;
    case UP:
    case UP1:
    case UP2:
        table->moveCursor(UP, SchoolMenu::getUpLine());
        return 1;
    case LEFT:
    case LEFT1:
    case LEFT2:
        table->moveCursor(LEFT, SchoolMenu::getUpLine());
        return 1;
    case RIGHT:
    case RIGHT1:
    case RIGHT2:
        table->moveCursor(RIGHT, SchoolMenu::getUpLine());
        return 1;
    case ENTER:
    {
        Menu* temp = cur;
        int c = table->getCursor();
        for(int i = 0; i < row; i++)
        {
            for(int j = 0; j < col; j++)
            {
                if((c - i * col * mline - j * mline) < mline && (c - i * col * mline - j * mline) >= 0)
                {
                    c -= i * col * mline + j * mline;
                    for(int k = 0; k < i*j+j; k++)
                    {
                        temp = temp->getAfter();
                    }
                    break;
                }
            }
        }
        temp = temp->useContent(c, temp);
        if(temp)
        {
            if (temp != cur)
            {
                SchoolMenu::setUpLine(0);
            }
            cur = temp;
        }
        upload();
        return 1;
    }
    }
    return 0;
}