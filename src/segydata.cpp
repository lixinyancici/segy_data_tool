/*******************************************************************************
** 该文件是SegyData类的实现文件
**
** AUTHOR: 李新彦
** Date: 2015.04.03
*******************************************************************************/
#include "segydata.h"
#include "floatconversion.h"
#include <QFile>
#include <QBitArray>
#include <math.h>
#include <cmath>
#include <QTextStream>

#include <QDebug>

SegyData::SegyData()
{
}

SegyData::~SegyData()
{
}


bool SegyData::readSegyFlie(QString filePath)
{
    QFile file(filePath);
    QDataStream in(&file);
    in.setByteOrder(QDataStream::BigEndian);

    if(file.open(QFile::ReadOnly))
    {
        //================ read text reel head =================================
        quint8 E2A[256]={
              0,   1,   2,   3, 156,   9, 134, 127, 151, 141, 142,  11,  12,  13,  14,
             15,  16,  17,  18,  19, 157, 133,   8, 135,  24,  25, 146, 143,  28,  29,
             30,  31, 128, 129, 130, 131, 132,  10,  23,  27, 136, 137, 138, 139, 140,
              5,   6,   7, 144, 145,  22, 147, 148, 149, 150,   4, 152, 153, 154, 155,
             20,  21, 158,  26,  32, 160, 161, 162, 163, 164, 165, 166, 167, 168,  91,
             46,  60,  40,  43,  33,  38, 169, 170, 171, 172, 173, 174, 175, 176, 177,
             93,  36,  42,  41,  59,  94,  45,  47, 178, 179, 180, 181, 182, 183, 184,
            185, 124,  44,  37,  95,  62,  63, 186, 187, 188, 189, 190, 191, 192, 193,
            194,  96,  58,  35,  64,  39,  61,  34, 195,  97,  98,  99, 100, 101, 102,
            103, 104, 105, 196, 197, 198, 199, 200, 201, 202, 106, 107, 108, 109, 110,
            111, 112, 113, 114, 203, 204, 205, 206, 207, 208, 209, 126, 115, 116, 117,
            118, 119, 120, 121, 122, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219,
            220, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230, 231, 123,  65,  66,
             67,  68,  69,  70,  71,  72,  73, 232, 233, 234, 235, 236, 237, 125,  74,
             75,  76,  77,  78,  79,  80,  81,  82, 238, 239, 240, 241, 242, 243,  92,
            159,  83,  84,  85,  86,  87,  88,  89,  90, 244, 245, 246, 247, 248, 249,
             48,  49,  50,  51, 52,   53,  54,  55,  56,  57, 250, 251, 252, 253, 254,
            255 };

        quint8 f3200[3200];
        file.seek(0);
        for (quint32 textHeadBitIndex = 0; textHeadBitIndex < 3200; ++textHeadBitIndex)
        {
            in >> f3200[textHeadBitIndex];
        }

        for (quint8 textHeadBitIndexRow = 0; textHeadBitIndexRow < 40; ++textHeadBitIndexRow)
        {
            for (quint8 textHeadBitIndexCol = 0; textHeadBitIndexCol < 80; ++textHeadBitIndexCol)
            {
                m_textReelHead += E2A[f3200[textHeadBitIndexRow*80+textHeadBitIndexCol]];
            }
            if (textHeadBitIndexRow != 39)
            {
                m_textReelHead += "\n";
            }
        }

        //================ read binary reel head ===============================
        file.seek(3216);
        in >> m_binaryReelHead.sampleInterval;
        file.seek(3220);
        in >> m_binaryReelHead.samplesPerTrace;
        file.seek(3224);
        in >> m_binaryReelHead.sampleValueType;
        file.seek(3228);
        in >> m_binaryReelHead.traceCompoundType;
        file.seek(3500);
        in >> m_binaryReelHead.version;
        file.seek(3502);
        in >> m_binaryReelHead.traceFixedLengthMark;
        file.seek(3504);
        in >> m_binaryReelHead.expandReelHeadMark;
        m_binaryReelHead.traceAmount =
            (file.size()-3600)/(m_binaryReelHead.samplesPerTrace*4+240);


        //================ read traces data ====================================
        for (quint32 traceIndex = 1; traceIndex <= m_binaryReelHead.traceAmount; ++traceIndex)
        {
            file.seek(3600+(240+m_binaryReelHead.samplesPerTrace*4)*(traceIndex-1)+240);
            for (quint16 samplesIndex = 1; samplesIndex <= m_binaryReelHead.samplesPerTrace; ++samplesIndex)
            {
                if (m_binaryReelHead.sampleValueType == 1) // 4 bits IBM float
                {

                    quint32 ibm;
                    in >> ibm;

                    m_traces[traceIndex].append( FloatConversion::ibm2decimal(ibm) );

                } else if (m_binaryReelHead.sampleValueType == 5)//4 bits IEEE float
                {
                    float ieee;
                    in >> ieee;
                    m_traces[traceIndex].append( FloatConversion::ieee2Decimal(ieee) );
                }
            }
        }

        file.close();

        m_segyFilePath = filePath;

        return true;
    } else
    {
        return false;
    }
}


