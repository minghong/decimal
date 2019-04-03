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

/**
 * sch1 的整数部分可能比d1实际整数部分位数多，此处不做优化。
 * 因为针对一个column的decimal运算，计算结果需要有统一的schema 
 * 
 */
int add(DecimalSchema sch1, Decimal d1, DecimalSchema sch2, Decimal d2, DecimalSchema &sch3, Decimal &d3)
{
    char scale = max(sch1.scale, sch2.scale);
    char zpart = max(sch1.precision - sch1.scale, sch2.precision - sch2.scale);
    sch3 = {min( zpart + scale + 1, 18), scale};
    char shift = scale - sch1.scale;
    if (shift) d1 = d1 * scaleTable[shift];
    shift = scale - sch2.scale;
    if (shift) d2 = d2 * scaleTable[shift];
    d3 = d1 + d2;
    return 0;
}

int sub(DecimalSchema sch1, Decimal d1, DecimalSchema sch2, Decimal d2, DecimalSchema &sch3, Decimal &d3)
{
    char scale = max(sch1.scale, sch2.scale);
    char zpart = max(sch1.precision - sch1.scale, sch2.precision - sch2.scale);
    sch3 = {min( zpart + scale + 1, 18), scale};
    char shift = scale - sch1.scale;
    if (shift) d1 = d1 * scaleTable[shift];
    shift = scale - sch2.scale;
    if (shift) d2 = d2 * scaleTable[shift];
    d3 = d1 - d2;
    return 0;
}

int multiply(DecimalSchema sch1, Decimal d1, DecimalSchema sch2, Decimal d2, DecimalSchema &sch3, Decimal &d3)
{
    sch3.precision = MAX_PRECISION;
    
    sch3.scale = sch1.scale + sch2.scale;
    d3 = d1 * d2;
    return 0;
}

int div(DecimalSchema sch1, Decimal d1, DecimalSchema sch2, Decimal d2, DecimalSchema &sch3, Decimal &d3)
{
    const char MAX_DIV_SCALE = 6;
    char scale = max( MAX_DIV_SCALE, max( sch1.scale, sch2.scale ) );//scale for result
    long long z1 = d1 / d2;
    long long m1 = d1 % d2;
    int scalediff = sch2.scale - sch1.scale;
    if (scalediff <= 0){
        scalediff = 0 - scalediff;
        if (scalediff > scale)
        {
            d3 = z1 / scaleTable[scalediff - scale];
        }else if (scalediff == scale) {
            d3 = z1;
        }else{ //scalediff < scale
            char scalemore = scale - scalediff; //z1 左移 scalemore 位， 尾部 由 m1/d2 补充
            m1 = m1 * scaleTable[scalemore];
            z1 = z1 * scaleTable[scalemore];
            long long z2 = m1/d2;
            d3 = z1 + z2;
        }
    // }else if (scalediff == 0){
    //     d3 = z1;
    }else{
        d3 = z1 * scaleTable[scalediff + scale];
        m1 = m1 * scaleTable[scalediff+scale];
        d3 += m1 / d2;
    }
    sch3.precision = MAX_PRECISION;
    sch3.scale = scale;
    
    return 0;
}