#include "mainwindow.h"
#include "segydata.h"
#include "loaddatadlg.h"
#include "exportdatadlg.h"

#include <QMenu>
#include <QIcon>
#include <QLabel>
#include <QMenuBar>
#include <QToolBar>
#include <QAction>
#include <QStatusBar>
#include <QTabWidget>
#include <QSpinBox>
#include <QDialog>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QInputDialog>
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("SI");
    setWindowIcon(QIcon(":/images/image/app.png"));
    resize(800, 500);

    dataSet = false;
    wavSet = false;
    dataInvSet = false;

//////////////// init GUI //////////////////////////////////////////////////////
    m_file = menuBar()->addMenu("&File");
    m_loadData = new QAction(QIcon(":/images/image/open.png"), "&Load Data", 0);
    m_exportData = new QAction(QIcon(":/images/image/export.png"), "&Export Data", 0);
    m_quit = new QAction(QIcon(":/images/image/close.png"), "&Quit", 0);
    m_file->addAction(m_loadData);
    m_file->addAction(m_exportData);
    m_file->addSeparator();
    m_file->addAction(m_quit);

    m_inversion = menuBar()->addMenu("&Inversion");
    m_spectralInversion = new QAction(QIcon(":/images/image/inversion.png"), "&Spectral Inversion", 0);
    m_inversion->addAction(m_spectralInversion);

    m_tool = menuBar()->addMenu("&Tool");
    m_antialiasOn = new QAction("&Open Antialias", 0);
    m_antialiasOff = new QAction("&Close Antialias", 0);
    m_paintingType = new QMenu("&Painting Type");
    m_wiggle = new QAction("&Wiggle", 0);
    m_wiggleFilled = new QAction("&Wiggle Filled", 0);
    m_paintingType->addAction(m_wiggle);
    m_paintingType->addAction(m_wiggleFilled);
    m_normValue= new QAction(QIcon(":/images/image/norm.png"), "&Normlized Value", 0);
    m_tool->addAction(m_antialiasOff);
    m_tool->addAction(m_antialiasOn);
    m_tool->addSeparator();
    m_tool->addMenu(m_paintingType);
    m_tool->addSeparator();
    m_tool->addAction(m_normValue);

    m_about = menuBar()->addMenu("&About");
    m_aboutSI = new QAction(QIcon(":/images/image/about.png"), "&About SI", 0);
    m_about->addAction(m_aboutSI);

///////////// Tool Bar /////////////////////////////////////////////////////////
    m_toolBar = this->addToolBar("&ToolBar");
    m_toolBar->addAction(m_loadData);
    m_toolBar->addAction(m_exportData);
    //m_toolBar->addAction(m_normValue);
    //m_toolBar->addAction(m_spectralInversion);


///////////// Central Widget ///////////////////////////////////////////////////
    m_tabWidget = new QTabWidget();
    m_originSeismicData = new DisplayWidget();
    m_waveletData = new DisplayWidget();
    m_inversedSeismicData = new DisplayWidget();
    m_tabWidget->addTab(m_originSeismicData, "Origin Seismic Data");
    m_tabWidget->addTab(m_waveletData, "Wavelet Data");
    m_tabWidget->addTab(m_inversedSeismicData, "Inversed Seismic Data");

    setCentralWidget(m_tabWidget);

///////////// Status Bar ///////////////////////////////////////////////////////
    statusBar()->showMessage("Ready");

////////////////// Tool Menu Signals and Slots /////////////////////////////////
    connect(m_antialiasOn, SIGNAL(triggered()), this, SLOT(slot_setAntialiasOn()));
    connect(m_antialiasOff, SIGNAL(triggered()), this, SLOT(slot_setAntialiasOff()));
    connect(m_wiggle, SIGNAL(triggered()), this, SLOT(slot_paintingTypeWiggle()));
    connect(m_wiggleFilled, SIGNAL(triggered()), this, SLOT(slot_paintingTypeWiggleFilled()));
    connect(m_normValue, SIGNAL(triggered()), this, SLOT(slot_normValueDlg()));

