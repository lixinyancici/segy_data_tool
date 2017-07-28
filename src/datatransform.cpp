#include "datatransform.h"
#include <QtCore>

DataTransform::DataTransform()
{

}

DataTransform::~DataTransform()
{

}

QHash<quint32, QVector<float> >
DataTransform::seismicDataNorm(
        const QHash<quint32, QVector<float> > &seismicData,
        float normValue)
{
    const quint32 traceAmount = seismicData.count();
    const quint16 sampleAmount = seismicData.value(1).count();

    // temp variable for return value
    QHash<quint32, QVector<float> > seismicDataNorm;

    // the amplititude of normlized seismic data, default is 3.0
    float seismicDataAmp = normValue;

    // storage the maxinum value and mininum value of one trace
    float max, min;

    // find the maxinum value's absolute value
    for(quint32 traceIndex = 1; traceIndex <= traceAmount; traceIndex++)
    {
        max = seismicData.value(traceIndex).at(0);
        min = seismicData.value(traceIndex).at(0);

        for(quint16 sampleIndex = 0; sampleIndex < sampleAmount; sampleIndex++)
        {
            if(seismicData.value(traceIndex).at(sampleIndex) > max)
            {
                max = seismicData.value(traceIndex).at(sampleIndex);
            }
            if(seismicData.value(traceIndex).at(sampleIndex) < min)
            {
                min = seismicData.value(traceIndex).at(sampleIndex);
            }
        }

        max = (abs(max) > abs(min)) ? abs(max) : abs(min);

        for(quint16 sampleIndex = 0; sampleIndex < sampleAmount; sampleIndex++)
        {
            seismicDataNorm[traceIndex].append(
                        seismicData.value(traceIndex).at(sampleIndex)
                        / max
                        * seismicDataAmp);
        }
    }

    return seismicDataNorm;
}

// width : canvas's width
// height : canvas's height
// add one zero point to begin and end port
QHash<quint32, QPainterPath>
DataTransform::seismicDataCoordMap(
        const QHash<quint32, QVector<float> > &seismicDataNorm,
        float width,
        float height)
{
    const quint32 traceAmount = seismicDataNorm.count();
    const quint16 sampleAmount = seismicDataNorm.value(1).count();

    // temp variable for return value
    QHash<quint32, QPainterPath> seismicDataCoord;

    // compute the width of per trace in widget (canvas)
    float traceInterval = width / traceAmount;

    // compute the width of per sample in widget (canvas)
    float timeInterval  = height / sampleAmount;

    // coordinate transform
    for(quint32 traceIndex = 1; traceIndex <= traceAmount; traceIndex++)
    {
        float x, y;

        seismicDataCoord[traceIndex].moveTo(0, 0);
        for(quint16 sampleIndex = 0; sampleIndex < sampleAmount; sampleIndex++)
        {
            x = seismicDataNorm.value(traceIndex).at(sampleIndex)
                    * 0.5 * traceInterval;

            y = (sampleIndex + 0.5) * timeInterval;

            seismicDataCoord[traceIndex].lineTo(x, y);                
        }

        seismicDataCoord[traceIndex].lineTo(0, y);

    }

    return seismicDataCoord;
}



QHash<quint32, QPainterPath>
DataTransform::seismicDataCoordMapFill(
        const QHash<quint32, QVector<float> > &seismicDataNorm,
        float width,
        float height)
{
    const quint32 traceAmount = seismicDataNorm.count();
    const quint16 sampleAmount = seismicDataNorm.value(1).count();

    // temp variable for return value
    QHash<quint32, QPainterPath> seismicDataCoordFill;

    // compute the width of per trace in widget (canvas)
    float traceInterval = width / traceAmount;

    // compute the width of per sample in widget (canvas)
    float timeInterval  = height / sampleAmount;

    // coordinate transform and negative value become zero
    for(quint32 traceIndex = 1; traceIndex <= traceAmount; traceIndex++)
    {
        float x, y;

        seismicDataCoordFill[traceIndex].moveTo(0, 0);
        for(quint16 sampleIndex = 0; sampleIndex < sampleAmount; sampleIndex++)
        {          
            if (seismicDataNorm.value(traceIndex).at(sampleIndex) >= 0)
            {
                x = seismicDataNorm.value(traceIndex).at(sampleIndex)
                        * 0.5 * traceInterval;

                y = (sampleIndex + 0.5) * timeInterval;

                seismicDataCoordFill[traceIndex].lineTo(x, y);
            }
            else
            {
                x = 0.0;

                y = (sampleIndex + 0.5) * timeInterval;

                seismicDataCoordFill[traceIndex].lineTo(x, y);
            }
        }

        seismicDataCoordFill[traceIndex].lineTo(0, y);
        seismicDataCoordFill[traceIndex].lineTo(0, 0);
    }

    return seismicDataCoordFill;
}

float
DataTransform::getTraceInterval(
        const QHash<quint32, QVector<float> > &seismicData,
        float width)
{
    const quint32 traceAmount = seismicData.count();

    // the width of per trace in widget (canvas)
    float traceInterval = width / traceAmount;

    return traceInterval;
}