bool SegyData::readTxtFile(QString filePath)
{
    m_textReelHead = "This is *.txt file, no header";

    QFile file(filePath);
    QTextStream in(&file);

    if(file.open(QFile::ReadOnly))
    {
        QStringList trace;
        quint16 traceLen;
        QString str;

        for (quint32 traceIndex = 1; in.atEnd() == false; ++traceIndex)
        {
            str = in.readLine();
            trace = str.split(QRegExp("\\s+"));
            traceLen = trace.count();

            if (traceLen > 1)
            {
                for (quint16 i = 0; i < traceLen-1; ++i)
                {
                    m_traces[traceIndex].append(trace.at(i).toFloat());
                }
            } else
            {
                m_traces[1].append(trace.at(0).toFloat());
            }

        }


        file.close();

        m_segyFilePath = filePath;

    } else
    {
        return false;
    }

    m_binaryReelHead.traceAmount = m_traces.count();
    m_binaryReelHead.samplesPerTrace = m_traces.value(1).count();
    m_binaryReelHead.sampleInterval = 66; // 66 indicate that no this data
    m_binaryReelHead.expandReelHeadMark = 66;
    m_binaryReelHead.sampleValueType = 66;
    m_binaryReelHead.traceCompoundType = 66;
    m_binaryReelHead.traceFixedLengthMark = 66;
    m_binaryReelHead.version = 666;

    return true;
}


// this function read text reel head
QString SegyData::getTextReelHead()
{
    return m_textReelHead;
}


SegyBinaryReelHead SegyData::getBinaryReelHead()
{
    return m_binaryReelHead;
}


QHash<quint32, QVector<float> > SegyData::getTraces()
{
    return m_traces;
}



QHash<quint32, QVector<float> >
SegyData::getTraces(quint32 startTrace, quint32 endTrace)
{
    QHash<quint32, QVector<float> > temp_traces;
    for (quint32 traceIndex = startTrace; traceIndex <= endTrace; ++traceIndex)
    {
        temp_traces[traceIndex-startTrace+1] = m_traces.value(traceIndex);
    }
    return temp_traces;
}



qint16 SegyData::getSampleInterval()
{
    return m_binaryReelHead.sampleInterval;
}



qint16 SegyData::getSamplesPerTrace()
{
    return m_binaryReelHead.samplesPerTrace;
}



qint32 SegyData::getTraceAmount()
{
    return m_binaryReelHead.traceAmount;
}


QString SegyData::getValueType()
{
    if (m_binaryReelHead.sampleValueType == 1)
    {
        return QString("4-bit IBM float");
    } else if (m_binaryReelHead.sampleValueType == 5)
    {
        return QString("4-bit IEEE float");
    } else
    {
        return QString("--");
    }
}

QString SegyData::getSegyFilePath()
{
    return m_segyFilePath;
}


