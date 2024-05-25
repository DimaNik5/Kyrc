#include "Table.h"

#define PRINTTOP(X) printEl(grid[2]);for(int z = 1; z < X; z++){printEl(grid[0], mchars + 6);printEl(grid[6]);}printEl(grid[0], mchars + 6);printEl(grid[4]);std::wcout<<std::endl;
#define PRINTMIDLE(X) printEl(grid[8]);for(int z = 1; z < X; z++){printEl(grid[0], mchars + 6);printEl(grid[10]);}printEl(grid[0], mchars + 6);printEl(grid[9]);std::wcout<<std::endl;
#define PRINTBOTTOM(X) printEl(grid[3]);for(int z = 1; z < X; z++){printEl(grid[0], mchars + 6);printEl(grid[7]);}printEl(grid[0], mchars + 6);printEl(grid[5]);
#define FOR_I(X) for(int i = 0; i < X; i++)
#define FOR_J(X) for(int j = 0; j < X; j++)
#define FOR_K(X) for(int k = 0; k < X; k++)
#define PAINT_TEXT std::wcout << colorize(style[0]);
#define PAINT_GRAF std::wcout << colorize(style[1]);
#define PAINT_ARROW(X) std::wcout << colorize(style[2]) << cursor.arrow[X];


#ifdef _WIN32

#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#define DISABLE_NEWLINE_AUTO_RETURN  0x0008

void Table::activateVirtualTerminal()
{
	HANDLE handleOut = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD consoleMode;
	GetConsoleMode(handleOut, &consoleMode);
	consoleMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	consoleMode |= DISABLE_NEWLINE_AUTO_RETURN;
	SetConsoleMode(handleOut, consoleMode);
}
#endif
/*
enum Table::COLORS {
	NC = -1,
	BLACK,
	RED,
	GREEN,
	YELLOW,
	BLUE,
	MAGENTA,
	CYAN,
	WHITE,
	GREY = 60,
	BRIGHT_RED,
	BRIGHT_GREEN,
	BRIGHT_YELLOW,
	BRIGHT_BLUE,
	BRIGHT_MAGENTA,
	BRIGHT_CYAN,
	BRIGHT_WHITE
};
*/
const wchar_t* Table::colorize(int *styl = nullptr) {
	static wchar_t code[20];
    int font, back;
    if (styl)
    {
        font = styl[0];
        back = styl[1];
    }
    else
    {
        font = 0;
        back = 0;
    }
	if (font >= 0)
		font += 30;
	else
		font = 0;
	if (back >= 0)
		back += 40;
	else
		back = 0;
	if (back > 0) {
		swprintf(code,20, L"\033[%d;%dm", font, back);
	}
	else {
		swprintf(code,20, L"\033[%dm", font);
	}
	return code;
}

void Table::nulStyle()
{
	for (int i = 0; i < STYLE_SIZE; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			if (i == 0 && j == 1)
			{
				style[i][j] = 7;
			}
			style[i][j] = -1;
		}
	}
}

void Table::setStyleText(int f, int b)
{
    if(f != -2) style[0][0] = f;
    if(b != -2) style[0][1] = b;
}
void Table::setStyleGraf(int f, int b)
{
    if(f != -2) style[1][0] = f;
    if(b != -2) style[1][1] = b;
}
void Table::setStyleArrow(int f, int b)
{
    if(f != -2) style[2][0] = f;
    if(b != -2) style[2][1] = b;
}

void Table::getStyleText(int* a, int* b)
{
    *a = style[0][0];
    *b = style[0][1];
}
void Table::getStyleGraf(int* a, int* b)
{
    *a = style[1][0];
    *b = style[1][1];
}
void Table::getStyleArrow(int* a, int* b)
{
    *a = style[2][0];
    *b = style[2][1];
}