///////////////// File Menu Signals and Slots //////////////////////////////////
    connect(m_loadData, SIGNAL(triggered()), this, SLOT(slot_loadDataDlg()));
    connect(m_exportData, SIGNAL(triggered()), this, SLOT(slot_exportDataDlg()));
    connect(m_quit, SIGNAL(triggered()), this, SLOT(close()));

//////////////// Spectral Inversion Menu Signals and Slots /////////////////////
    connect(m_spectralInversion, SIGNAL(triggered()), this, SLOT(slot_spectralInversionDlg()));

///////////////// About Menu Signals and Slots /////////////////////////////////
    connect(m_aboutSI, SIGNAL(triggered()), this, SLOT(slot_aboutDlg()));
}

MainWindow::~MainWindow()
{

}


//////////// Public Fuction ////////////////////////////////////////////////////
void MainWindow::showStatusBarMsg(QString msg)
{
    statusBar()->showMessage(msg);
}

void MainWindow::setSeismicDataOrigin(
        const QHash<quint32, QVector<float> > &seismicData)
{
    m_originSeismicData->setSeismicData(seismicData);
}

void MainWindow::setWaveletDataDisp(
        const QHash<quint32, QVector<float> > &seismicData)
{
    m_waveletData->setSeismicData(seismicData);
}

void MainWindow::setSeismicDataInvered(
        const QHash<quint32, QVector<float> > &seismicData)
{
    m_inversedSeismicData->setSeismicData(seismicData);
}

////////////////////////////////////////////////////////////////////////////////
void MainWindow::setSeismicData(
        const QHash<quint32, QVector<float> > &p_seismicData)
{
    seismicData = p_seismicData;
    dataSet = true;
}


