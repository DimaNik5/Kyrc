#include "SchoolMenu.h"

std::locale SchoolMenu::utf8_locale;
int SchoolMenu::curUpLine;
int SchoolMenu::mline;
int SchoolMenu::person;
Table* SchoolMenu::curTable;
std::vector<SchoolMenu::Schedule*> SchoolMenu::schedule = *(new std::vector<SchoolMenu::Schedule*>);
std::vector<std::wstring> SchoolMenu::lessons = {L"�������_����", L"����������", L"���������", L"��������", L"����������", L"������", L"���", L"�������", L"��������", L"�����", L"�����������" , L"�����������_����"};

Menu* SchoolMenu::getMenu(int l, Table* table, std::wstring w)
{
    mline = l;
    curTable = table;
    person = 0;
    curUpLine = 0;
    utf8_locale = std::locale(std::locale(), new std::codecvt_utf8<wchar_t>());
    std::wifstream file(w);
    if (file.is_open())
    {
        file.imbue(utf8_locale);
        std::wstring g;
        file >> g;
        curTable->regrid(g);
    }
    file.close();
    std::wifstream file1(L"set/graf.txt");
    if (file1.is_open())
    {
        file1.imbue(utf8_locale);
        int i, j;
        file1 >> i >> j;
        curTable->setStyleText(i, j);
        file1 >> i >> j;
        curTable->setStyleGraf(i, j);
        file1 >> i >> j;
        curTable->setStyleArrow(i, j);
    }
    file1.close();
    getInfo(&schedule);
    return createMenu(1);
}

int SchoolMenu::getUpLine()
{
    return curUpLine;
}

void SchoolMenu::setUpLine(int l)
{
    curUpLine = l;
}

void SchoolMenu::update(std::vector<Menu*> menus)
{
    curTable->clearContent();
    for (int i = 0; i < menus.size(); i++)
    {
        curTable->rename(menus[i]->getName());
        curTable->recontent(menus[i]->getContent());
    }
    curTable->moveCursor(-1, curUpLine);
}

Menu* SchoolMenu::down(Menu* menus)
{
    if (curUpLine >= 0 && menus->getAfter()->getSizeContent() > curUpLine + mline)
    {
        curUpLine += mline;
    }
    return menus;
}

Menu* SchoolMenu::up(Menu* menus)
{
    if (curUpLine >= mline)
    {
        curUpLine -= mline;
    }
    return menus;
}

Menu* SchoolMenu::back(Menu* cur)
{
    cur = cur->getParent();
    if (cur->getBefor())
    {
        cur = cur->getBefor();
    }
    return cur;
}

Menu* SchoolMenu::exitp(Menu* cur)
{
    exit(0);
}

Menu* SchoolMenu::input(Menu* cur)
{
    system("cls");
    std::wcout << L"������� �����: ";
    std::wstring t, f;
    int p;
    std::wcin >> t;
    std::wifstream file(L"data/log.txt");
    if (!file.is_open())
    {
        return nullptr;
    }
    file.imbue(utf8_locale);
    file >> f;
    while (!file.eof())
    {
        file >> p;
        if (f == t)
        {
            if (p > 0 && p < 34)
            {
                person = p - 1;
                cur->setContent(createMenu(8, cur), 0);
                cur = cur->useContent(0, cur);
                file.close();
                return cur;
            }
            else if (p > 100 && p < 113)
            {
                person = p;
                cur->setContent(createMenu(9, cur), 0);
                cur = cur->useContent(0, cur);
                file.close();
                return cur;
            }
            else if (p == -1)
            {
                person = p;
                cur->setContent(createMenu(10, cur), 0);
                cur = cur->useContent(0, cur);
                file.close();
                return cur;
            }
        }
        file >> f;
    }
    file.close();
    return nullptr;
}

Menu* SchoolMenu::help(Menu* cur)
{
    _wsystem(L"start help.txt");
    return nullptr;
}

