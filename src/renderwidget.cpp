#include "renderwidget.h"
#include "datatransform.h"

#include <QHash>
#include <QResizeEvent>
#include <QMouseEvent>
#include <QPainter>

RenderWidget::RenderWidget(QWidget *parent) :
    QWidget(parent)
{
    setMouseTracking(true);

    m_scaleWidth = 25;
    m_antialiasEnable = true;
    m_normValue = 3.0;
    m_paintingType = wiggleFilled;
    m_dataInited = false;
}


RenderWidget::~RenderWidget()
{

}

void RenderWidget::setAntialias(bool on)
{
    m_antialiasEnable = on;
    update();
}


void RenderWidget::setPaintingType(PaintingType paitingType)
{
    m_paintingType = paitingType;
    update();
}


void RenderWidget::setNormValue(float normValue)
{
    m_normValue = normValue;
    seismicDataProc();
    update();
}

float RenderWidget::getNormValue()
{
    return m_normValue;
}


void RenderWidget::setSeismicData(
        const QHash<quint32, QVector<float> > &seismicData)
{    
    m_seismicData = seismicData;
    m_traceAmount = (quint32)m_seismicData.count();
    m_sampleAmount = (quint32)m_seismicData.value(1).count();
    seismicDataProc();
    m_dataInited = true;

    update();
}


QSize RenderWidget::minimumSizeHint() const
{
    return QSize(300, 200);
}

QSize RenderWidget::sizeHint() const
{
    return QSize(600,300);
}


void RenderWidget::seismicDataProc()
{
    QHash<quint32, QVector<float> > seismicDataNorm;
    if (m_seismicData.count() != 1)
    {
        seismicDataNorm = DataTransform::seismicDataNorm(
                    m_seismicData, m_normValue);

        m_seismicDataCoord = DataTransform::seismicDataCoordMap(
                    seismicDataNorm,
                    (float)this->width()-m_scaleWidth,
                    (float)this->height()-m_scaleWidth);

        m_seismicDataCoordFill = DataTransform::seismicDataCoordMapFill(
                    seismicDataNorm,
                    (float)this->width()-m_scaleWidth,
                    (float)this->height()-m_scaleWidth);
    } else
    {
        seismicDataNorm = DataTransform::seismicDataNorm(
                    m_seismicData, (float)width() / 20000 * m_normValue);

        m_seismicDataCoord = DataTransform::seismicDataCoordMap(
                    seismicDataNorm,
                    (float)this->width(),
                    (float)this->height());

        m_seismicDataCoordFill = DataTransform::seismicDataCoordMapFill(
                    seismicDataNorm,
                    (float)this->width(),
                    (float)this->height());
    }
}


void RenderWidget::paintEvent( QPaintEvent * event)
{
    QPainter painter(this);
    painter.fillRect(event->rect(), QBrush(Qt::white));

    if (m_antialiasEnable == false)
    {
        painter.setRenderHint(QPainter::Antialiasing, false);
    } else
    {
        painter.setRenderHint(QPainter::Antialiasing, true);
    }


    if (m_dataInited == false)
    {
        QWidget::paintEvent(event);
    } else
    {
        if (m_seismicData.count() != 1)
        {
            painter.translate(m_scaleWidth, m_scaleWidth);

            painter.save();
            drawTimeScale(painter, m_scaleWidth);
            painter.restore();

            painter.save();
            drawTraceScale(painter, m_scaleWidth);
            painter.restore();

            float traceInterval = float(this->width()-m_scaleWidth) / m_traceAmount;

            QPainterPath path, pathFill;

            for (quint32 traceIndex = 1; traceIndex <= m_traceAmount; ++traceIndex)
            {
                path = m_seismicDataCoord.value(traceIndex);
                pathFill = m_seismicDataCoordFill.value(traceIndex);
                double offset = (traceIndex - 1)* traceInterval + 0.5 * traceInterval;

                painter.save();
                if (traceIndex == 1 || traceIndex == m_traceAmount)
                {
                    painter.translate(offset*3.0, 0);
                } else
                {
                    painter.translate(offset, 0);
                }
                painter.drawPath(path);
                if (m_paintingType == wiggle)
                {

                } else if (m_paintingType == wiggleFilled)
                {
                    painter.fillPath(pathFill, QBrush(Qt::black));
                }
                painter.restore();
            }
        } else
        {
            painter.translate(m_scaleWidth, 0);

            painter.save();
            drawTimeScaleWav(painter, m_scaleWidth);
            painter.restore();

            painter.save();
            painter.drawLine(m_scaleWidth, -5, width(), -5);
            painter.restore();

            QPainterPath path, pathFill;

            path = m_seismicDataCoord.value(1);
            pathFill = m_seismicDataCoordFill.value(1);

            double offset = (double)this->width() * 0.5;

            painter.save();

            painter.translate(offset, 0);
            painter.drawPath(path);

            painter.save();
            QPen pen(Qt::black, 1, Qt::DotLine, Qt::RoundCap, Qt::RoundJoin);
            painter.setPen(pen);
            painter.drawLine(0, 0, 0, this->height());
            painter.restore();

            if (m_paintingType == wiggle)
            {

            } else if (m_paintingType == wiggleFilled)
            {
                painter.fillPath(pathFill, QBrush(Qt::black));
            }

            painter.restore();
        }
    }
}


void RenderWidget::resizeEvent(QResizeEvent * event)
{
    seismicDataProc();
    QWidget::resizeEvent(event);
}


