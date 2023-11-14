/* vim: set ai et ts=4 sw=4: */
#ifndef __FONTS_DEF_H__
#define __FONTS_DEF_H__

#include <stdint.h>

typedef struct {
    const uint8_t width;
    uint8_t height;
    const uint32_t size;
    const uint16_t *data;
} FontDef;

//const uint8_t Font_Rows = 95;
//extern FontDef Font_7x10;
//extern FontDef Font_11x18;
//extern FontDef Font_16x26;

#endif // __FONTS_H__
