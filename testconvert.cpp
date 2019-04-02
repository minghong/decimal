#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "decimal.h"


int main()
{
    const char* str[] = {
        "123456789.0987654321",
        "123456.0987654321",
        "12345",
        "-123456789.",
        ".12345A67",
        "-123.456",
        "abcd3",
    };
    const char* exp[] = {
        "123456789.0987654321",
        "123456.0987654321",
        "12345",
        "-123456789",
        "0.12345",
        "-123.456",
        "0",
    };
    int count = sizeof(str) / sizeof(char*);
    for(int i = 0; i<count; i++)
    {
        long long d;
        char sd[100];
        DecimalSchema s = StringToDecimal(str[i], d);
        DecimalToString(s, d, sd);
        if(strcmp(sd, exp[i])){
            printf("failed: %s => expect:%s, fact:%s\n", str[i], exp[i], sd);
        }   
        
    }
    return 0;
}