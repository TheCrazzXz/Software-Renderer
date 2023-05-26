#pragma once

#ifndef __MEMUTILS_H__
#define __MEMUTILS_H__

// Sets a byte of a number
#define SET_BYTE(INPUT, VALUE, POSITION)   (INPUT=(VALUE<<(POSITION<<3))|(INPUT&(0xFFFFFFFF^(0xFF<<(POSITION<<3)))))
#define GET_BYTE(INPUT, POSITION) ((INPUT >> (8*POSITION)) & 0xff)

#define COLOR_RED(INPUT) GET_BYTE(INPUT, 2)
#define COLOR_GREEN(INPUT) GET_BYTE(INPUT, 1)
#define COLOR_BLUE(INPUT) GET_BYTE(INPUT, 0)

// Fill a Uint32 buffer with val
void *memsetu32(void *m, Uint32 val, size_t count)
{
    Uint32 *buf = (Uint32*) m;

    while(count--) *buf++ = val;
    return m;
}

#endif