Table::Table(int c, int r, int s, std::wstring a): column(c), row(r), mlines(s)
{
    cursor.column = 0;
    cursor.row = 0;
    cursor.line = 0;
    cursor.arrow = a;
    mchars = 15;
    grid = L"-|+++++++++"; // L"─│┌└┐┘┬┴├┤┼" {196, 179, 218, 192, 191, 217, 194, 193, 195, 180, 197, L'\0'}
    nulStyle();
}

void Table::rename(std::wstring n, int k)
{
    if(k<0)
    {
        if(column > name.size())
        {
            name.push_back(n);
        }
    }
    else
    {
        if(name.size() > k)
        {
            name[k] = n;
        }
    }
}

void Table::recontent(std::vector<std::wstring> n, int k)
{
    if(k<0)
    {
        if(row * column + column > content.size())
        {
            content.push_back(n);
        }
    }
    else
    {
        if(content.size() > k)
        {
            content[k] = n;
        }
    }
}

void Table::regrid(std::wstring g)
{
    grid = g;
}

std::wstring Table::getGrid()
{
    return grid;
}

void Table::clearContent()
{
    name.clear();
    content.clear();
}

int Table::getColumn()
{
    return column;
}

int Table::getRow()
{
    return row;
}

int Table::getMlines()
{
    return mlines;
}

int Table::getCursor()
{
    return cursor.column * mlines + cursor.row * column * mlines + cursor.line;
}

void Table::moveCursor(int code, int upl)
{
    switch(code){
        case UP:
        if(cursor.line > 0)
        {
            cursor.line--;
        }
        else if(cursor.row > 0)
        {
            cursor.row--;
            cursor.line = 0;
        }
        break;
        case DOWN:
        if(cursor.line < mlines - 1 && cursor.line < content[cursor.row * column + cursor.column].size() - upl - 1)
        {
            cursor.line++;
        }
        else if(cursor.row < row - 1)
        {
            cursor.row++;
            cursor.line = 0;
        }
        break;
        case LEFT:
        if(cursor.column > 0)
        {
            cursor.column--;
            cursor.line = 0;
        }
        break;
        case RIGHT:
        if(cursor.column < column - 1)
        {
            cursor.column++;
            cursor.line = 0;
        }
        break;
        case START:
            cursor.line = 0;
            break;
    }
}

void Table::printTable(int upl)
{
    PAINT_GRAF
    PRINTTOP(column)
    printEl(grid[1]);
    FOR_I(name.size())
    {
        printCenter(name[i]);
    }
    std::wcout << std::endl;
    FOR_I(row)
    {
        PAINT_GRAF
        PRINTMIDLE(column)
        FOR_J(mlines)
        {
            PAINT_GRAF
            printEl(grid[1]);
            FOR_K(column)
            {
                if(j >= content[i * column + k].size() - upl * k)
                {
                    printCenter(L"");
                }
                else
                {
                    printCenter(content[i * column + k][j + upl*k], i * column * mlines + k * mlines + j  == getCursor());
                }
            }
            std::wcout << std::endl;
        }
    }
    PAINT_GRAF
    PRINTBOTTOM(column)
    colorize();
}

void Table::printCenter(std::wstring s, int k)
{
    if(s.size() > mchars)
    {
        s.resize(mchars-2);
        s.push_back(L'.');
        s.push_back(L'.');
    }
    if(k)
    {
        PAINT_TEXT
        printEl(32, (mchars - s.size())/2 + 1);
        PAINT_ARROW(0)
        PAINT_TEXT
        std::wcout << " " << s << " ";
        PAINT_ARROW(1)
        PAINT_TEXT
        printEl(32, (mchars - s.size())/2 + 1 + (mchars - s.size())%2);
    }
    else
    {
        PAINT_TEXT
        printEl(32, (mchars - s.size())/2 + 3);
        std::wcout << s;
        printEl(32, (mchars - s.size())/2 + 3 + (mchars - s.size())%2);
    }
    PAINT_GRAF
    printEl(grid[1]);
}

void Table::printEl(wchar_t s, int n)
{
    for(int i = 0; i < n; i++)
    {
        std::wcout << s;
    }
}
