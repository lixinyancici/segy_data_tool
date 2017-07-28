/*******************************************************************************
 ** ���ļ������������ļ�ͷ�ṹ�嶨���Լ�segy������Ķ��壬SegyData�����segy���ݣ�Ϊ��
 ** �򻯳��򣬸�����ֻ������
 ** 1���ı���ͷsegyTextReelHead,ΪQString�࣬������segy�ļ�ͷ��ebcdic�����3200�ֽ���
 ** �������������ı���Ϣ��
 ** 2�������ƾ�ͷSegyBinaryReelHead�ṹ�壬�ýṹ��ֻ����7���segy�ļ��е�400�ֽڶ�����
 ** ͷ���ֽ�����������Ϣ���������Ŀ�ò��������Ժ��ԣ�
 ** 3�������ĵ�������seismicDataTraces,����һ����ϣ��ÿ������Ϊһ��QVector<float>,
 ** ��ʾһ���������ݣ������ж������ļ�ͷ��Ϣ����õ��� ���ڱ���Ŀ��ʹ�õ���������ݣ���
 ** ��������������е�������ͬ��������ͬ�Ĳ������������������Ҳ��ͬ�����Բ���Ҫ������ͷ
 ** ���ݡ�����Ŀ
 **
 ** AUTHOR: ������
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
