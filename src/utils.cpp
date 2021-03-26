#include "utils.hpp"

#include <cctype>
#include <cstdio>

// Color escape table
static const float colorEscape[10][4] = {
    {0.0f, 0.0f, 0.0f, 1.0f},
    {1.0f, 0.0f, 0.0f, 1.0f},
    {0.0f, 1.0f, 0.0f, 1.0f},
    {0.0f, 0.0f, 1.0f, 1.0f},
    {1.0f, 1.0f, 0.0f, 1.0f},
    {1.0f, 0.0f, 1.0f, 1.0f},
    {0.0f, 1.0f, 1.0f, 1.0f},
    {1.0f, 1.0f, 1.0f, 1.0f},
    {0.7f, 0.7f, 0.7f, 1.0f},
    {0.4f, 0.4f, 0.4f, 1.0f}
};

int IsColorEscape(const char* str)
{
    if (str[0] != '^') {
        return 0;
    }
    if (isdigit(str[1])) {
        return 2;
    } else if (str[1] == 'x' || str[1] == 'X') {
        for (int c = 0; c < 6; c++) {
            if ( !isxdigit(str[c + 2]) ) {
                return 0;
            }
        }
        return 8;
    }
    return 0;
}

void ReadColorEscape(const char* str, float* out)
{
    int len = IsColorEscape(str);
    switch (len) {
    case 2:
        out[0] = colorEscape[str[1] - '0'][0];
        out[1] = colorEscape[str[1] - '0'][1];
        out[2] = colorEscape[str[1] - '0'][2];
        break;
    case 8:
    {
        int xr, xg, xb;
        sscanf(str + 2, "%2x%2x%2x", &xr, &xg, &xb);
        out[0] = xr / 255.0f;
        out[1] = xg / 255.0f;
        out[2] = xb / 255.0f;
    }
    break;
    }
}
