#ifndef RENDERWIDGET_H
#define RENDERWIDGET_H

#include <QWidget>
#include <QPainterPath>
#include <QPainter>
#include <QHash>

enum PaintingType {wiggle, wiggleFilled, color};

class RenderWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RenderWidget(QWidget *parent = 0);
    ~RenderWidget();

    void setAntialias(bool on);

    void setPaintingType(PaintingType paitingType);

    void setNormValue(float normValue);

    void setSeismicData(
            const QHash<quint32, QVector<float> > &seismicData);


    float getNormValue();

    QSize minimumSizeHint() const;

    QSize sizeHint() const;

signals:
    void mousePosChanged(QPoint pos);

private:
    void paintEvent(QPaintEvent * event);
    void resizeEvent(QResizeEvent * event);
    void mouseMoveEvent(QMouseEvent * event);

private:
    void seismicDataProc();

    void drawTimeScale(QPainter &painter, float timeWidth);

    void drawTraceScale(QPainter &painter, float traceHeight);

    void drawTimeScaleWav(QPainter &painter, float timeWidth);

    QHash<quint32, QVector<float> > m_seismicData;
    QHash<quint32, QPainterPath> m_seismicDataCoord;
    QHash<quint32, QPainterPath> m_seismicDataCoordFill;

    float m_normValue;
    PaintingType m_paintingType;

    quint32 m_traceAmount;
    quint32 m_sampleAmount;

    float m_scaleWidth;

    bool m_antialiasEnable;

    bool m_dataInited;
};

#endif // RENDERWIDGET_H