Menu* SchoolMenu::color(Menu* cur)
{
    int line = getLine();
    line += line > 7 ? 52 : 0;
    if (cur->getName() == L"�����")
    {
        curTable->setStyleText(line, -2);
    }
    else if (cur->getName() == L"�������")
    {
        curTable->setStyleGraf(line, -2);
    }
    else if (cur->getName() == L"�������")
    {
        curTable->setStyleArrow(line, -2);
    }
    else if (cur->getName() == L"���")
    {
        curTable->setStyleText(-2, line);
        curTable->setStyleGraf(-2, line);
        curTable->setStyleArrow(-2, line);
    }
    return nullptr;
}

Menu* SchoolMenu::save(Menu* cur)
{
    if (cur->getAfter()->getName() == L"�������")
    {
        std::wofstream file(L"set/grid.txt");
        if (file.is_open())
        {
            file.imbue(utf8_locale);
            file << curTable->getGrid();
        }
        file.close();
    }
    else
    {
        std::wofstream file(L"set/graf.txt");
        if (file.is_open())
        {
            file.imbue(utf8_locale);
            int i, j;
            curTable->getStyleText(&i, &j);
            file << i << L" " << j << std::endl;
            curTable->getStyleGraf(&i, &j);
            file << i << L" " << j << std::endl;
            curTable->getStyleArrow(&i, &j);
            file << i << L" " << j << std::endl;
        }
        file.close();
    }
    return nullptr;
}

Menu* SchoolMenu::exita(Menu* cur)
{
    person = 0;
    cur = back(cur);
    cur->getAfter()->setContent((Menu*)nullptr, 0);
    return cur;
}

Menu* SchoolMenu::grid(Menu* cur)
{
    std::wstring w;
    switch (getLine())
    {
    case 0:
        w = L"set/grid1.txt";
        break;
    case 1:
        w = L"set/grid2.txt";
        break;
    }
    std::wifstream file(w);
    if (file.is_open())
    {
        file.imbue(utf8_locale);
        std::wstring g;
        file >> g;
        curTable->regrid(g);
    }
    file.close();
    return nullptr;
}

Menu* SchoolMenu::showSchedule(Menu* cur)
{
    int i;
    if (person >= 0 && person < 34)
    {
        i = person;
    }
    else
    {
        i = getLine();
    }
    printSchedule(i);
    return nullptr;
}

Menu* SchoolMenu::clearSchedule(Menu* cur)
{
    Schedule* temp = schedule[Menu::getNote()];
    while (temp->day != cur->getName())
    {
        temp = temp->next;
    }
    temp->lessons.clear();
    temp->lessons.push_back(L".");
    return nullptr;
}

Menu* SchoolMenu::addSchedule(Menu* cur)
{
    Schedule* temp = schedule[Menu::getNote()];
    while (temp->day != cur->getParent()->getName())
    {
        temp = temp->next;
    }
    if(temp->lessons.size() < 9)
    {
        if (temp->lessons[0] == L".") temp->lessons.clear();
        temp->lessons.push_back(lessons[getLine()]);
    }
    return nullptr;
}

Menu* SchoolMenu::saveSchedule(Menu* cur)
{
    setInfo(&schedule);
    return nullptr;
}

Menu* SchoolMenu::openFolder(Menu* cur)
{
    std::wstring way;
    createWay(&way, cur, -person / 100);
    Menu* cfolder = new Menu(L"�������");
    Menu* folder = new Menu(cur->getContent()[getLine()]);
    WIN32_FIND_DATA FindFileData;
    HANDLE hf;
    hf = FindFirstFile(way.c_str(), &FindFileData);
    if (hf != INVALID_HANDLE_VALUE) {
        way.pop_back();
        int i = 0;
        do {
            if (i++ < 2) continue;
            std::wstring tw = way + FindFileData.cFileName;
            DWORD dwAttr = GetFileAttributes(tw.c_str());
            if (dwAttr & FILE_ATTRIBUTE_DIRECTORY)
            {
                folder->setContent(FindFileData.cFileName, &openFolder);
            }
            else
            {
                folder->setContent(FindFileData.cFileName, &openFile);
            }
        } while (FindNextFile(hf, &FindFileData) != 0);
        FindClose(hf);
        cfolder->setAfter(folder);
        folder->setBefor(cfolder);
        cfolder->setParent(cur);
        folder->setParent(cur);
        addComand(cfolder, folder);
        cur->setContent(cfolder, getLine());
        cur = cur->useContent(getLine(), cur);
        return cur;
    }
    return nullptr;
}