// time uints is us
bool SegyData::writeSegyFile(QString segyWritePath,
                             QHash<quint32, QVector<float> > traceData)
{
    QFile wFile(segyWritePath);
    QFile rFile(m_segyFilePath);

    QDataStream in(&rFile);
    in.setByteOrder(QDataStream::BigEndian);
    QDataStream out(&wFile);
    out.setByteOrder(QDataStream::BigEndian);

    if (!wFile.open(QFile::WriteOnly) || !rFile.open(QFile::ReadOnly))
    {
        return false;
    }

    rFile.seek(0);
    wFile.seek(0);

    quint8  f3200;
    for (quint32 i = 0; i < 3200; ++i)
    {
        in >> f3200;
        out << f3200;
    }

    quint8 f400;
    for (quint32 i = 0; i < 400; ++i)
    {
        in >> f400;
        out << f400;
    }

    quint32 traces = m_binaryReelHead.traceAmount;
    quint8  f240;
    for (quint32 traceIndex = 1; traceIndex <= traces; ++traceIndex)
    {
        rFile.seek( 3600
                    + (240+m_binaryReelHead.samplesPerTrace*4)*(traceIndex-1) );

        for (quint32 i = 0; i < 240; ++i)
        {
            in >> f240;
            out << f240;
        }

        if (m_binaryReelHead.sampleValueType == 1)
        {
            quint32 ibmData;
            for (quint16 samplesIndex = 1;
                 samplesIndex <= m_binaryReelHead.samplesPerTrace;
                 ++samplesIndex)
            {
                ibmData = FloatConversion::decimal2ibm(
                            traceData.value(traceIndex)[samplesIndex-1] );
                out << ibmData;
            }
        } else if (m_binaryReelHead.sampleValueType == 5)
        {
            quint32 ieeeData;
            for (quint16 samplesIndex = 1;
                 samplesIndex <= m_binaryReelHead.samplesPerTrace;
                 ++samplesIndex)
            {
                ieeeData = FloatConversion::decimal2ieee(
                            traceData.value(traceIndex)[samplesIndex-1] );
                out << ieeeData;
            }
        }
    }

    wFile.close();
    rFile.close();

    return true;
}



bool SegyData::writeSegyFile(QString segyWritePath,
                             QHash<quint32, QVector<float> > traceData,
                             quint32 startTrace,
                             quint32 endTrace)
{
    QFile wFile(segyWritePath);
    QFile rFile(m_segyFilePath);

    QDataStream in(&rFile);
    in.setByteOrder(QDataStream::BigEndian);
    QDataStream out(&wFile);
    out.setByteOrder(QDataStream::BigEndian);

    if (!wFile.open(QFile::WriteOnly) || !rFile.open(QFile::ReadOnly))
    {
        return false;
    }

    rFile.seek(0);
    wFile.seek(0);

    quint8  f3200;
    for (quint32 i = 0; i < 3200; ++i)
    {
        in >> f3200;
        out << f3200;
    }

    quint8 f400;
    for (quint32 i = 0; i < 400; ++i)
    {
        in >> f400;
        out << f400;
    }

    ///quint32 traces = m_binaryReelHead.traceAmount;
    quint8  f240;
    for (quint32 traceIndex = startTrace; traceIndex <= endTrace; ++traceIndex)
    {
        rFile.seek( 3600
                    + (240+m_binaryReelHead.samplesPerTrace*4)*(traceIndex-1) );

        for (quint32 i = 0; i < 240; ++i)
        {
            in >> f240;
            out << f240;
        }

        if (m_binaryReelHead.sampleValueType == 1)
        {
            quint32 ibmData;
            for (quint16 samplesIndex = 1;
                 samplesIndex <= m_binaryReelHead.samplesPerTrace;
                 ++samplesIndex)
            {
                ibmData = FloatConversion::decimal2ibm(
                            traceData.value(traceIndex)[samplesIndex-1] );
                out << ibmData;
            }
        } else if (m_binaryReelHead.sampleValueType == 5)
        {
            quint32 ieeeData;
            for (quint16 samplesIndex = 1;
                 samplesIndex <= m_binaryReelHead.samplesPerTrace;
                 ++samplesIndex)
            {
                ieeeData = FloatConversion::decimal2ieee(
                            traceData.value(traceIndex)[samplesIndex-1] );
                out << ieeeData;
            }
        }
    }

    wFile.close();
    rFile.close();

    return true;
}



