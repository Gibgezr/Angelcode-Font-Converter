#include <iostream>
#include "Afont.h"

int main(int argc, char **argv)
{
	if(argc != 3)
	{
		std::cout << "USAGE: " << argv[0] << "<textinputfile.fnt> <binaryoutputfile.fnt>" << std::endl;
		return -1;
	}

	AngelcodeFontConverter font;
	font.Read(argv[1]);
	font.Write(argv[2]);
}