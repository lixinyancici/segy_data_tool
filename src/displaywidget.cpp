#include "displaywidget.h"

#include <QLabel>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>

DisplayWidget::DisplayWidget(QWidget *parent) :
    QWidget(parent)
{
    renderWidget = new RenderWidget();

    traceLabel = new QLabel("Trace :");
    timeLabel = new QLabel("Time :");
    valueLabel = new QLabel("Value :");

    traceEdit = new QLabel();
    timeEdit = new QLabel();
    valueEdit = new QLabel();

    QHBoxLayout *hlayout = new QHBoxLayout;
    hlayout->addSpacing(25);
    hlayout->addWidget(traceLabel);
    hlayout->addWidget(traceEdit);
    hlayout->addWidget(timeLabel);
    hlayout->addWidget(timeEdit);
    hlayout->addWidget(valueLabel);
    hlayout->addWidget(valueEdit);
    hlayout->addStretch();

    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(renderWidget);
    layout->addSpacing(10);
    layout->addLayout(hlayout);
    layout->setStretchFactor(renderWidget, 20);
    layout->setStretchFactor(hlayout, 1);

    setLayout(layout);
    setWindowTitle("Seismic Display");

    connect(renderWidget, SIGNAL(mousePosChanged(QPoint)),
            this, SLOT(mousePosChanged(QPoint)));
}

DisplayWidget::~DisplayWidget()
{

}

void DisplayWidget::mousePosChanged(QPoint pos)
{
    if (pos.x() > 0 && pos.x() < seismicData.count() && pos.y() > 0 && pos.y() < seismicData.value(1).count())
    {
        traceEdit->setText(QString::number(pos.x()-1));
        timeEdit->setText(QString::number(pos.y()));
        valueEdit->setText(QString::number(seismicData.value(pos.x()+1).at(pos.y())));
    } else
    {
        traceEdit->setText("");
        timeEdit->setText("");
        valueEdit->setText("");
    }

}

void DisplayWidget::setAntialias(bool on)
{
    renderWidget->setAntialias(on);
}

void DisplayWidget::setPaintingType(PaintingType paintingType)
{
    renderWidget->setPaintingType(paintingType);
}

void DisplayWidget::setNormValue(float normValue)
{
    renderWidget->setNormValue(normValue);
}

void DisplayWidget::setSeismicData(
        const QHash<quint32, QVector<float> > &seismicData)
{
    this->seismicData = seismicData;
    renderWidget->setSeismicData(this->seismicData);
}


float DisplayWidget::getNormValue()
{
    return renderWidget->getNormValue();
}
