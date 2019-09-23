#include "Afont.h"
#include <iostream>
#include <fstream>
#include <cassert>
#include <sstream> 
#include <algorithm>

AngelcodeFontConverter::~AngelcodeFontConverter()
{

}

AngelcodeFontConverter::AngelcodeFontConverter()
{
	//determine endianness of architecture
	unsigned char word[4] = { (unsigned char)0x01, (unsigned char)0x23, (unsigned char)0x45, (unsigned char)0x67 };

	unsigned long be = 0x01234567;
	unsigned long le = 0x67452301;
	unsigned long me = 0x23016745;
	unsigned long we;

	memcpy(&we, word, 4);
	endian = 0;
	if(we == le) endian = 0;
	else if(we == be) endian = 1;
	else if(we == me) endian = 2;
}

bool AngelcodeFontConverter::Read(std::string fontfile)
{
	std::ifstream Stream;
	Stream.open(fontfile, std::ios::in);
	if(!Stream.is_open())
	{
		std::cout << "ERROR: cannot open " << fontfile << " for input." << std::endl;
		return false;
	}

	std::string Line;
	std::string Read, Key, Value;
	std::size_t i;
	while(!Stream.eof())
	{
		std::stringstream LineStream;
		std::getline(Stream, Line);
		LineStream << Line;

		//read the line's type
		LineStream >> Read;

		if(Read == "char")
		{
			//this is data for a specific char
			AngelcodeCharDescriptor cd;

			while(!LineStream.eof())
			{
				std::stringstream Converter;
				LineStream >> Read;
				i = Read.find('=');
				Key = Read.substr(0, i);
				Value = Read.substr(i + 1);

				//assign the correct value
				Converter << Value;
				if(Key == "id")
				{
					Converter >> cd.id;
				}
				else if(Key == "x")
					Converter >>cd.x;
				else if(Key == "y")
					Converter >> cd.y;
				else if(Key == "width")
					Converter >> cd.width;
				else if(Key == "height")
					Converter >> cd.height;
				else if(Key == "xoffset")
					Converter >> cd.xOffset;
				else if(Key == "yoffset")
					Converter >> cd.yOffset;
				else if(Key == "xadvance")
					Converter >> cd.xAdvance;
				else if(Key == "page")
					Converter >> cd.page;
				else if(Key == "chnl")
					Converter >> cd.chnl;
			}

			Chars.push_back(cd);
		}
		else if(Read == "kerning")
		{
			KerningPair kp;
			//this holds page data
			while(!LineStream.eof())
			{
				std::stringstream Converter;
				LineStream >> Read;
				i = Read.find('=');
				Key = Read.substr(0, i);
				Value = Read.substr(i + 1);


				//assign the correct value
				Converter << Value;
				if(Key == "first")
					Converter >> kp.first;
				else if(Key == "second")
					Converter >> kp.second;
				else if(Key == "amount")
					Converter >> kp.amount;

			}

			kerningPairs.push_back(kp);
		}
		else if(Read == "info")
		{
			//this holds info data
			while(!LineStream.eof())
			{
				std::stringstream Converter;
				LineStream >> Read;
				i = Read.find('=');
				Key = Read.substr(0, i);
				Value = Read.substr(i + 1);

				//assign the correct value
				Converter << Value;
				if(Key == "face")
					Converter >> fontName;
				else if(Key == "size")
					Converter >> info.fontSize;
				else if(Key == "bold")
				{
					bool input;
					uint8_t bit = 1;
					Converter >> input;
					if(input) info.bitField |= (bit << 3);
				}
				else if(Key == "italic")
				{
					bool input;
					uint8_t bit = 1;
					Converter >> input;
					if(input) info.bitField |= (bit << 2);
				}
				else if(Key == "charset")
				{
					//skip for now?
					Converter >> info.charSet;
				}
				else if(Key == "unicode")
				{
					bool input = false;
					uint8_t bit = 1;
					Converter >> input;
					if(input) info.bitField |= (bit << 1);
				}
				else if(Key == "stretchH")
					Converter >> info.stretchH;
				else if(Key == "smooth")
				{
					bool input;
					uint8_t bit = 1;
					Converter >> input;
					if(input) info.bitField |= (bit << 0);
				}
				else if(Key == "aa")
					Converter >> info.aa;
				else if(Key == "padding")
				{
					//break value down into 4 values
					int val;
					std::vector<int> vect;
					
					while(Converter >> val)
					{
						vect.push_back(val);

						if(Converter.peek() == ',' || Converter.peek() == ' ')
							Converter.ignore();
					}
					assert(vect.size() == 4);
					info.paddingUp = vect[0];
					info.paddingRight = vect[1];
					info.paddingDown = vect[2];
					info.paddingLeft = vect[3];
				}
				else if(Key == "spacing")
				{
					//break value down into 2 values
					int val;
					std::vector<int> vect;

					while(Converter >> val)
					{
						vect.push_back(val);

						if(Converter.peek() == ',' || Converter.peek() == ' ')
							Converter.ignore();
					}
					assert(vect.size() == 2);
					info.spacingHoriz = vect[0];
					info.spacingVert = vect[1];
				}
				else if(Key == "outline")
					Converter >> info.outline;				
			}
		}
		else if(Read == "common")
		{
			//this holds common data
			while(!LineStream.eof())
			{
				std::stringstream Converter;
				LineStream >> Read;
				i = Read.find('=');
				Key = Read.substr(0, i);
				Value = Read.substr(i + 1);

				//assign the correct value
				Converter << Value;
				if(Key == "lineHeight")
					Converter >> common.lineHeight;
				else if(Key == "base")
					Converter >> common.base;
				else if(Key == "scaleW")
					Converter >> common.scaleW;
				else if(Key == "scaleH")
					Converter >> common.scaleH;
				else if(Key == "pages")
					Converter >> common.pages;
				else if(Key == "packed")
				{
					bool input;
					uint8_t bit = 1;
					Converter >> input;
					if(input) common.bitField |= (bit << 7);
				}
				else if(Key == "alphaChnl")
					Converter >> common.alphaChnl;
				else if(Key == "redChnl")
					Converter >> common.redChnl;
				else if(Key == "greenChnl")
					Converter >> common.greenChnl;
				else if(Key == "blueChnl")
					Converter >> common.blueChnl;
			}
		}
		else if(Read == "page")
		{
			Page p;
			//this holds page data
			while(!LineStream.eof())
			{
				std::stringstream Converter;
				LineStream >> Read;
				i = Read.find('=');
				Key = Read.substr(0, i);
				Value = Read.substr(i + 1);

				
				//assign the correct value
				Converter << Value;
				if(Key == "id")
					Converter >> p.id;
				else if(Key == "file")
				{
					// Remove all double-quote characters
					std::string tempString;
					Converter >> tempString;
					tempString.erase(
						remove(tempString.begin(), tempString.end(), '\"'),
						tempString.end()
						);

					p.pageName = tempString;
				}
			}

			pages.push_back(p);
		}		
	}

	//sort pages by id, just in case they were stored out of order
	std::sort(pages.begin(), pages.end(), [](Page A, Page B) -> bool{ return (A.id < B.id); });
	return true;
}

