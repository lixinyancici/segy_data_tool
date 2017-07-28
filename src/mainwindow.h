#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "renderwidget.h"
#include "displaywidget.h"
#include "segydata.h"
#include <QMainWindow>

class QMenuBar;
class QMenu;
class QToolBar;
class QAction;
class QStatusBar;
class QTabWidget;
class QSpinBox;
class QLabel;
class QHBoxLayout;
class QStatusBar;


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void setSeismicDataOrigin(
            const QHash<quint32, QVector<float> > &seismicData);

    void setWaveletDataDisp(
            const QHash<quint32, QVector<float> > &seismicData);

    void setSeismicDataInvered(
            const QHash<quint32, QVector<float> > &seismicData);

    void setSeismicData(
            const QHash<quint32, QVector<float> > &p_seismicData);

    void setWaveletData(
            const QHash<quint32, QVector<float> > &p_waveletData);

    void setSampleInterval(quint16 interval);

    void setSegyPath(QString path);

    void setWaveletPath(QString path);


    void showStatusBarMsg(QString msg);


    void setOriginDataType(QString type);

    QString getOriginDataType();

    void setWaveletDataType(QString type);

    QString getWaveletDataType();


public slots:
//////////// Tool Menu Slots ///////////////////////////////////////////////////
    void slot_setAntialiasOn();
    void slot_setAntialiasOff();
    void slot_paintingTypeWiggle();
    void slot_paintingTypeWiggleFilled();
    void slot_normValue(int norm);
    void slot_normValueDlg();


//////////// File Menu Slots ///////////////////////////////////////////////////
    void slot_loadDataDlg();
    void slot_exportDataDlg();


//////////// Spectral Inverion Menu Slots //////////////////////////////////////
    void slot_spectralInversionDlg();


//////////// About Menu Slots //////////////////////////////////////////////////
    void slot_aboutDlg();


private:
    QHash<quint32, QVector<float> > seismicData;
    QHash<quint32, QVector<float> > seismicDataInv;
    QHash<quint32, QVector<float> > waveletData;  

    bool dataSet, wavSet, dataInvSet;
    QString m_segyPath;
    QString m_waveletPath;
    QString originDataType;
    QString waveletDataType;
    quint16 sampleInterval;

    QMenu *m_file;
    QAction *m_loadData;
    QAction *m_exportData;
    QAction *m_quit;

    QMenu *m_inversion;
    QAction *m_spectralInversion;

    QMenu *m_tool;
    QAction *m_antialiasOn;
    QAction *m_antialiasOff;
    QMenu *m_paintingType;
    QAction *m_wiggle;
    QAction *m_wiggleFilled;
    QAction *m_normValue;

    QMenu *m_about;
    QAction *m_aboutSI;

    QTabWidget *m_tabWidget;
    DisplayWidget *m_originSeismicData;
    DisplayWidget *m_inversedSeismicData;
    DisplayWidget *m_waveletData;

    QToolBar *m_toolBar;
};

#endif // MAINWINDOW_H
