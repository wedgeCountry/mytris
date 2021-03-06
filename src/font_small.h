/*
 *
 * small_font
 *
 * created with FontCreator
 * written by F. Maximilian Thiele
 *
 * http://www.apetech.de/fontCreator
 * me@apetech.de
 *
 * File Name           : small_font.h
 * Date                : 13.08.2008
 * Font size in bytes  : 2796
 * Font width          : 4
 * Font height         : 8
 * Font first char     : 32
 * Font last char      : 126
 * Font used chars     : 94
 *
 * The font data are defined as
 *
 * struct _FONT_ {
 *     uint16_t   font_Size_in_Bytes_over_all_included_Size_it_self;
 *     uint8_t    font_Width_in_Pixel_for_fixed_drawing;
 *     uint8_t    font_Height_in_Pixel_for_all_characters;
 *     unit8_t    font_First_Char;
 *     uint8_t    font_Char_Count;
 *
 *     uint8_t    font_Char_Widths[font_Last_Char - font_First_Char +1];
 *                  // for each character the separate width in pixels,
 *                  // characters < 128 have an implicit virtual right empty row
 *
 *     uint8_t    font_data[];
 *                  // bit field of all characters
 */

#include <inttypes.h>
#include <avr/pgmspace.h>

#ifndef SMALL_FONT_H
#define SMALL_FONT_H

#define SMALL_FONT_WIDTH 4
#define SMALL_FONT_HEIGHT 8

