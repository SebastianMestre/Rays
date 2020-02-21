// bmp.h
#pragma once

#include <stdint.h>

void bmp_write (char const* fname, uint8_t* red, uint8_t* green, uint8_t* blue, int w, int h);
