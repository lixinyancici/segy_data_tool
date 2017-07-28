#ifndef FLOATCONVERSION_H
#define FLOATCONVERSION_H

#include <QtCore>

class FloatConversion
{
public:
    FloatConversion();
    ~FloatConversion();

    static float ibm2decimal( quint32 ibm);

    static float ieee2Decimal( quint32 ieee );

    static quint32 decimal2ibm( float dec );

    static quint32 decimal2ieee( float dec );
};

#endif // FLOATCONVERSION_H
