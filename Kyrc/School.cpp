#include "Display.h"
#include <io.h>          // для функции _setmode
#include <fcntl.h>       // для константы _O_U16TEXT

int main()
{
	_setmode(_fileno(stdout), _O_U16TEXT);
	_setmode(_fileno(stdin), _O_U16TEXT);
	Display s{ L"set/grid.txt", 2, 1, 8};
	s.run();
	return 0;
}