static uint8_t small_font[] PROGMEM = {
    0x0A, 0xEC, // size
    0x04, // width
    0x08, // height
    0x20, // first char
    0x5E, // char count
    
    // char widths
    0x00, 0x01, 0x03, 0x05, 0x03, 0x05, 0x05, 0x01, 0x04, 0x04, 
    0x03, 0x03, 0x02, 0x03, 0x01, 0x04, 0x04, 0x03, 0x03, 0x03, 
    0x04, 0x03, 0x04, 0x04, 0x04, 0x04, 0x01, 0x02, 0x04, 0x04, 
    0x04, 0x04, 0x05, 0x05, 0x03, 0x04, 0x04, 0x03, 0x03, 0x04, 
    0x04, 0x03, 0x03, 0x04, 0x04, 0x05, 0x04, 0x05, 0x04, 0x05, 
    0x04, 0x03, 0x05, 0x04, 0x04, 0x05, 0x05, 0x05, 0x04, 0x02, 
    0x04, 0x02, 0x05, 0x05, 0x02, 0x04, 0x04, 0x03, 0x04, 0x03, 
    0x04, 0x04, 0x04, 0x02, 0x03, 0x04, 0x02, 0x05, 0x04, 0x04, 
    0x04, 0x04, 0x03, 0x03, 0x04, 0x04, 0x04, 0x05, 0x04, 0x05, 
    0x03, 0x03, 0x01, 0x03, 
    
    // font data
    0x2F, // 33
    0x03, 0x00, 0x03, // 34
    0x34, 0x1C, 0x36, 0x1C, 0x16, // 35
    0x26, 0x7F, 0x32, // 36
    0x32, 0x0D, 0x1E, 0x2C, 0x13, // 37
    0x18, 0x26, 0x2D, 0x12, 0x28, // 38
    0x03, // 39
    0x1C, 0x22, 0x41, 0x41, // 40
    0x41, 0x41, 0x22, 0x1C, // 41
    0x0A, 0x05, 0x0A, // 42
    0x10, 0x38, 0x10, // 43
    0x80, 0x60, // 44
    0x08, 0x08, 0x08, // 45
    0x20, // 46
    0x60, 0x18, 0x06, 0x01, // 47
    0x1E, 0x21, 0x21, 0x1E, // 48
    0x22, 0x3F, 0x20, // 49
    0x31, 0x29, 0x26, // 50
    0x25, 0x25, 0x1A, // 51
    0x0C, 0x0A, 0x3F, 0x08, // 52
    0x27, 0x25, 0x19, // 53
    0x1E, 0x25, 0x25, 0x18, // 54
    0x01, 0x39, 0x05, 0x03, // 55
    0x1A, 0x25, 0x25, 0x1A, // 56
    0x06, 0x29, 0x29, 0x1E, // 57
    0x24, // 58
    0x80, 0x64, // 59
    0x08, 0x08, 0x14, 0x22, // 60
    0x14, 0x14, 0x14, 0x14, // 61
    0x22, 0x14, 0x08, 0x08, // 62
    0x01, 0x29, 0x05, 0x02, // 63
    0x3C, 0x42, 0x59, 0x55, 0x5E, // 64
    0x30, 0x1C, 0x12, 0x1C, 0x30, // 65
    0x3E, 0x2A, 0x36, // 66
    0x1C, 0x22, 0x22, 0x22, // 67
    0x3E, 0x22, 0x22, 0x1C, // 68
    0x3E, 0x2A, 0x2A, // 69
    0x3E, 0x0A, 0x0A, // 70
    0x1C, 0x22, 0x2A, 0x3A, // 71
    0x3E, 0x08, 0x08, 0x3E, // 72
    0x22, 0x3E, 0x22, // 73
    0x22, 0x22, 0x1E, // 74
    0x3E, 0x08, 0x14, 0x22, // 75
    0x3E, 0x20, 0x20, 0x20, // 76
    0x3E, 0x04, 0x18, 0x04, 0x3E, // 77
    0x3E, 0x04, 0x08, 0x3E, // 78
    0x1C, 0x22, 0x22, 0x22, 0x1C, // 79
    0x3E, 0x0A, 0x0A, 0x04, // 80
    0x1C, 0x22, 0x22, 0x62, 0x9C, // 81
    0x3E, 0x0A, 0x14, 0x20, // 82
    0x24, 0x2A, 0x12, // 83
    0x02, 0x02, 0x3E, 0x02, 0x02, // 84
    0x1E, 0x20, 0x20, 0x1E, // 85
    0x0E, 0x30, 0x30, 0x0E, // 86
    0x0E, 0x30, 0x0C, 0x30, 0x0E, // 87
    0x22, 0x14, 0x08, 0x14, 0x22, // 88
    0x02, 0x04, 0x38, 0x04, 0x02, // 89
    0x32, 0x2A, 0x2A, 0x26, // 90
    0x7F, 0x41, // 91
    0x01, 0x06, 0x18, 0x60, // 92
    0x41, 0x7F, // 93
    0x18, 0x06, 0x01, 0x06, 0x18, // 94
    0x40, 0x40, 0x40, 0x40, 0x40, // 95
    0x01, 0x02, // 96
    0x34, 0x34, 0x38, 0x20, // 97
    0x3F, 0x24, 0x24, 0x18, // 98
    0x18, 0x24, 0x24, // 99
    0x18, 0x24, 0x24, 0x3F, // 100
    0x18, 0x2C, 0x28, // 101
    0x04, 0x3E, 0x05, 0x05, // 102
    0x58, 0x54, 0x54, 0x3C, // 103
    0x3F, 0x08, 0x04, 0x38, // 104
    0x04, 0x3D, // 105
    0x44, 0x44, 0x3D, // 106
    0x3F, 0x08, 0x14, 0x20, // 107
    0x01, 0x3F, // 108
    0x3C, 0x08, 0x3C, 0x08, 0x3C, // 109
    0x3C, 0x08, 0x04, 0x38, // 110
    0x18, 0x24, 0x24, 0x18, // 111
    0x7C, 0x24, 0x24, 0x18, // 112
    0x18, 0x24, 0x24, 0x7C, // 113
    0x3C, 0x08, 0x04, // 114
    0x28, 0x2C, 0x14, // 115
    0x04, 0x1E, 0x24, 0x04, // 116
    0x1C, 0x20, 0x10, 0x3C, // 117
    0x0C, 0x30, 0x30, 0x0C, // 118
    0x0C, 0x30, 0x1C, 0x30, 0x0C, // 119
    0x24, 0x18, 0x18, 0x24, // 120
    0x40, 0x4C, 0x70, 0x30, 0x0C, // 121
    0x34, 0x2C, 0x2C, // 122
    0x08, 0x36, 0x41, // 123
    0x7F, // 124
    0x41, 0x36, 0x08 // 125
    
};

#endif
