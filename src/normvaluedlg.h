#ifndef NORMVALUEDLG_H
#define NORMVALUEDLG_H

#include <QDialog>

class NormValueDlg : public QDialog
{
public:
    NormValueDlg();
    ~NormValueDlg();

signals:
    void normValueChanged(float norm);

public slots:
    void okCliked();
};

#endif // NORMVALUEDLG_H
