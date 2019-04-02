#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "decimal.h"

Decimal scaleTable [] = {
    1, //scale 0
    10,
    100,
    1000,
    10000,
    100000,
    1000000,
    10000000,
    100000000,
    1000000000,
    10000000000,
    100000000000,
    1000000000000,
    10000000000000,
    100000000000000,
    1000000000000000,
    10000000000000000,
    100000000000000000,
    1000000000000000000,
};

void DecimalToString(DecimalSchema sch, Decimal d, char * str)
{
    long long z = d / scaleTable[sch.scale];
    long long f = d % scaleTable[sch.scale];
    int n= sprintf(str, "%lld", z);
    str = str + n;
    if (f)
    {
        if (f<0) f = 0 - f;
        str[0] = '.';
        str ++;
        char frac[18];
        int m = sprintf(frac, "%lld", f);
        int zeros = sch.scale - m;
        for(int i=0; i<zeros; i++) str[i] = '0';
        str = str + zeros;
        sprintf(str, "%lld", f);
    }
    
}


DecimalSchema StringToDecimal(const char* str, Decimal &d)
{
    char sign = 1;
    if(*str == '-')
    {
        str ++;
        sign = -1;
    }
    char* end;
    d = strtoll(str, &end, 10);
    if (errno != 0){
        errno = 0;
        return {-1};
    }

    char zlen = end - str;
    if (*end != '.') return {zlen, 0};

    str = end + 1;
    long long frac = strtoll(str, &end, 10);
    if (errno != 0){
        errno = 0;
        return {-1};
    }
    char scale = end - str;
    d = d * scaleTable[scale] + frac;
    if (sign == -1) d = 0 - d;
    return {zlen+scale, scale};
}

int add(DecimalSchema sch1, Decimal d1, DecimalSchema sch2, Decimal d2, DecimalSchema &sch3, Decimal &d3)
{
    char scale = max(sch1.scale, sch2.scale);
    char zpart = max(sch1.precision - sch1.scale, sch2.precision - sch2.scale);
    if (zpart + scale > 18) return -1; //overflow
    sch3 = {zpart + scale, scale};
    char shift = scale - sch1.scale;
    if (shift) d1 = d1 * scaleTable[shift];
    shift = scale - sch2.scale;
    if (shift) d2 = d2 * scaleTable[shift];
    d3 = d1 + d2;
    return 0;
}