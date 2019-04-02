#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "decimal.h"

typedef struct {
    char * d1;
    char * d2;
    char * d3;
} testcase;

int main()
{
    testcase cases[] = {
        {"1234.4", "5.999", ""},
    };

    int count = sizeof(cases) / sizeof(testcase);
    for(int i=0; i<count; i++)
    {
        Decimal d1;
        DecimalSchema sch1 = StringToDecimal(cases[i].d1, d1);
        Decimal d2;
        DecimalSchema sch2 = StringToDecimal(cases[i].d2, d2);
        Decimal d3;
        DecimalSchema sch3;
        int err = add(sch1, d1, sch2, d2, sch3, d3);
        char buff[20];
        DecimalToString(sch3, d3, buff);
        printf("%s\n", buff);
    }
    return 0;
}