Menu* SchoolMenu::openFile(Menu* cur)
{
    std::wstring way;
    createWay(&way, cur, person > 100 ? -100 : 100);
    _wsystem((L"start \"\" \"" + way + L"\"").c_str());
    return nullptr;
}

Menu* SchoolMenu::addFile(Menu* cur)
{
    system("cls");
    std::wstring w1, w2;
    createWay(&w2, cur->getAfter(), -200);
    std::wcout << L"������� ��������: ";
    std::wcin >> w1;
    WIN32_FIND_DATA FindFileData;
    HANDLE hf;
    hf = FindFirstFile(w1.c_str(), &FindFileData);
    if (hf != INVALID_HANDLE_VALUE)
    {
        while (w1[w1.size() - 1] != L'\\' && w1.find(L'\\') != std::string::npos)
        {
            w1.pop_back();
        }
        do {
            if (FindFileData.cFileName[0] == L'.') continue;
            std::wstring tw = w1 + (w1.find(L'\\') != std::string::npos ? FindFileData.cFileName : L"");
            for (int i = 0; i < cur->getAfter()->getContent().size(); i++)
            {
                if (cur->getAfter()->getContent()[i] == FindFileData.cFileName)
                {
                    cur->getAfter()->delContent(i);
                }
            }
            DWORD dwAttr = GetFileAttributes(tw.c_str());
            if (dwAttr & FILE_ATTRIBUTE_DIRECTORY)
            {
                w2 += FindFileData.cFileName;
                cur->getAfter()->setContent(FindFileData.cFileName, &openFolder);
                _wsystem((L"xcopy \"" + tw + L"\" \"" + w2 + L"\" /i /y /e /q").c_str());
            }
            else
            {
                cur->getAfter()->setContent(FindFileData.cFileName, &openFile);
                _wsystem((L"xcopy \"" + tw + L"\" \"" + w2 + L"\" /i /y /q").c_str());
            }
        } while (FindNextFile(hf, &FindFileData) != 0);
        return cur;
    }
    else
    {
        if(w1.find(L'\\') != std::string::npos || w1.find(L'*') != std::string::npos) return nullptr;
        if (w1.find(L'.') != std::string::npos)
        {
            std::wofstream ofs(w2 + w1);
            ofs.close();
            cur->getAfter()->setContent(w1, &openFile);
        }
        else
        {
            _wsystem((L"mkdir \"" + w2 + w1 + L"\"").c_str());
            cur->getAfter()->setContent(w1, &openFolder);
        }
        return cur;
    }
}

Menu* SchoolMenu::delFile(Menu* cur)
{
    cur->setContent((Menu*) nullptr, getLine());
    Menu* cfolder = new Menu(L"�������");
    Menu* folder = new Menu(L"�������");
    for (int i = 0; i < cur->getAfter()->getSizeContent(); i++)
    {
        folder->setContent(cur->getAfter()->getContent()[i], &delF);
    }
    cfolder->setAfter(folder);
    folder->setBefor(cfolder);
    cfolder->setParent(cur);
    folder->setParent(cur);
    addComand(cfolder, folder);
    cur->setContent(cfolder, getLine());
    cur = cur->useContent(getLine(), cur->getAfter());
    return cur;
}

