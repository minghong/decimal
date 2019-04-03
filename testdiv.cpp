#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "decimal.h"

typedef struct {
    const char * d1;
    const char * d2;
    const char * d3;
} testcase;

int main()
{
    testcase cases[] = {
        //同号
        {"1234.4", "5.999", "205.767627"},   //s2>s1
        {"1234.56789", "5.43", "227.360569"},//s2<s1, diff<s       
        {"1234.1234567", "5.3", "232.8534823"}, //s2<s1, diff=s
        {"1234.123456789", "5.3", "232.853482413"}, //s2<s1, diff>s
        {"1234.123456789", "5.987654321", "206.111340205"}, //s2=s1
        
        {"-1234.4", "-5.999", "205.767627"},   //s2>s1
        {"-1234.56789", "-5.43", "227.360569"},//s2<s1, diff<s       
        {"-1234.1234567", "-5.3", "232.8534823"}, //s2<s1, diff=s
        {"-1234.123456789", "-5.3", "232.853482413"}, //s2<s1, diff>s
        {"-1234.123456789", "-5.987654321", "206.111340205"}, //s2=s1

        //异号
        {"-1234.4", "5.999", "-205.767627"},   //s2>s1
        {"1234.56789", "-5.43", "-227.360569"},//s2<s1, diff<s       
        {"-1234.1234567", "5.3", "-232.8534823"}, //s2<s1, diff=s
        {"-1234.123456789", "5.3", "-232.853482413"}, //s2<s1, diff>s
        {"-1234.123456789", "5.987654321", "-206.111340205"}, //s2=s1
        
        //整数
        {"1234123456789", "5.3", "232853482413.018867"}, //s2<s1, diff>s
        {"1234.123456789", "5987654321", "0.000000206"}, //s2=s1
    };

    int count = sizeof(cases) / sizeof(testcase);
    bool passed = true;
    for(int i=0; i<count; i++)
    {
        Decimal d1;
        DecimalSchema sch1 = StringToDecimal(cases[i].d1, d1);
        Decimal d2;
        DecimalSchema sch2 = StringToDecimal(cases[i].d2, d2);
        Decimal d3;
        DecimalSchema sch3;
        int err = div(sch1, d1, sch2, d2, sch3, d3);
        char buff[20];
        DecimalToString(sch3, d3, buff);
        if (strcmp(buff, cases[i].d3)) {
            passed = false;
            printf("failed: %s / %s = %s, expect %s\n", cases[i].d1, cases[i].d2, buff, cases[i].d3);
        }
    }
    if (passed) printf("div passed\n");
    return 0;
}