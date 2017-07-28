#ifndef EXPORTDATADLG_H
#define EXPORTDATADLG_H

#include "mainwindow.h"
#include <QDialog>
#include <QString>

class QGridLayout;
class QVBoxLayout;
class QHBoxLayout;
class QGroupBox;
class QCheckBox;
class QLabel;
class QPushButton;
class QComboBox;


class ExportDataDlg : public QDialog
{
    Q_OBJECT

public:
    ExportDataDlg(MainWindow * parent = 0);
    ~ExportDataDlg();

    QString getDataName();
    // 0->txt, 1->sgy
    int whoChecked();

    void setDataBox(bool ori, bool wav, bool inv);

private slots:
    void slot_txtChecked();
    void slot_sgyChecked();
    void slot_waveletSel(QString item);

private:
    MainWindow *parent;

    QGridLayout *glayout;
    QVBoxLayout *vlayout;
    QHBoxLayout *hlayout;
    QHBoxLayout *typeLayout;

    QGroupBox *typeBox;
    QCheckBox *txtItem;
    QCheckBox *sgyItem;

    QPushButton *okBtn;
    QPushButton *cancelBtn;

    QLabel *dataLabel;
    QComboBox *dataBox;
};

#endif // EXPORTDATADLG_H