Menu* SchoolMenu::delF(Menu* cur)
{
    std::wstring w;
    if (cur->getContent()[getLine()].find(L'.') != std::string::npos)
    {
        w = L"del /f \"";
    }
    else
    {
        w = L"rd /s /q \"";
    }
    createWay(&w, cur->getParent()->getAfter(), -100);
    w += L"\"";
    _wsystem(w.c_str());
    cur->getParent()->getAfter()->delContent(getLine());
    cur = cur->getParent();
    return cur;
}

void SchoolMenu::addComand(Menu* m, Menu* m2)
{
    if (m2->getSizeContent() > mline)
    {
        m->setContent(L"������", &up);
        m->setContent(L"����", &down);
    }
   
    if (m->getParent() != nullptr)
    {
        if (m->getParent()->getName() == L"����" || m->getAfter()->getName() == L"�������")
        {
            m->setContent(L"���������", &save);
        }
        else if (m->getParent()->getName() == L"���� ������")
        {
            m->setContent(L"���������", &saveSchedule);
        }
        else if (person > 100 && getWorkF(m2) && m2->getName() != L"�������")
        {
            m->setContent(L"��������", &addFile);
            m->setContent(L"�������", &delFile);
        }
        m->setContent(L"�����", &back);
    }
    m->setContent(L"�����", &exitp);
}

