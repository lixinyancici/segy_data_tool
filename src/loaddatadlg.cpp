#include "loaddatadlg.h"
#include "segydata.h"
#include <QGroupBox>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QPainter>
#include <QPaintEvent>
#include <QFileDialog>
#include <QString>
#include <QHash>
#include <QApplication>
#include <QDir>

LoadDataDlg::LoadDataDlg(MainWindow *parent) :
    QDialog(parent)
{
    setWindowTitle("Load Data");
    setWindowIcon(QIcon("ico/open.png"));
    resize(360, 200);

    this->parent = parent;

//----------------------------------------------------------------------------//
    seismicDataBox = new QGroupBox("Seismic Data");
    infoBox = new QGroupBox();

    infoLayout = new QGridLayout(infoBox);
    label1 = new QLabel("  Trace Amount:"); //12
    traceAmountL = new QLabel();
    label2 = new QLabel("  Trace Length:"); //12
    samplePerTraceL = new QLabel();
    label3 = new QLabel("Sample Interval:"); //15
    sampleIntervalL = new QLabel();
    label4 = new QLabel("    Data Format:"); //10
    valueTypeL = new QLabel();
    infoLayout->addWidget(label1, 0, 0);
    infoLayout->addWidget(traceAmountL, 0, 1);
    infoLayout->addWidget(label2, 0, 2);
    infoLayout->addWidget(samplePerTraceL, 0, 3);
    infoLayout->addWidget(label3, 2, 0);
    infoLayout->addWidget(sampleIntervalL, 2, 1);
    infoLayout->addWidget(label4, 2, 2);
    infoLayout->addWidget(valueTypeL, 2, 3);
    infoLayout->setRowMinimumHeight(1, 5);

    fileLayout = new QHBoxLayout();
    fileEdit = new QLabel();
    fileEdit->setFrameStyle(QFrame::Box | QFrame::Sunken);
    fileBtn = new QPushButton("Add...");
    fileLayout->addWidget(fileEdit);
    fileLayout->addWidget(fileBtn);
    fileLayout->setStretchFactor(fileEdit, 4);
    fileLayout->setStretchFactor(fileBtn, 1);

    seismicDataLayout = new QVBoxLayout(seismicDataBox);
    seismicDataLayout->addLayout(fileLayout);
    seismicDataLayout->addWidget(infoBox);

//----------------------------------------------------------------------------//
    waveletBox = new QGroupBox("Wavelet Data");
    wavInfoBox= new QGroupBox();

    wavInfoLayout = new QHBoxLayout(wavInfoBox);
    wavLabel1 = new QLabel("Wavelet Length:");
    wavTraceAmount = new QLabel();
    wavInfoLayout->addWidget(wavLabel1);
    wavInfoLayout->addWidget(wavTraceAmount);
    wavInfoLayout->addStretch();

    wavFileLayout = new QHBoxLayout();
    wavFileEdit = new QLabel();
    wavFileEdit->setFrameStyle(QFrame::Box | QFrame::Sunken);
    wavFileBtn = new QPushButton("Add...");
    wavFileLayout->addWidget(wavFileEdit);    
    wavFileLayout->addWidget(wavFileBtn);
    wavFileLayout->setStretchFactor(wavFileEdit, 4);
    wavFileLayout->setStretchFactor(wavFileBtn, 1);

    waveletDataLayout = new QVBoxLayout(waveletBox);
    waveletDataLayout->addLayout(wavFileLayout);
    waveletDataLayout->addWidget(wavInfoBox);

//----------------------------------------------------------------------------//
    btnLayout = new QHBoxLayout();
    okBtn = new QPushButton("OK");
    btnLayout->addStretch();
    btnLayout->addWidget(okBtn);

//----------------------------------------------------------------------------//
    mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(seismicDataBox);
    mainLayout->addWidget(waveletBox);
    mainLayout->addLayout(btnLayout);

//----------------------------------------------------------------------------//
    connect(fileBtn, SIGNAL(clicked()), this, SLOT(addSeismicFile()));
    connect(wavFileBtn, SIGNAL(clicked()), this, SLOT(addWaveletFile()));
    connect(okBtn, SIGNAL(clicked()), this, SLOT(okBtnClicked()));
}


LoadDataDlg::~LoadDataDlg()
{

}


void LoadDataDlg::okBtnClicked()
{
    this->close();
}

void LoadDataDlg::addSeismicFile()
{
    QString fileName
        = QFileDialog::getOpenFileName(
                this,
                "Select Seismic Data",
                QDir::currentPath(),
                "all file(*.*);;"
                "segy file(*.sgy);;"
                "segy file(*.segy);;"
                "txt file(*.txt)");

    if (fileName.isEmpty())
    {
        return;
    }
    QStringList nameList = fileName.split('/');
    QString absName = nameList.at(nameList.count()-1);

    fileEdit->setText(absName);

//----------------------------------------------------------------------------//
    parent->showStatusBarMsg("Busy");

    QStringList list = fileName.split('.');
    QString typeStr = list.at(list.count()-1);
    SegyData segy;
    if (fileName != "")
    {
        if (typeStr == "segy" || typeStr == "sgy")
        {
            segy.readSegyFlie(fileName);
            sampleIntervalL->setText(QString::number(segy.getSampleInterval())+" us");
            valueTypeL->setText(segy.getValueType());
            parent->setOriginDataType("sgy");
        } else if (typeStr == "txt")
        {
            segy.readTxtFile(fileName);
            sampleIntervalL->setText("- -");
            valueTypeL->setText("- -");
            parent->setOriginDataType("txt");
        }

        QHash<quint32, QVector<float> > traces = segy.getTraces();

        traceAmountL->setText(QString::number(traces.count()));
        samplePerTraceL->setText(QString::number(traces.value(1).count()));

        parent->setSegyPath(fileName);
        parent->setSeismicDataOrigin(traces);
        parent->setSeismicData(traces);
        parent->setSampleInterval(segy.getSampleInterval());
    }

    parent->showStatusBarMsg("Ready");
}



void LoadDataDlg::addWaveletFile()
{
    QString fileName
        = QFileDialog::getOpenFileName(
                this,
                "Select Wavelet Data",
                "f:/",
                "all file(*.*);;"
                "txt file(*.txt);;"
                "segy file(*.sgy);;"
                "segy file(*.segy)");

    if (fileName.isEmpty())
    {
        return;
    }

    QStringList nameList = fileName.split('/');
    QString absName = nameList.at(nameList.count()-1);

    wavFileEdit->setText(absName);

//----------------------------------------------------------------------------//
    parent->showStatusBarMsg("Busy");

    QStringList list = fileName.split('.');
    QString typeStr = list.at(list.count()-1);
    SegyData segy;
    if (fileName != "")
    {
        if (typeStr == "segy" ||typeStr == "sgy")
        {
            segy.readSegyFlie(fileName);
            parent->setWaveletDataType("sgy");            
        } else if (typeStr == "txt")
        {
            segy.readTxtFile(fileName);
            parent->setWaveletDataType("txt");
        }

        QHash<quint32, QVector<float> > traces = segy.getTraces();

        wavTraceAmount->setText(QString::number(traces.value(1).count()));

        parent->setWaveletPath(fileName);
        parent->setWaveletDataDisp(traces);
        parent->setWaveletData(traces);
    }

    parent->showStatusBarMsg("Ready");
}
