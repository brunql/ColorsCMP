//================================================//
//==		lcd_nokia_3310_font_lookup.h		==//
//================================================//
// Date: 21.01.2010
// Author  : brunql
//================================================//

#include "main.h" // if here defined USING_RU_FONTS includes in flash FontLookup_RU

// 	775 bytes

prog_uchar FontLookup[91][5]= 
{
	{ 0x00, 0x00, 0x00, 0x00, 0x00 },  // sp
	{ 0x00, 0x00, 0x2f, 0x00, 0x00 },   // !
    { 0x00, 0x07, 0x00, 0x07, 0x00 },   // "
    { 0x14, 0x7f, 0x14, 0x7f, 0x14 },   // #
    { 0x24, 0x2a, 0x7f, 0x2a, 0x12 },   // $
    { 0xc4, 0xc8, 0x10, 0x26, 0x46 },   // %
    { 0x36, 0x49, 0x55, 0x22, 0x50 },   // &
    { 0x00, 0x05, 0x03, 0x00, 0x00 },   // '
    { 0x00, 0x1c, 0x22, 0x41, 0x00 },   // (
    { 0x00, 0x41, 0x22, 0x1c, 0x00 },   // )
    { 0x14, 0x08, 0x3E, 0x08, 0x14 },   // *
    { 0x08, 0x08, 0x3E, 0x08, 0x08 },   // +
    { 0x00, 0x00, 0x50, 0x30, 0x00 },   // ,
    { 0x10, 0x10, 0x10, 0x10, 0x10 },   // -
    { 0x00, 0x60, 0x60, 0x00, 0x00 },   // .
    { 0x20, 0x10, 0x08, 0x04, 0x02 },   // /
    { 0x3E, 0x51, 0x49, 0x45, 0x3E },   // 0 
    { 0x00, 0x42, 0x7F, 0x40, 0x00 },   // 1
    { 0x42, 0x61, 0x51, 0x49, 0x46 },   // 2
    { 0x21, 0x41, 0x45, 0x4B, 0x31 },   // 3
    { 0x18, 0x14, 0x12, 0x7F, 0x10 },   // 4
    { 0x27, 0x45, 0x45, 0x45, 0x39 },   // 5
    { 0x3C, 0x4A, 0x49, 0x49, 0x30 },   // 6
    { 0x01, 0x71, 0x09, 0x05, 0x03 },   // 7
    { 0x36, 0x49, 0x49, 0x49, 0x36 },   // 8
    { 0x06, 0x49, 0x49, 0x29, 0x1E },   // 9
    { 0x00, 0x36, 0x36, 0x00, 0x00 },   // :
    { 0x00, 0x56, 0x36, 0x00, 0x00 },   // ;
    { 0x08, 0x14, 0x22, 0x41, 0x00 },   // <
    { 0x14, 0x14, 0x14, 0x14, 0x14 },   // =
    { 0x00, 0x41, 0x22, 0x14, 0x08 },   // >
    { 0x02, 0x01, 0x51, 0x09, 0x06 },   // ?
    { 0x3E, 0x59, 0x65, 0x65, 0x5E },   // @
    { 0x7E, 0x11, 0x11, 0x11, 0x7E },   // A
    { 0x7F, 0x49, 0x49, 0x49, 0x36 },   // B
    { 0x3E, 0x41, 0x41, 0x41, 0x22 },   // C
    { 0x7F, 0x41, 0x41, 0x22, 0x1C },   // D
    { 0x7F, 0x49, 0x49, 0x49, 0x41 },   // E
    { 0x7F, 0x09, 0x09, 0x09, 0x01 },   // F
    { 0x3E, 0x41, 0x49, 0x49, 0x7A },   // G
    { 0x7F, 0x08, 0x08, 0x08, 0x7F },   // H
    { 0x00, 0x41, 0x7F, 0x41, 0x00 },   // I
    { 0x20, 0x40, 0x41, 0x3F, 0x01 },   // J
    { 0x7F, 0x08, 0x14, 0x22, 0x41 },   // K
    { 0x7F, 0x40, 0x40, 0x40, 0x40 },   // L
    { 0x7F, 0x02, 0x0C, 0x02, 0x7F },   // M
    { 0x7F, 0x04, 0x08, 0x10, 0x7F },   // N
    { 0x3E, 0x41, 0x41, 0x41, 0x3E },   // O
    { 0x7F, 0x09, 0x09, 0x09, 0x06 },   // P
    { 0x3E, 0x41, 0x51, 0x21, 0x5E },   // Q
    { 0x7F, 0x09, 0x19, 0x29, 0x46 },   // R
    { 0x46, 0x49, 0x49, 0x49, 0x31 },   // S
    { 0x01, 0x01, 0x7F, 0x01, 0x01 },   // T
    { 0x3F, 0x40, 0x40, 0x40, 0x3F },   // U
    { 0x1F, 0x20, 0x40, 0x20, 0x1F },   // V
    { 0x3F, 0x40, 0x38, 0x40, 0x3F },   // W
    { 0x63, 0x14, 0x08, 0x14, 0x63 },   // X
    { 0x07, 0x08, 0x70, 0x08, 0x07 },   // Y
    { 0x61, 0x51, 0x49, 0x45, 0x43 },   // Z
    { 0x00, 0x7F, 0x41, 0x41, 0x00 },   // [
    { 0x55, 0x2A, 0x55, 0x2A, 0x55 },   // 55
    { 0x00, 0x41, 0x41, 0x7F, 0x00 },   // ]
    { 0x04, 0x02, 0x01, 0x02, 0x04 },   // ^
    { 0x40, 0x40, 0x40, 0x40, 0x40 },   // _
    { 0x00, 0x01, 0x02, 0x04, 0x00 },   // '
    { 0x20, 0x54, 0x54, 0x54, 0x78 },   // a
    { 0x7F, 0x48, 0x44, 0x44, 0x38 },   // b
    { 0x38, 0x44, 0x44, 0x44, 0x20 },   // c
    { 0x38, 0x44, 0x44, 0x48, 0x7F },   // d
    { 0x38, 0x54, 0x54, 0x54, 0x18 },   // e
    { 0x08, 0x7E, 0x09, 0x01, 0x02 },   // f
    { 0x0C, 0x52, 0x52, 0x52, 0x3E },   // g
    { 0x7F, 0x08, 0x04, 0x04, 0x78 },   // h
    { 0x00, 0x44, 0x7D, 0x40, 0x00 },   // i
    { 0x20, 0x40, 0x44, 0x3D, 0x00 },   // j
    { 0x7F, 0x10, 0x28, 0x44, 0x00 },   // k
    { 0x00, 0x41, 0x7F, 0x40, 0x00 },   // l
    { 0x7C, 0x04, 0x18, 0x04, 0x78 },   // m
    { 0x7C, 0x08, 0x04, 0x04, 0x78 },   // n
    { 0x38, 0x44, 0x44, 0x44, 0x38 },   // o
    { 0x7C, 0x14, 0x14, 0x14, 0x08 },   // p
    { 0x08, 0x14, 0x14, 0x18, 0x7C },   // q
    { 0x7C, 0x08, 0x04, 0x04, 0x08 },   // r
    { 0x48, 0x54, 0x54, 0x54, 0x20 },   // s
    { 0x04, 0x3F, 0x44, 0x40, 0x20 },   // t
    { 0x3C, 0x40, 0x40, 0x20, 0x7C },   // u
    { 0x1C, 0x20, 0x40, 0x20, 0x1C },   // v
    { 0x3C, 0x40, 0x30, 0x40, 0x3C },   // w
    { 0x44, 0x28, 0x10, 0x28, 0x44 },   // x
    { 0x0C, 0x50, 0x50, 0x50, 0x3C },   // y
    { 0x44, 0x64, 0x54, 0x4C, 0x44 }    // z
};