void MainWindow::setWaveletData(
        const QHash<quint32, QVector<float> > &p_waveletData)
{
    waveletData = p_waveletData;
    wavSet = true;
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::setSampleInterval(quint16 interval)
{
    sampleInterval = interval;
}


void MainWindow::setSegyPath(QString path)
{
    m_segyPath = path;
}

void MainWindow::setWaveletPath(QString path)
{
    m_waveletPath = path;
}


void MainWindow::setOriginDataType(QString type)
{
    originDataType = type;
}

QString MainWindow::getOriginDataType()
{
    return originDataType;
}

void MainWindow::setWaveletDataType(QString type)
{
    waveletDataType = type;
}

QString MainWindow::getWaveletDataType()
{
    return waveletDataType;
}


//////////// Tool Menu Slots ///////////////////////////////////////////////////
void MainWindow::slot_setAntialiasOn()
{
    if (m_tabWidget->currentIndex() == 0)
    {
        m_originSeismicData->setAntialias(true);
    }else if(m_tabWidget->currentIndex() == 1)
    {
        m_inversedSeismicData->setAntialias(true);
    }
}

void MainWindow::slot_setAntialiasOff()
{
    if (m_tabWidget->currentIndex() == 0)
    {
        m_originSeismicData->setAntialias(false);
    }else if(m_tabWidget->currentIndex() == 1)
    {
        m_inversedSeismicData->setAntialias(false);
    }
}


void MainWindow::slot_paintingTypeWiggle()
{
    if (m_tabWidget->currentIndex() == 0)
    {
        m_originSeismicData->setPaintingType(wiggle);
    }else if(m_tabWidget->currentIndex() == 1)
    {
        m_inversedSeismicData->setPaintingType(wiggle);
    }
}

void MainWindow::slot_paintingTypeWiggleFilled()
{
    if (m_tabWidget->currentIndex() == 0)
    {
        m_originSeismicData->setPaintingType(wiggleFilled);
    }else if(m_tabWidget->currentIndex() == 1)
    {
        m_inversedSeismicData->setPaintingType(wiggleFilled);
    }
}

void MainWindow::slot_normValue(int norm)
{
    if (m_tabWidget->currentIndex() == 0)
    {
        m_originSeismicData->setNormValue((float)norm);
    }else if(m_tabWidget->currentIndex() == 2)
    {
        m_inversedSeismicData->setNormValue((float)norm);
    } else if(m_tabWidget->currentIndex() == 1)
    {
        m_waveletData->setNormValue((float)norm);
    }
}

void MainWindow::slot_normValueDlg()
{
    QDialog *dlg = new QDialog(this);
    dlg->setWindowTitle("Normlized Value");
    dlg->setAttribute(Qt::WA_DeleteOnClose);

    QSpinBox *spinBox = new QSpinBox();

    if (m_tabWidget->currentIndex() == 0)
    {
        spinBox->setValue(m_originSeismicData->getNormValue());
    }else if(m_tabWidget->currentIndex() == 2)
    {
        spinBox->setValue(m_inversedSeismicData->getNormValue());
    }else if(m_tabWidget->currentIndex() == 1)
    {
        spinBox->setValue(m_waveletData->getNormValue());
    }

    spinBox->setRange(0, 20);
    QPushButton *okBtn = new QPushButton("OK");

    QHBoxLayout *spinLayout = new QHBoxLayout();
    spinLayout->addStretch();
    spinLayout->addWidget(okBtn);

    QVBoxLayout *vlayout = new QVBoxLayout(dlg);
    vlayout->addWidget(spinBox);
    vlayout->addSpacing(10);
    vlayout->addLayout(spinLayout);

    connect(okBtn, SIGNAL(clicked()), dlg, SLOT(close()));
    connect(spinBox, SIGNAL(valueChanged(int)), this, SLOT(slot_normValue(int)));

    dlg->resize(80, 70);
    dlg->show();
}

//////////// File Menu Slots ///////////////////////////////////////////////////
void MainWindow::slot_loadDataDlg()
{
    LoadDataDlg *loadData = new LoadDataDlg(this);
    loadData->setAttribute(Qt::WA_DeleteOnClose);
    loadData->setWindowIcon(QIcon(":/images/image/open.png"));

    loadData->show();
}


void MainWindow::slot_exportDataDlg()
{
    ExportDataDlg dlg(this);
    dlg.setWindowIcon(QIcon(":/images/image/export.png"));
    dlg.resize(300, 150);

    if (dataSet == true)
    {
        dlg.setDataBox(true, false, false);
    }
    if (wavSet == true)
    {
        dlg.setDataBox(false, true, false);
    }
    if (dataInvSet == true)
    {
        dlg.setDataBox(false, false, true);
    }

    if (dlg.exec() == QDialog::Accepted)
    {
        statusBar()->showMessage("Busy");

        SegyData segy;
        QString newName;

        if (dlg.getDataName() == "Wavelet Data")
        {
            QString filePath = this->m_waveletPath;
            QStringList list1 = filePath.split("/");
            QStringList list2 = list1.at(list1.count()-1).split('.');
            QString newName = list2.at(0);

            if (waveletDataType == "segy" || waveletDataType == "sgy")
            {
                segy.readSegyFlie(m_waveletPath);

                if (dlg.whoChecked() == 0)
                {
                    newName = newName.append("_SI_WAVELET_DATA.txt");
                    list1.replace(list1.count()-1, newName);
                    filePath = list1.join("/");
                    segy.writeTxtFile(filePath, waveletData);
                } else
                {
                    newName = newName.append("_SI_WAVELET_DATA.sgy");
                    list1.replace(list1.count()-1, newName);
                    filePath = list1.join("/");
                    segy.writeSegyFile(filePath, waveletData);
                }
            } else if (waveletDataType == "txt")
            {
                newName = newName.append("_SI_WAVELET_DATA.txt");
                list1.replace(list1.count()-1, newName);
                filePath = list1.join("/");
                statusBar()->showMessage("Busy");
                segy.writeTxtFile(filePath, waveletData);
            }
        } else
        {
            QString filePath = this->m_segyPath;
            QStringList list1 = filePath.split("/");
            QStringList list2 = list1.at(list1.count()-1).split('.');
            QString newName = list2.at(0);

            if (originDataType == "segy" || originDataType == "sgy")
            {
                segy.readSegyFlie(m_segyPath);

                if (dlg.getDataName() == "Origin Seismic Data")
                {
                   if (dlg.whoChecked() == 0)
                   {
                       newName = newName.append("_SI_ORIGIN_SEISMICDATA.txt");
                       list1.replace(list1.count()-1, newName);
                       filePath = list1.join("/");
                       segy.writeTxtFile(filePath, seismicData);
                   } else
                   {
                       newName = newName.append("_SI_ORIGIN_SEISMICDATA.sgy");
                       list1.replace(list1.count()-1, newName);
                       filePath = list1.join("/");
                       segy.writeSegyFile(filePath, seismicData);
                   }
                } else if (dlg.getDataName() == "Inverted Seismic Data")
                {
                    if (dlg.whoChecked() == 0)
                    {
                        newName = newName.append("_SI_INVERED_SEISMICDATA.txt");
                        list1.replace(list1.count()-1, newName);
                        filePath = list1.join("/");
                        segy.writeTxtFile(filePath, seismicDataInv);
                    } else
                    {
                        newName = newName.append("_SI_INVERED_SEISMICDATA.sgy");
                        list1.replace(list1.count()-1, newName);
                        filePath = list1.join("/");
                        segy.writeSegyFile(filePath, seismicDataInv);
                    }
                }
            } else if (originDataType == "txt")
            {
                if (dlg.getDataName() == "Origin Seismic Data")
                {
                    newName = newName.append("_SI_ORIGIN_SEISMICDATA.txt");
                    list1.replace(list1.count()-1, newName);
                    filePath = list1.join("/");
                    segy.writeTxtFile(filePath, seismicData);
                } else if (dlg.getDataName() == "Inverted Seismic Data")
                {
                    newName = newName.append("_SI_INVERED_SEISMICDATA.txt");
                    list1.replace(list1.count()-1, newName);
                    filePath = list1.join("/");
                    segy.writeTxtFile(filePath, seismicDataInv);
                }
            }
        }
    }

    statusBar()->showMessage("Ready");
}

//////////// Spectral Inverion Menu Slots //////////////////////////////////////
void MainWindow::slot_spectralInversionDlg()
{

}



//////////// About Menu Slots //////////////////////////////////////////////////
void MainWindow::slot_aboutDlg()
{
    QDialog msgBox;
    msgBox.setFixedSize(300, 133);
    msgBox.setWindowIcon(QIcon(":/images/image/about.png"));
    msgBox.setWindowFlags(Qt::WindowCloseButtonHint);

    QLabel *label = new QLabel();
    label->setPixmap(QPixmap(":/images/image/smile.png"));

    QLabel *author  = new QLabel("<h3> Author : <font color=blue>StarrySky</font></h2>");
    QLabel *time    = new QLabel("<h3>   Time : 2015/05/08</h2>");
    QLabel *version = new QLabel("<h3>Version : V0.1 beta</h2>");

    QHBoxLayout *hlayout = new QHBoxLayout();
    QVBoxLayout *vlayout = new QVBoxLayout();

    vlayout->addSpacing(10);
    vlayout->addWidget(author);
    vlayout->addSpacing(10);
    vlayout->addWidget(time);
    vlayout->addSpacing(10);
    vlayout->addWidget(version);
    vlayout->addSpacing(10);

    hlayout->addSpacing(10);
    hlayout->addWidget(label);
    hlayout->addSpacing(10);
    hlayout->addLayout(vlayout);
    hlayout->addSpacing(10);

    //QString str = QString("%1\n%2\n%3")
    //        .arg("+_+ Author: lixinyan")
    //        .arg("+_+ Modified time: 2015.05.06 ")
    //        .arg("+_+ Version 0.1");

    msgBox.setLayout(hlayout);
    //msgBox.setText(str);
    msgBox.setWindowTitle("About");
    msgBox.setWindowIcon(QIcon("ico/about.png"));
    msgBox.exec();
}