void RenderWidget::mouseMoveEvent(QMouseEvent * event)
{
    QPointF pos = event->posF();

    double x = pos.x() - 10;
    double y = pos.y() - 10;

    float traceInterval = (float)(this->width()-m_scaleWidth) / m_traceAmount;
    float sampleInterval = (float)(this->height()-m_scaleWidth) / m_sampleAmount;

    int intX = (int)(x / traceInterval - 1.5);
    int intY = (int)(y / sampleInterval) + 1;

    emit mousePosChanged(QPoint(intX, intY));

    QWidget::mouseMoveEvent(event);
}


void RenderWidget::drawTimeScale(QPainter &painter, float timeWidth)
{
    float width = timeWidth;
    float height = (float)this->height() - timeWidth;

    quint32 step;
    if (m_sampleAmount >= 300)
    {
        step = 40;
    } else if (m_sampleAmount >= 150)
    {
        step = 30;
    } else if (m_sampleAmount >= 100)
    {
        step = 20;
    } else if (m_sampleAmount >= 50)
    {
        step = 10;
    } else if (m_sampleAmount >= 20)
    {
        step = 5;
    } else if (m_sampleAmount >= 10)
    {
        step = 2;
    } else
    {
        step = 1;
    }

    painter.drawLine(0, 0, 0, height);

    for (int sampleIndex = 0; sampleIndex < (int)m_sampleAmount; ++sampleIndex)
    {
        if (sampleIndex%step == 0 || sampleIndex == m_sampleAmount-1)
        {
            painter.drawLine(
                        -0.4*width, sampleIndex*(height/m_sampleAmount),
                        0, sampleIndex*(height/m_sampleAmount));

            painter.drawText(
                        -timeWidth,
                        sampleIndex*(height/m_sampleAmount)-5,
                        QString::number(sampleIndex));
        }
    }
}


void RenderWidget::drawTraceScale(QPainter &painter, float traceHeight)
{
    float width = (float)this->width();
    float height = traceHeight;

    float traceInterval = float(this->width()-m_scaleWidth) / m_traceAmount;

    quint32 step;
    if (m_traceAmount >= 300)
    {
        step = 40;
    } else if (m_traceAmount >= 150)
    {
        step = 30;
    } else if (m_traceAmount >= 100)
    {
        step = 20;
    } else if (m_traceAmount >= 50)
    {
        step = 10;
    } else if (m_traceAmount >= 20)
    {
        step = 5;
    } else if (m_traceAmount >= 10)
    {
        step = 2;
    } else
    {
        step = 1;
    }


    painter.save();

    painter.drawLine(0.5 * traceInterval*3.0, -5, width, -5);

    for (quint32 traceIndex = 1; traceIndex <= m_traceAmount; ++traceIndex)
    {
        painter.save();

        double offset = (traceIndex - 1)* traceInterval + 0.5 * traceInterval;

        if (traceIndex == 1)
        {
            painter.translate(offset*3.0, 0);
        } else
        {
            painter.translate(offset+0.5 * traceInterval*(3.0-1), 0);
        }

        if (traceIndex%step == 0 || traceIndex == 1)
        {
            painter.drawLine(
                        0, -0.5*height,
                        0, -5);

            if (traceIndex < 10)
            {
                painter.drawText(
                            0-4,
                            -0.6*height,
                            QString::number(traceIndex));
            } else if (traceIndex < 100)
            {
                painter.drawText(
                            0-7,
                            -0.6*height,
                            QString::number(traceIndex));
            } else
            {
                painter.drawText(
                            0-20,
                            -0.6*height,
                            QString::number(traceIndex));
            }

        }

        painter.restore();
    }

    painter.restore();
}

void RenderWidget::drawTimeScaleWav(QPainter &painter, float timeWidth)
{
    float width = timeWidth;
    float height = (float)this->height();

    quint32 step;
    if (m_sampleAmount >= 300)
    {
        step = 40;
    } else if (m_sampleAmount >= 150)
    {
        step = 30;
    } else if (m_sampleAmount >= 100)
    {
        step = 20;
    } else if (m_sampleAmount >= 50)
    {
        step = 10;
    } else if (m_sampleAmount >= 20)
    {
        step = 5;
    } else if (m_sampleAmount >= 10)
    {
        step = 2;
    } else
    {
        step = 1;
    }

    painter.drawLine(0, 0, 0, height);

    for (int sampleIndex = 0; sampleIndex < (int)m_sampleAmount; ++sampleIndex)
    {
        if (sampleIndex%step == 0 || sampleIndex == m_sampleAmount-1)
        {

            painter.drawLine(
                        -0.4*width, sampleIndex*(height/m_sampleAmount),
                        0, sampleIndex*(height/m_sampleAmount));

            painter.save();
            painter.setPen(Qt::DotLine);
            painter.drawLine(
                        0, sampleIndex*(height/m_sampleAmount),
                        this->width()-timeWidth, sampleIndex*(height/m_sampleAmount));
            painter.restore();

            if (sampleIndex == 0)
            {
                painter.drawText(
                            -timeWidth,
                            sampleIndex*(height/m_sampleAmount)+10,
                            QString(" %1").arg(sampleIndex));
            } else if (sampleIndex < 100)
            {
                painter.drawText(
                            -timeWidth,
                            sampleIndex*(height/m_sampleAmount),
                            QString::number(sampleIndex));
            } else
            {
                painter.drawText(
                            -timeWidth,
                            sampleIndex*(height/m_sampleAmount)-5,
                            QString::number(sampleIndex));
            }


        }
    }
}
