#pragma once

/*
Angelcode bitmap font class for converting text->binary files.
*/

#include <string>
#include <vector>
#include <stdint.h>


struct AngelcodeCharDescriptor
{
public:
	uint32_t id;
	uint16_t x, y;
	uint16_t width, height;
	int16_t xOffset, yOffset;
	int16_t xAdvance;
	uint16_t page;
	uint16_t chnl;

	AngelcodeCharDescriptor() : x(0), y(0), width(0), height(0), xOffset(0), yOffset(0),
		xAdvance(0)
	{ }
};

struct Info
{
	int16_t fontSize;
	int8_t bitField;
	uint8_t charSet;
	uint16_t stretchH;
	uint8_t aa;
	uint8_t paddingUp, paddingRight, paddingDown, paddingLeft;
	uint8_t spacingHoriz, spacingVert;
	uint8_t outline;
	//store fontname seperately, makes it easier to calculate size
	Info() : fontSize(0), bitField(0), charSet(0), stretchH(0), aa(0),
		paddingUp(0), paddingRight(0), paddingDown(0), paddingLeft(0),
		spacingHoriz(0), spacingVert(0), outline(0)
	{ }
};

struct Common
{
	uint16_t lineHeight;
	uint16_t base;
	uint16_t scaleW, scaleH;
	uint16_t pages;
	int8_t bitField;
	uint16_t alphaChnl, redChnl, greenChnl, blueChnl;
	Common() : lineHeight(0), base(0), scaleW(0), scaleH(0), pages(0), bitField(0),
		alphaChnl(0), redChnl(0), greenChnl(0), blueChnl(0)
	{ }
};

struct KerningPair
{
	uint32_t first;
	uint32_t second;
	int16_t amount;
	KerningPair() : first(0), second(0), amount(0)
	{ }
};

struct Page
{
	uint8_t id;
	std::string pageName;
	Page() : id(0)
	{ }
};

class AngelcodeFontConverter
{
private:
	char endian;
	Info info;
	std::string fontName;
	Common common;
	std::vector<Page> pages; //filenames of the textures
	std::vector<AngelcodeCharDescriptor> Chars;
	std::vector<KerningPair> kerningPairs;
public:	
	AngelcodeFontConverter();
	~AngelcodeFontConverter();
	bool Read(std::string fontfile);
	bool Write(std::string outfilename);
};