void SchoolMenu::addMenu(Menu* m, int i)
{
    switch (i)
    {
    case 1:
        m->setContent(L"����", &input);
        m->setContent(L"���������", createMenu(2, m));
        m->setContent(L"������", &help);
        break;
    case 2:
        m->setContent(L"����", createMenu(3, m));
        m->setContent(L"�������", createMenu(11, m));
        break;
    case 3:
        m->setContent(L"�����", createMenu(4, m));
        m->setContent(L"�������", createMenu(5, m));
        m->setContent(L"�������", createMenu(6, m));
        m->setContent(L"���", createMenu(7, m));
        break;
    case 4: case 5: case 6: case 7:
        m->setContent(L"������", &color);
        m->setContent(L"�������", &color);
        m->setContent(L"�������", &color);
        m->setContent(L"������", &color);
        m->setContent(L"�����", &color);
        m->setContent(L"���������", &color);
        m->setContent(L"�������", &color);
        m->setContent(L"�����", &color);
        m->setContent(L"�����", &color);
        m->setContent(L"������ �������", &color);
        m->setContent(L"������ �������", &color);
        m->setContent(L"������ ������", &color);
        m->setContent(L"������ �����", &color);
        m->setContent(L"������ ���������", &color);
        m->setContent(L"������ �������", &color);
        m->setContent(L"������ �����", &color);
        break;
    case 8:
        m->setContent(L"�������", createMenu(18, m));
        m->setContent(L"����������", &showSchedule);
        m->setContent(L"�����", &exita);
        break;
    case 9:
        m->setContent(L"�����", createMenu(19, m));
        m->setContent(L"����������", createMenu(12, m));
        m->setContent(L"�����", &exita);
        break;
    case 10:
        m->setContent(L"����������", createMenu(13, m));
        m->setContent(L"�����", &exita);
        break;
    case 11:
        m->setContent(L"��� 1", &grid);
        m->setContent(L"��� 2", &grid);
        break;
    case 12:
        m->setContent(L"1�", &showSchedule);
        m->setContent(L"1�", &showSchedule);
        m->setContent(L"1�", &showSchedule);
        m->setContent(L"2�", &showSchedule);
        m->setContent(L"2�", &showSchedule);
        m->setContent(L"2�", &showSchedule);
        m->setContent(L"3�", &showSchedule);
        m->setContent(L"3�", &showSchedule);
        m->setContent(L"3�", &showSchedule);
        m->setContent(L"4�", &showSchedule);
        m->setContent(L"4�", &showSchedule);
        m->setContent(L"4�", &showSchedule);
        m->setContent(L"5�", &showSchedule);
        m->setContent(L"5�", &showSchedule);
        m->setContent(L"5�", &showSchedule);
        m->setContent(L"6�", &showSchedule);
        m->setContent(L"6�", &showSchedule);
        m->setContent(L"6�", &showSchedule);
        m->setContent(L"7�", &showSchedule);
        m->setContent(L"7�", &showSchedule);
        m->setContent(L"7�", &showSchedule);
        m->setContent(L"8�", &showSchedule);
        m->setContent(L"8�", &showSchedule);
        m->setContent(L"8�", &showSchedule);
        m->setContent(L"9�", &showSchedule);
        m->setContent(L"9�", &showSchedule);
        m->setContent(L"9�", &showSchedule);
        m->setContent(L"10�", &showSchedule);
        m->setContent(L"10�", &showSchedule);
        m->setContent(L"10�", &showSchedule);
        m->setContent(L"11�", &showSchedule);
        m->setContent(L"11�", &showSchedule);
        m->setContent(L"11�", &showSchedule);
        break;
    case 13:
        m->setContent(L"����������", createMenu(12, m));
        m->setContent(L"��������", createMenu(14, m));
        break;
    case 14:
        m->setContent(L"1�", createMenu(17, m));
        m->setContent(L"1�", createMenu(17, m));
        m->setContent(L"1�", createMenu(17, m));
        m->setContent(L"2�", createMenu(17, m));
        m->setContent(L"2�", createMenu(17, m));
        m->setContent(L"2�", createMenu(17, m));
        m->setContent(L"3�", createMenu(17, m));
        m->setContent(L"3�", createMenu(17, m));
        m->setContent(L"3�", createMenu(17, m));
        m->setContent(L"4�", createMenu(17, m));
        m->setContent(L"4�", createMenu(17, m));
        m->setContent(L"4�", createMenu(17, m));
        m->setContent(L"5�", createMenu(17, m));
        m->setContent(L"5�", createMenu(17, m));
        m->setContent(L"5�", createMenu(17, m));
        m->setContent(L"6�", createMenu(17, m));
        m->setContent(L"6�", createMenu(17, m));
        m->setContent(L"6�", createMenu(17, m));
        m->setContent(L"7�", createMenu(17, m));
        m->setContent(L"7�", createMenu(17, m));
        m->setContent(L"7�", createMenu(17, m));
        m->setContent(L"8�", createMenu(17, m));
        m->setContent(L"8�", createMenu(17, m));
        m->setContent(L"8�", createMenu(17, m));
        m->setContent(L"9�", createMenu(17, m));
        m->setContent(L"9�", createMenu(17, m));
        m->setContent(L"9�", createMenu(17, m));
        m->setContent(L"10�", createMenu(17, m));
        m->setContent(L"10�", createMenu(17, m));
        m->setContent(L"10�", createMenu(17, m));
        m->setContent(L"11�", createMenu(17, m));
        m->setContent(L"11�", createMenu(17, m));
        m->setContent(L"11�", createMenu(17, m));
        break;
    case 17:
    {
        Schedule* temp = schedule[getLine()];
        for (int j = 0; j < 5; j++)
        {
            m->setContent(temp->day, createMenu(15, m));
            temp = temp->next;
        }
        break;
    }
    case 15:
        m->setContent(L"��������", createMenu(16, m));
        m->setContent(L"�������", &clearSchedule);
        break;
    case 16:
        for(int j = 0; j < lessons.size(); j++)
        {
            m->setContent(lessons[j], &addSchedule);
        }
        m->setContent(L"-", &addSchedule);
        break;
    case 18:
        for (int j = 0; j < lessons.size(); j++)
        {
            m->setContent(lessons[j], &openFolder);
        }
        break;
    case 19:
        m->setContent(L"1�", &openFolder);
        m->setContent(L"1�", &openFolder);
        m->setContent(L"1�", &openFolder);
        m->setContent(L"2�", &openFolder);
        m->setContent(L"2�", &openFolder);
        m->setContent(L"2�", &openFolder);
        m->setContent(L"3�", &openFolder);
        m->setContent(L"3�", &openFolder);
        m->setContent(L"3�", &openFolder);
        m->setContent(L"4�", &openFolder);
        m->setContent(L"4�", &openFolder);
        m->setContent(L"4�", &openFolder);
        m->setContent(L"5�", &openFolder);
        m->setContent(L"5�", &openFolder);
        m->setContent(L"5�", &openFolder);
        m->setContent(L"6�", &openFolder);
        m->setContent(L"6�", &openFolder);
        m->setContent(L"6�", &openFolder);
        m->setContent(L"7�", &openFolder);
        m->setContent(L"7�", &openFolder);
        m->setContent(L"7�", &openFolder);
        m->setContent(L"8�", &openFolder);
        m->setContent(L"8�", &openFolder);
        m->setContent(L"8�", &openFolder);
        m->setContent(L"9�", &openFolder);
        m->setContent(L"9�", &openFolder);
        m->setContent(L"9�", &openFolder);
        m->setContent(L"10�", &openFolder);
        m->setContent(L"10�", &openFolder);
        m->setContent(L"10�", &openFolder);
        m->setContent(L"11�", &openFolder);
        m->setContent(L"11�", &openFolder);
        m->setContent(L"11�", &openFolder);
        break;
    }


}

