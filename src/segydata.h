/*******************************************************************************
 ** 该文件包含二进制文件头结构体定义以及segy数据类的定义，SegyData类代表segy数据，为了
 ** 简化程序，该类中只包含：
 ** 1、文本卷头segyTextReelHead,为QString类，包含从segy文件头部ebcdic编码的3200字节中
 ** 解析出的所有文本信息；
 ** 2、二进制卷头SegyBinaryReelHead结构体，该结构体只包含7项从segy文件中的400字节二进制
 ** 头部分解析出来的信息，其余项本项目用不到固予以忽略；
 ** 3、各道的地震数据seismicDataTraces,它是一个哈希表，每个表项为一个QVector<float>,
 ** 表示一道地震数据，长度有二进制文件头信息计算得到。 由于本项目仅使用叠后地震数据，而
 ** 叠后地震数据所有道长度相同（具有相同的采样点数），采样间隔也相同，所以不需要解析道头
 ** 数据。本项目
 **
 ** AUTHOR: 李新彦
 ** DATE: 2015.04.02
 ******************************************************************************/

#ifndef SEGYREADER_H
#define SEGYREADER_H

#include <QHash>
#include <QVector>
#include <QString>

typedef struct SegyBinaryReelHead {
    quint16 version; // 3501-3502
    qint16 expandReelHeadMark; // 3505-3506
    qint16 traceFixedLengthMark; // 3503-3504

    qint16 traceCompoundType; // 3229-3230
    qint16 sampleValueType; // 3225-3226

    qint16 sampleInterval; // 3217-3218
    qint16 samplesPerTrace; // 3221-3222

    quint32 traceAmount; // get the number of all traces
} SegyBinaryReelHead;


class SegyData
{
public:
    SegyData();
    ~SegyData();

    //==========================================================================
    // if read success return true else false
    bool readSegyFlie(QString filePath);

    // if read wavelet, only m_trace.value(1) have data;
    // and should separate data with single blank space
    bool readTxtFile(QString filePath);

    //==========================================================================
    QString getTextReelHead();

    SegyBinaryReelHead getBinaryReelHead();

    QHash<quint32, QVector<float> > getTraces();

    QHash<quint32, QVector<float> > getTraces(
            quint32 startTrace,
            quint32 endTrace);

    QString getSegyFilePath();

    //==========================================================================
    qint16 getSampleInterval();

    qint16 getSamplesPerTrace();

    qint32 getTraceAmount();

    QString getValueType();

    //==========================================================================
    bool writeSegyFile(
            QString segyWritePath,
            QHash<quint32, QVector<float> > traceData);

    bool writeSegyFile(
            QString segyWritePath,
            QHash<quint32, QVector<float> > traceData,
            quint32 startTrace,
            quint32 endTrace);

    bool writeSegyFile(
            QString segyWritePath,
            QHash<quint32, QVector<float> > traceData,
            quint32 startTrace,
            quint32 endTrace,
            quint32 startSample,
            quint32 endSample);

    bool writeTxtFile(
            QString segyWritePath,
            QHash<quint32, QVector<float> > traceData);

    bool writeTxtFile(
            QString segyWritePath,
            QHash<quint32, QVector<float> > traceData,
            quint32 startTrace,
            quint32 endTrace,
            quint32 startSample,
            quint32 endSample);

private:
    QString m_segyFilePath;

    QString m_textReelHead;

    SegyBinaryReelHead m_binaryReelHead;

    QHash<quint32, QVector<float> > m_traces; //index start from 1, not 0,index 0 don't store data.
    // m_traces.value(1) represent the first trace
};

#endif // SEGYREADER_H
