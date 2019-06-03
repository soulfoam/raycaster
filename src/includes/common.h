#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <SDL2/SDL.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

#define lerp(start, end, percent) (start + percent*(end - start))
#define min(a, b) (a < b ? a : b)
#define max(a, b) (a > b ? a : b)
#define clamp(num, low, high) min(high, max(low, num))

static inline float dist(float x0, float y0, float z0, float x1, float y1, float z1){
    float x_distance = x0 - x1;
    float y_distance = y0 - y1;
    float z_distance = z0 - z1;
    
    return sqrt((x_distance * x_distance) 
            + (y_distance * y_distance)
            + (z_distance * z_distance));
}

static const char* CHARACTERS =
    " !\"#$%&'()*+,-./"
    "0123456789:;<=>?"
    "@ABCDEFGHIJKLMNO"
    "PQRSTUVWXYZ[\\]^_"
    "`abcdefghijklmno"
    "pqrstuvwxyz{|}~ ";
static const int CHARACTERS_LENGTH = 96;
static const int CHARACTER_WIDTH = 6;
static const int CHARACTER_HEIGHT = 8;

static inline int get_character_index(char c){
    for (int i = 0; i < CHARACTERS_LENGTH; i++){
        if (CHARACTERS[i] == c){
            return i;
        }
    }
    return -1;
}

#endif