Menu* SchoolMenu::createMenu(int i, Menu* p)
{
    Menu* mM, * cM;
    switch (i)
    {
    case 1:
        mM = new Menu(L"����");
        break;
    case 2:
        mM = new Menu(L"���������");
        break;
    case 3:
        mM = new Menu(L"����");
        break;
    case 4:
        mM = new Menu(L"�����");
        break;
    case 5:
        mM = new Menu(L"�������");
        break;
    case 6:
        mM = new Menu(L"�������");
        break;
    case 7:
        mM = new Menu(L"���");
        break;
    case 8:
        mM = new Menu(L"������");
        break;
    case 9:
        mM = new Menu(L"�������");
        break;
    case 10:
        mM = new Menu(L"�������������");
        break;
    case 11:
        mM = new Menu(L"�������");
        break;
    case 12:
        mM = new Menu(L"����������");
        break;
    case 13:
        mM = new Menu(L"����������");
        break;
    case 14:
        mM = new Menu(L"������");
        break;
    case 17:
        mM = new Menu(L"���� ������");
        break;
    case 15:
    {
        Schedule* temp = schedule[Menu::getNote()];
        for(int j =0; j<p->getSizeContent(); j++)
        {
            temp = temp->next;
        }
        mM = new Menu(temp->day);
        break;
    }
    case 16:
        mM = new Menu(L"�������");
        break;
    case 18:
        mM = new Menu(L"�������");
        mM->setNote();
        break;
    case 19:
        mM = new Menu(L"������");
        mM->setNote();
        break;
    }
    cM = new Menu(L"�������");
    addMenu(mM, i);
    mM->setParent(p);
    cM->setParent(p);
    mM->setBefor(cM);
    cM->setAfter(mM);
    addComand(cM, mM);
    return cM;
}

void SchoolMenu::setInfo(std::vector<Schedule*>* sc)
{
    std::wofstream fout("data/schedule.txt", std::wostream::binary);
    fout.imbue(utf8_locale);
    Schedule* temp;
    for (int i = 0; i < sc->size(); i++)
    {
        temp = (*sc)[i];
        while (true) {
            fout << *temp;
            if (temp->next == nullptr) break;
            temp = temp->next;
        }
    }
    fout.close();
}

