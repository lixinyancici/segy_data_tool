#ifndef DISPLAYWIDGET_H
#define DISPLAYWIDGET_H

#include <QWidget>
#include "renderwidget.h"

class QLabel;
class QTextEdit;

class DisplayWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DisplayWidget(QWidget *parent = 0);
    ~DisplayWidget();

    void setAntialias(bool on);

    void setPaintingType(PaintingType paintingType = wiggleFilled);

    void setNormValue(float normValue = 3.0);

    void setSeismicData(
            const QHash<quint32, QVector<float> > &seismicData);

    float getNormValue();

public slots:
    void mousePosChanged(QPoint pos);

private:
    RenderWidget *renderWidget;

    QHash<quint32, QVector<float> > seismicData;

    QLabel *traceLabel;
    QLabel *timeLabel;
    QLabel *valueLabel;

    QLabel *traceEdit;
    QLabel *timeEdit;
    QLabel *valueEdit;

};

#endif // DISPLAYWIDGET_H