#ifdef USING_RU_FONTS
prog_uchar FontLookup_RU[64][5]= 
{
	{ 0xF0, 0x2C, 0x22, 0x2C, 0xF0 },	// �  - 0xC0
	{ 0xFE, 0x92, 0x92, 0x92, 0xF2 },
	{ 0xFE, 0x92, 0x92, 0x9E, 0xF0 },
	{ 0xFE, 0x02, 0x02, 0x02, 0x02 },
	{ 0xC0, 0x7C, 0x42, 0x7C, 0xC0 },
	{ 0xFE, 0x92, 0x92, 0x92, 0x92 },
	{ 0xC6, 0x28, 0xFE, 0x28, 0xC6 },
	{ 0x82, 0x92, 0x92, 0x92, 0x6C },
	{ 0xFE, 0x40, 0x20, 0x10, 0xFE },
	{ 0xFE, 0x40, 0x22, 0x10, 0xFE },
	{ 0xFE, 0x10, 0x28, 0x44, 0x82 },
	{ 0xF8, 0x04, 0x02, 0x02, 0xFE },
	{ 0xFE, 0x04, 0x08, 0x04, 0xFE },
	{ 0xFE, 0x10, 0x10, 0x10, 0xFE },
	{ 0x7C, 0x82, 0x82, 0x82, 0x7C },
	{ 0xFE, 0x02, 0x02, 0x02, 0xFE },
	{ 0xFE, 0x22, 0x22, 0x22, 0x1C },
	{ 0x7C, 0x82, 0x82, 0x82, 0x82 },
	{ 0x02, 0x02, 0xFE, 0x02, 0x02 },
	{ 0x4E, 0x90, 0x90, 0x90, 0x7E },
	{ 0x1C, 0x22, 0xFE, 0x22, 0x1C },
	{ 0x82, 0x44, 0x38, 0x44, 0x82 },
	{ 0x3E, 0x40, 0x40, 0x7E, 0xC0 },
	{ 0x0E, 0x10, 0x10, 0x10, 0xFE },
	{ 0x7E, 0x40, 0x7C, 0x40, 0x7E },
	{ 0x7E, 0x40, 0x7C, 0x40, 0xFE },
	{ 0x02, 0xFE, 0x90, 0x90, 0x60 },
	{ 0xFE, 0x90, 0x60, 0x00, 0xFE },
	{ 0x00, 0xFE, 0x90, 0x90, 0x60 },
	{ 0x44, 0x92, 0x92, 0x92, 0x7C },
	{ 0xFE, 0x10, 0xFE, 0x82, 0xFE },
	{ 0x8C, 0x52, 0x32, 0x12, 0xFE },

	{ 0x48, 0xA4, 0x94, 0xA4, 0xF8 }, // � - 0xE0 
	{ 0xFC, 0x94, 0x94, 0x94, 0x64 },
	{ 0xFC, 0x94, 0x94, 0x98, 0x60 },
	{ 0xFC, 0x04, 0x04, 0x04, 0x04 },
	{ 0xC0, 0x78, 0x44, 0x78, 0xC0 },
	{ 0x78, 0xA4, 0xA4, 0x94, 0x58 },
	{ 0xC4, 0x28, 0xFC, 0x28, 0xC4 },
	{ 0x84, 0x84, 0x94, 0x94, 0x68 },
	{ 0x7C, 0x80, 0x80, 0x40, 0xFC },
	{ 0x7C, 0x80, 0x84, 0x40, 0xFC },
	{ 0xFC, 0x10, 0x10, 0x68, 0x84 },
	{ 0xF0, 0x08, 0x04, 0x04, 0xFC },
	{ 0xFC, 0x08, 0x10, 0x08, 0xFC },
	{ 0xFC, 0x10, 0x10, 0x10, 0xFC },
	{ 0x78, 0x84, 0x84, 0x84, 0x78 },
	{ 0xFC, 0x04, 0x04, 0x04, 0xFC },
	{ 0xFC, 0x24, 0x24, 0x24, 0x18 },
	{ 0x78, 0x84, 0x84, 0x84, 0x84 },
	{ 0x04, 0x04, 0xFC, 0x04, 0x04 },
	{ 0x4C, 0x90, 0x90, 0x90, 0x7C },
	{ 0x18, 0x24, 0xFC, 0x24, 0x18 },
	{ 0x84, 0x48, 0x30, 0x48, 0x84 },
	{ 0x3C, 0x40, 0x40, 0x7C, 0xC0 },
	{ 0x1C, 0x20, 0x20, 0x20, 0xFC },
	{ 0x7C, 0x40, 0x78, 0x40, 0x7C },
	{ 0x7C, 0x40, 0x78, 0x40, 0xFC },
	{ 0x04, 0xFC, 0xA0, 0xA0, 0x40 },
	{ 0xFC, 0xA0, 0xE0, 0x00, 0xFC },
	{ 0x00, 0xFC, 0xA0, 0xA0, 0x40 },
	{ 0x48, 0x84, 0x94, 0x94, 0x78 },
	{ 0xFC, 0x10, 0xFC, 0x84, 0xFC },
	{ 0x88, 0x94, 0x54, 0x34, 0xFC }
};
#endif