bool SegyData::writeSegyFile(QString segyWritePath,
                             QHash<quint32, QVector<float> > traceData,
                             quint32 startTrace,
                             quint32 endTrace,
                             quint32 startSample,
                             quint32 endSample)
{
    QFile wFile(segyWritePath);
    QFile rFile(m_segyFilePath);

    QDataStream in(&rFile);
    in.setByteOrder(QDataStream::BigEndian);
    QDataStream out(&wFile);
    out.setByteOrder(QDataStream::BigEndian);

    if (!wFile.open(QFile::WriteOnly) || !rFile.open(QFile::ReadOnly))
    {
        return false;
    }

    rFile.seek(0);
    wFile.seek(0);

    quint8  f3200;
    for (quint32 i = 0; i < 3200; ++i)
    {
        in >> f3200;
        out << f3200;
    }

    quint8 f400;
    for (quint32 i = 0; i < 400; ++i)
    {
        in >> f400;
        out << f400;
    }

    //quint32 traces = m_binaryReelHead.traceAmount;
    quint8  f240;
    for (quint32 traceIndex = startTrace; traceIndex <= endTrace; ++traceIndex)
    {
        rFile.seek( 3600
                    + (240+m_binaryReelHead.samplesPerTrace*4)*(traceIndex-1) );

        for (quint32 i = 0; i < 240; ++i)
        {
            in >> f240;
            out << f240;
        }

        if (m_binaryReelHead.sampleValueType == 1)
        {
            quint32 ibmData;
            for (quint16 samplesIndex = startSample;
                 samplesIndex <= endSample;
                 ++samplesIndex)
            {
                ibmData = FloatConversion::decimal2ibm(
                            traceData.value(traceIndex)[samplesIndex-1] );
                out << ibmData;
            }
        } else if (m_binaryReelHead.sampleValueType == 5)
        {
            quint32 ieeeData;
            for (quint16 samplesIndex = startSample;
                 samplesIndex <= endSample;
                 ++samplesIndex)
            {
                ieeeData = FloatConversion::decimal2ieee(
                            traceData.value(traceIndex)[samplesIndex-1] );
                out << ieeeData;
            }
        }
    }

    wFile.close();
    rFile.close();

    return true;
}

bool SegyData::writeTxtFile(
        QString segyWritePath,
        QHash<quint32, QVector<float> > traceData,
        quint32 startTrace,
        quint32 endTrace,
        quint32 startSample,
        quint32 endSample)
{

    QString binOut;

    if ( !((startTrace > 0) &&
         (startTrace < endTrace) &&
         (endTrace <= (quint32)traceData.count()) &&
         (startSample >= 0) &&
         (startSample < endSample) &&
         (endSample < traceData.value(1).count())) )
    {
        return false;
    }

    for (quint32 i = startTrace; i <= endTrace; ++i)
    {
        for (quint16 j = startSample; j < endSample; ++j)
        {
            binOut += QString("%1 ") .arg(traceData.value(i).at(j));
        }
        binOut += "\n";
    }

    QFile outFile(segyWritePath);
    if (outFile.open(QFile::WriteOnly))
    {
        QTextStream fout(&outFile);
        fout << binOut;
        outFile.close();
        return true;
    } else
    {
        return false;
    }
}


bool SegyData::writeTxtFile(
        QString segyWritePath,
        QHash<quint32, QVector<float> > traceData)
{

    QString binOut;

    for (quint32 i = 1; i <= (quint32)traceData.count(); ++i)
    {
        for (quint16 j = 0; j < traceData.value(1).count(); ++j)
        {
            binOut += QString("%1 ") .arg(traceData.value(i).at(j));
        }
        binOut += "\n";
    }

    QFile outFile(segyWritePath);
    if (outFile.open(QFile::WriteOnly))
    {
        QTextStream fout(&outFile);
        fout << binOut;
        outFile.close();
        return true;
    } else
    {
        return false;
    }
}

