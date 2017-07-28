#ifndef LOADDATADLG_H
#define LOADDATADLG_H

#include "mainwindow.h"
#include <QDialog>

class QGroupBox;
class QGridLayout;
class QVBoxLayout;
class QLineEdit;
class QHBoxLayout;
class QPushButton;
class QLabel;

class LoadDataDlg : public QDialog
{
    Q_OBJECT

public:
    LoadDataDlg(MainWindow *parent = 0);
    ~LoadDataDlg();

private slots:
    void okBtnClicked();
    void addSeismicFile();
    void addWaveletFile();

private:
    MainWindow *parent;

    QGroupBox *seismicDataBox;
    QGroupBox *infoBox;
    QHBoxLayout *fileLayout;
    QLabel *fileEdit;
    QPushButton *fileBtn; //
    QGridLayout *infoLayout;
    QLabel *label1;
    QLabel *traceAmountL;
    QLabel *label2;
    QLabel *samplePerTraceL;
    QLabel *label3;
    QLabel *sampleIntervalL;
    QLabel *label4;
    QLabel *valueTypeL; //
    QVBoxLayout *seismicDataLayout;

    QGroupBox *waveletBox;
    QGroupBox *wavInfoBox;
    QHBoxLayout *wavFileLayout;
    QLabel *wavFileEdit;
    QPushButton *wavFileBtn;
    QHBoxLayout *wavInfoLayout;
    QLabel *wavLabel1;
    QLabel *wavTraceAmount;
    QVBoxLayout *waveletDataLayout;

    QHBoxLayout *btnLayout;
    QPushButton *okBtn;

    QVBoxLayout *mainLayout;
};

#endif // LOADDATADLG_H
