#include "exportdatadlg.h"
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QCheckBox>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>

ExportDataDlg::ExportDataDlg(MainWindow *parent) :
    QDialog(parent)
{
    this->parent = parent;
    setWindowTitle("Export Data");

    glayout = new QGridLayout();
    dataLabel = new QLabel("Output Data :  ");
    dataBox = new QComboBox();
    glayout->addWidget(dataLabel, 0, 0, 1, 1);
    glayout->addWidget(dataBox, 0, 1, 1, 4);

    typeBox = new QGroupBox();
    typeBox->setTitle("Output Format");
    typeLayout = new QHBoxLayout(typeBox);
    txtItem = new QCheckBox(".txt", 0);
    sgyItem = new QCheckBox(".sgy", 0);
    typeLayout->addWidget(txtItem);
    typeLayout->addWidget(sgyItem);
    typeLayout->addStretch();

    hlayout = new QHBoxLayout();
    okBtn = new QPushButton("OK");
    cancelBtn = new QPushButton("Cancel");
    hlayout->addStretch();
    hlayout->addWidget(okBtn);
    hlayout->addWidget(cancelBtn);

    vlayout = new QVBoxLayout(this);
    vlayout->addLayout(glayout);
    vlayout->addSpacing(15);
    vlayout->addWidget(typeBox);
    vlayout->addLayout(hlayout);

    connect(txtItem, SIGNAL(clicked()), this, SLOT(slot_txtChecked()));
    connect(sgyItem, SIGNAL(clicked()), this, SLOT(slot_sgyChecked()));

    connect(okBtn, SIGNAL(clicked()), this, SLOT(accept()));
    connect(cancelBtn, SIGNAL(clicked()), this, SLOT(reject()));

    connect(dataBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(slot_waveletSel(QString)));


}


ExportDataDlg::~ExportDataDlg()
{

}

QString ExportDataDlg::getDataName()
{
    return dataBox->currentText();
}

void ExportDataDlg::setDataBox(bool ori, bool wav, bool inv)
{
    if (ori == true)
    {
        dataBox->addItem("Origin Seismic Data");
    }

    if (wav == true)
    {
        dataBox->addItem("Wavelet Data");
    }

    if (inv == true)
    {
        dataBox->addItem("Inverted Seismic Data");
    }
}

int ExportDataDlg::whoChecked()
{
    if (txtItem->isChecked())
    {
        return 0;
    } else
    {
        return 1;
    }
}

void ExportDataDlg::slot_txtChecked()
{
    sgyItem->setChecked(false);
}


void ExportDataDlg::slot_sgyChecked()
{
    txtItem->setChecked(false);
}

void ExportDataDlg::slot_waveletSel(QString item)
{
    if (item == "Wavelet Data")
    {
        if (parent->getWaveletDataType() == "txt")
        {
            sgyItem ->setDisabled(true);
        } else
        {
            sgyItem ->setDisabled(false);
        }
    } else
    {
        if (parent->getOriginDataType() == "txt")
        {
            sgyItem ->setDisabled(true);
        } else
        {
            sgyItem ->setDisabled(false);
        }
    }
}
