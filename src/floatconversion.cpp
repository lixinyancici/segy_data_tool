#include "floatconversion.h"
#include <math.h>
#include <cmath>

FloatConversion::FloatConversion()
{
}

FloatConversion::~FloatConversion()
{
}

float FloatConversion::ibm2decimal( quint32 ibm)
{
    // gain sign from first bit
    double sign = (double)(ibm >> 31) ;

    // gain exponent from first byte, last 7 bits
    double exp0 = (double)( (ibm & 0x7f000000) >> 24) ;

    // remove bias from exponent
    double exp =(double)(exp0 - 64 );

    // gain mantissa from last 3 bytes
    double frac = (double)(ibm & 0x00ffffff);
    double fmant = frac / ( pow(2.0, 24) );

    float result;
    result = (1-2*sign) * ( pow(16, exp) ) * fmant;

    return result;
}



float FloatConversion::ieee2Decimal( quint32 ieee )
{
    int sign;
    sign = ( ieee & 0x80000000 ) * pow(2.0, -31);

    if (sign == 1) // for value < 0 :
    {
        ieee = ~(ieee & 0x7fffffff);
    }

    int e;
    e = (ieee & 0x7f800000 ) * pow(2.0, -23) - 127 - sign;

    unsigned int x ;
    x = ieee & 0x007fffff - sign; // -sign : for value < 0

    float x0 ;
    x0 = x * pow(2.0, -23);

    float result;

    if ( x0 == 0 && e + 127 == 0 )
    {
        result = 0;
    } else
    {
        result = pow(-1.0, sign) * (1 + x0) * pow(2.0, e);
    }

    return result;
}



quint32 FloatConversion::decimal2ibm( float dec )
{
    long sign;
    sign = (dec < 0) ? 1 : 0;

    long exp;
    exp = 0;

    dec = dec * pow(-1.0, sign);// abs(dec)
    float input1 ; // attention : cannot use long input1;
    input1 = dec;

    if ( dec > 0 )
    {
        if( (int)dec > 0 )
        {
            exp ++;
            while( (int)input1/16 > 0 )
            {
                exp ++;
                input1 = input1 / 16;
            }
        } else
        {
            while ( (int)input1*16 == 0 )
            {
                exp--;
                input1 = input1 * 16;
            }
            exp ++;// attention :ibm fmant:0.mantnot 1.mant !
        }
    }

    long e;
    e = ( exp + 64 ) ;
    double fm = dec * pow(16.0, -exp);
    long fmant = (long)( fm * pow(2.0, 24) ) ;

    unsigned long result ;
    result = (sign << 31) | (e << 24) | fmant ;

    return result;
}



quint32 FloatConversion::decimal2ieee( float dec )
{
    int sign, e;
    unsigned int x;
    sign = (dec < 0) ? 1 : 0;

    //abs(dec) :
    dec = dec * pow(-1.0, sign);

    float d1;// integer part of float :
    d1 = (float)(int)dec;

    double d2;// other part of float :
    d2 = (double)(dec - d1);

    int e0 = 0;
    int d1d = d1;
    if ( dec > 0)
    {
        if (d1d >= 1)// d1 will shift right :
        {
            while ( d1d>1)
            {
                d1d = (int)d1d/2;
                e0++;
            }
        }
        else //d2 will shift left :
        {
            int d2d = d2;
            while ((int)d2d != 1)
            {
                d2d *= 2;
                e0--;
            }
        }
    }

    e = e0 + 127;

    // gain x :
    float x0;
    x0 = dec * pow(2.0, -e0) - 1 ;
    x = x0 * pow(2.0, 23) ;

    if (dec == 0)
    {
        x = 0;
        e = 0;
    }

    // merge sign,e,x:
    quint32 result ;
    if (sign == 0)
    {
        result = sign*pow(2.0, 31) + e*pow(2.0, 23) + x;
    } else
    {
        result = (~e) * pow(2.0, 23) + (~x) + 1;
    }

    return result;
}



