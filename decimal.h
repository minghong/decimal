
#define MAX_PRECISION 18
typedef long long Decimal;


typedef struct{
    char precision;
    char scale;
}DecimalSchema;

template<typename T>
__inline__ T max(T t1, T t2) { return t1 > t2 ? t1: t2;}

template<typename T>
__inline__ T min(T t1, T t2) { return t1 < t2 ? t1: t2;}

void DecimalToString(DecimalSchema sch, Decimal d, char * str);
DecimalSchema StringToDecimal(const char* str, Decimal &d);

/**
 * d1+d2=d3
 * return 0, success
 *        -1, overflow
 **/
int add(DecimalSchema sch1, Decimal d1, DecimalSchema sch2, Decimal d2, DecimalSchema &sch3, Decimal &d3);
int sub(DecimalSchema sch1, Decimal d1, DecimalSchema sch2, Decimal d2, DecimalSchema &sch3, Decimal &d3);
int multiply(DecimalSchema sch1, Decimal d1, DecimalSchema sch2, Decimal d2, DecimalSchema &sch3, Decimal &d3);
int div(DecimalSchema sch1, Decimal d1, DecimalSchema sch2, Decimal d2, DecimalSchema &sch3, Decimal &d3);