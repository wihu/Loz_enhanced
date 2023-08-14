#pragma once

#include <stdint.h>

struct Bounds
{
    uint16_t X;
    uint16_t Y;
    uint16_t Width;
    uint16_t Height;
};

struct Point
{
    short   X;
    short   Y;
};

struct Cell
{
    uint8_t Row;
    uint8_t Col;
};
