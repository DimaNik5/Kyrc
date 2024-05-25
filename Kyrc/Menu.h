#pragma once

#include <iostream>
#include <string>
#include <vector>


class Menu
{
    public:
    Menu(std::wstring, Menu* = nullptr);
    ~Menu();
    void setNote();
    static int getNote();
    void setAfter(Menu*);
    void setBefor(Menu*);
    Menu* getAfter();
    Menu* getBefor();
    void setContent(std::wstring, Menu*);
    void setContent(std::wstring, Menu* (*)(Menu*));
    void setContent(Menu*, int);
    void setContent(Menu* (*)(Menu*), int);
    void delContent(int);
    std::vector<std::wstring> getContent();
    int getSizeContent();
    Menu* useContent(int, Menu*);
    std::wstring getName();
    Menu* getParent();
    void setParent(Menu*);

    private:
    std::wstring name;
    typedef struct Enclose
    {
        std::wstring name;
        Menu* (*act)(Menu*) = nullptr;
        Menu* next = nullptr;
        bool operator<(Enclose& other)
        {
            return this->name < other.name;
        }
    };
    std::vector<Enclose> content;
    static int note;
    bool fnote = false;
    Menu* parent;
    Menu *after = nullptr, *befor = nullptr;
};