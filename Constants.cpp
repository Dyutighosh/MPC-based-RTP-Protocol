#include "Constants.hpp"

const double d[10] = {6.57312, 3.19614, 9.39501, 3.23684, 7.11644, 9.60693, 7.11762, 5.15677, 8.61886, 7.70245};
const double eph[10] = {-0.812197, -0.908682, -0.731571, -0.907519, -0.796673, -0.725516, -0.796639, -0.852664, -0.753747, -0.77993};

const uint16_t sdouble_approxRecip_1k0s[16] = {
    0xFFC4, 0xF0BE, 0xE363, 0xD76F, 0xCCAD, 0xC2F0, 0xBA16, 0xB201,
    0xAA97, 0xA3C6, 0x9D7A, 0x97A6, 0x923C, 0x8D32, 0x887E, 0x8417
};

const uint16_t sdouble_approxRecip_1k1s[16] = {
    0xF0F1, 0xD62C, 0xBFA1, 0xAC77, 0x9C0A, 0x8DDB, 0x8185, 0x76BA,
    0x6D3B, 0x64D4, 0x5D5C, 0x56B1, 0x50B6, 0x4B55, 0x4679, 0x4211
};

uint_fast8_t sdouble_roundingMode = sdouble_round_near_even;
uint_fast8_t sdouble_detectTininess = init_detectTininess;

double alpha1 = 2.04999713368002112190424668369814753532409667968750;
double alpha2 = 0.36462958398842182372590059458161704242229461669922;