void SchoolMenu::getInfo(std::vector<Schedule*>* sc)
{
    Schedule* sch = new Schedule, * lastsh = new Schedule;
    std::wifstream fin("data/schedule.txt", std::wios::in);
    fin.imbue(utf8_locale);
    for (int j = 0; j < 33; j++)
    {
        for (int i = 0; i < 5; i++)
        {
            if (!fin.is_open())
            {
                switch (i) {
                    case 0:
                        sch->day = L"�����������";
                        break;
                    case 1:
                        sch->day = L"�������";
                        break;
                    case 2:
                        sch->day = L"�����";
                        break;
                    case 3:
                        sch->day = L"�������";
                        break;
                    case 4:
                        sch->day = L"�������";
                        break;
                }
                sch->lessons.push_back(L".");
                sch->last = nullptr;
            }
            else fin >> *sch;
            if (i == 0)
            {
                sc->push_back(sch);
                lastsh = sch;
            }
            else {
                lastsh->next = sch;
                sch->last = lastsh;
                sch->next = nullptr;
                lastsh = sch;
            }
            sch = new Schedule;
        }
    }
    fin.close();
}

void SchoolMenu::printSchedule(int k)
{
    system("cls");
    Table* table = new Table(2, 5, mline, L"  ");
    Schedule* temp = schedule[k];
    table->rename(L"���� ������");
    table->rename(L"����������");
    for (int i = 0; i < 5; i++)
    {
        std::vector<std::wstring> t = { temp->day };
        table->recontent(t);
        table->recontent(temp->lessons);
        temp = temp->next;
    }

    table->regrid(curTable->getGrid());
    std::wifstream file1(L"set/graf.txt");
    if (file1.is_open())
    {
        file1.imbue(utf8_locale);
        int i, j;
        file1 >> i >> j;
        table->setStyleText(i, j);
        file1 >> i >> j;
        table->setStyleGraf(i, j);
        file1 >> i >> j;
        table->setStyleArrow(i, j);
    }
    file1.close();
    table->printTable(0);
    std::wcout << std::endl << L"����� ���������, ������� �����";
    while (true)
    {
        if (_kbhit() != 0)
        {
            k = _getch();
            if (k == 13) break;
        }
    }

}

void SchoolMenu::addWay(std::wstring* w, Menu* t, int k)
{
    if (t->getName() != L"�������" && k >= 0)
    {
        addWay(w, t->getParent(), k+1);
        *w += t->getName() + L"\\";
    }
    if (k == 0)
    {
        if (t->getName() != L"������") *w += t->getContent()[getLine()];
        *w += L"\\";
        *w += L'*';
        return;
    }
    if (k == 100)
    {
        *w += t->getContent()[getLine()];
        return;
    }

    if (t->getName() != L"������" && t->getParent()->getName() != L"������" && k < 0)
    {
        addWay(w, t->getParent(), k - 1);
        *w += t->getName() + L"\\";
    }
    if (k == -1)
    {
        if (t->getName() != L"������") *w += t->getContent()[getLine()];
        *w += L"\\";
        *w += L'*';
        return;
    }
    if (k == -100)
    {
        *w += t->getContent()[getLine()];
        return;
    }
    if (k == -200) return;
}

void SchoolMenu::createWay(std::wstring* w, Menu* t, int k)
{
    *w += L"class\\";
    int p;
    if (person >= 0 && person < 34)
    {
        p = person;
    }
    else if (t->getName() == L"������" && k < 0)
    {
        p = getLine();
    }
    else
    {
        p = Menu::getNote();
    }
    switch (p % 3) {
    case 0:
        *w += L"a";
        break;
    case 1:
        *w += L"b";
        break;
    case 2:
        *w += L"v";
        break;
    }
    *w += ((wchar_t)(p / 3 + 49));
    *w += L"\\";
    if (k < 0) {
        *w += lessons[person % 100 - 1];
        *w += L"\\";
    }
    addWay(w, t, k);
}

int SchoolMenu::getLine()
{
    int line = curTable->getCursor();
    do {
        line -= mline;
    } while (line >= 0);
    line += mline + curUpLine;
    return line;
}

bool SchoolMenu::getWorkF(Menu* t)
{
    while (t->getParent() != nullptr)
    {
        t = t->getParent();
         if (t->getName() == L"������")
         {
             return true;
         }
    }
    return false;
}