bool AngelcodeFontConverter::Write(std::string outfilename)
{
	if(endian != 0)
	{
		std::cout << "This version of AngelcodeFontConverter only support little-endian systems. Sorry!" << std::endl;
		return false;
	}

	std::ofstream Stream;
	Stream.open(outfilename, std::ios::out | std::ios::binary | std::ios::trunc);
	if(!Stream.is_open())
	{
		std::cout << "ERROR: cannot open " << outfilename << " for output." << std::endl;
		return false;
	}

	char header[4] = { 'B', 'M', 'F', 3 };
	Stream.write(header, 4);

	//info block
	char id = 1;
	Stream.write(&id, 1); //identifier
	int32_t sizeBlock = sizeof(info) + fontName.size() + 1;
	Stream.write((char *)&sizeBlock, 4);
	Stream.write((char *)&info, sizeof(info));
	Stream.write(fontName.c_str(), fontName.size() + 1);

	//common block
	id = 2;
	Stream.write(&id, 1); //identifier
	sizeBlock = sizeof(common);
	Stream.write((char *)&sizeBlock, 4);
	Stream.write((char *)&common, sizeof(common));

	//page block
	id = 3;
	Stream.write(&id, 1); //identifier
	sizeBlock = (pages[0].pageName.size() + 1) * pages.size();
	Stream.write((char *)&sizeBlock, 4);

	for(auto p : pages)
	{
		Stream.write(p.pageName.c_str(), p.pageName.size() + 1);
	}

	//character block
	id = 4;
	Stream.write(&id, 1); //identifier
	sizeBlock = Chars.size() * 20;
	Stream.write((char *)&sizeBlock, 4);

	for(auto c : Chars)
	{
		Stream.write((char *)&c, 20);
	}

	//kerning pairs block
	id = 5;
	Stream.write(&id, 1); //identifier
	sizeBlock = kerningPairs.size() * 10;
	Stream.write((char *)&sizeBlock, 4);

	for(auto k : kerningPairs)
	{
		Stream.write((char *)&k, 10);
	}
	return true;
}