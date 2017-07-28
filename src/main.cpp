#include <QtGui>
#include <iostream>
#include "segydata.h"

QTextStream cout(stdout);

int main(int argc, char **argv)
{
    quint32 startTrace;
    quint32 endTrace;
    quint32 startSample;
    quint32 endSample;

    if (argc == 6)
    {
        startTrace = QString(argv[2]).toInt();
        endTrace = QString(argv[3]).toInt();
        startSample = QString(argv[4]).toInt();
        endSample = QString(argv[5]).toInt();
    }

    QString filePath = QString(argv[1]);
    QStringList list1 = filePath.split("/");
    QStringList list2 = list1.at(list1.count()-1).split('.');
    QString newName = list2.at(0);

    QString appe = QString("_SI_%1-%2_%3-%4.txt") .arg(startTrace) .arg(endTrace) .arg(startSample) .arg(endSample);
    newName = newName.append(appe);
    list1.replace(list1.count()-1, newName);
    newName = list1.join("/");

    SegyData segy;
    segy.readSegyFlie(filePath);

    if (startTrace > endTrace)
    {
        cout << "Error:Start trace big than end trace" << endl;
    }
    if ((startTrace < 1) && (endTrace > (quint32)segy.getTraceAmount()))
    {
        cout << "Error:Selected trace number cross the border of the orgin seismic data" << endl
             << QString("origin: traces:%1 | samples:%2").arg(segy.getTraceAmount()).arg(segy.getSamplesPerTrace()) << endl;
    }
    if ((startSample < 0) && (endSample >= (quint32)segy.getSamplesPerTrace()))
    {
        cout << "Error:Selected sample number cross the border of the orgin seismic data" << endl
             << QString("origin: traces:%1 | samples:%2").arg(segy.getTraceAmount()).arg(segy.getSamplesPerTrace()) << endl;
    }

    segy.writeTxtFile(newName, segy.getTraces(), startTrace,endTrace,startSample,endSample);


    return 0;
}
