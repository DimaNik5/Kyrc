#include "Menu.h"

int Menu::note;

Menu::Menu(std::wstring name, Menu* pa): name(name), parent(pa) {}

Menu::~Menu()
{
    content.clear();
}

void Menu::setAfter(Menu* a)
{
    after = a;
}

void Menu::setBefor(Menu* b)
{
    befor = b;
}

Menu* Menu::getAfter()
{
    return this->after;
}

Menu* Menu::getBefor()
{
    return befor ? befor : nullptr;
}

void Menu::setContent(std::wstring name, Menu* move)
{
    Enclose temp;
    temp.name = name;
    temp.next = move;
    content.push_back(temp);
}

void Menu::setContent(std::wstring name, Menu* (*f)(Menu*) )
{
    Enclose temp;
    temp.name = name;
    temp.act = f;
    content.push_back(temp);
}

void Menu::setContent(Menu* m, int i)
{
    content[i].next = m;
}

void Menu::setContent(Menu* (*f)(Menu*), int i)
{
    content[i].act = f;
}

void Menu::delContent(int line)
{
    delete content[line].next;
    content.erase(content.begin() + line);
}

std::vector<std::wstring> Menu::getContent()
{
    std::vector<std::wstring> temp;
    for(int i = 0; i < content.size(); i++)
    {
        temp.push_back(content[i].name);
    }
    return temp;
}

void Menu::setNote()
{
    fnote = true;
}
int Menu::getNote()
{
    return note;
}

Menu* Menu::useContent(int l, Menu* cur)
{
    if (l >= content.size()) return nullptr;
    if(content[l].next != nullptr)
    {
        if (fnote) note = l;
        return content[l].next;
    }
    if(content[l].act != nullptr)
    {
        return content[l].act(cur);
    }
    return nullptr;
}

std::wstring Menu::getName()
{
    return name;
}

int Menu::getSizeContent()
{
    return content.size();
}

Menu* Menu::getParent()
{
    return parent;
}

void Menu::setParent(Menu* p)
{
    parent = p;
}