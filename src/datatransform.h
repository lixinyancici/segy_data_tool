#ifndef DATATRANSFORM_H
#define DATATRANSFORM_H

#include <QHash>
#include <QVector>
#include <QPainterPath>

class DataTransform
{
public:
    DataTransform();
    ~DataTransform();

    static QHash<quint32, QVector<float> >
    seismicDataNorm(
            const QHash<quint32, QVector<float> > &seismicData,
            float normValue = 1.0);

    // Coordinate transform for wiggle
    static QHash<quint32, QPainterPath>
    seismicDataCoordMap(
            const QHash<quint32, QVector<float> > &seismicDataNorm,
            float width,
            float height);

    // Coordinate transform for wiggleFill
    static QHash<quint32, QPainterPath>
    seismicDataCoordMapFill(
            const QHash<quint32, QVector<float> > &seismicData,
            float width,
            float height);

    float getTraceInterval(const QHash<quint32, QVector<float> > &seismicData,
            float width);
};

#endif // DATATRANSFORM_H
