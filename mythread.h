#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QMainWindow>
#include <QObject>
#include <QTimer>
#include<QSemaphore>
#include <QApplication>
#include <QCoreApplication>
#include <QIODevice>
#include <QFile>
#include <QDebug>
#include <QThread>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <QString>

#define PI 3.14

using  namespace cv;

class MyThread : public QThread
{
    Q_OBJECT
public:
    QImage m_img;
    QSemaphore m_semaphore ;
    MyThread();
    QString filename;
    int type1=0; //伪色彩操作类型
    int type2=0; //去噪操作
    int type3=0; //直方图均衡操作
    int type4=0; //伽马变换
    int type5=0; //对数变换
    int type6=0; //边缘检测操作
    int type7=0;//形态学处理
    int type8=0;//二值化
    int type9=0;//MaxEntropy分割法

    //矩形转换成扇形
    Mat Rect2Sect(const uchar *data ,int rows ,int cols , int angles, int method);
    //双线性插值
    uint BilinearInterpolation2(const uchar *data, int rows ,int cols, float angle_scale,float range_scale);

    int Otsu(cv::Mat& src, cv::Mat& dst, int thresh);
    Mat MaxEntropySegment(Mat src);


signals:
    void sendimg();



protected:
    void run();



public:

    //  read RmLogHeader

    enum DataSizeType : uint8_t
    {
      dataSize8Bit,
      dataSize16Bit,
      dataSize24Bit,
      dataSize32Bit,
    };

    enum PingRateType : uint8_t
    {
      pingRateNormal  = 0x00, // 10Hz max ping rate
      pingRateHigh    = 0x01, // 15Hz max ping rate
      pingRateHighest = 0x02, // 40Hz max ping rate
      pingRateLow     = 0x03, // 5Hz max ping rate
      pingRateLowest  = 0x04, // 2Hz max ping rate
      pingRateStandby = 0x05, // Disable ping
    };

    struct RmLogHeader
    {
    public:
      unsigned       fileHeader;   // Fixed 4 byte header ident type
      unsigned       sizeHeader;   // Size of this structure
      char           source[16];   // 12 character max source identifier
      unsigned short version;      // File version type
      unsigned short encryption;   // Encryption style (0 = none)
      qint64         key;          // Possibly saved encryption key (0 otherwise)
      double         time;         // Time of file creation
    };


    struct RmLogItem
    {
    public:
      unsigned       itemHeader;   // Fixed 4 byte header byte
      unsigned       sizeHeader;   // Size of this structure
      unsigned short type;         // Identifer for the contained data type
      unsigned short version;      // Version for the data type
      double         time;         // Time item creation
      unsigned short compression;  // Compression type 0 = none, 1 = qCompress
      unsigned       originalSize; // Size of the payload prior to any compression
      unsigned       payloadSize;  // Size of the following payload
    };

    #pragma pack(push, 1)
    struct OculusMessageHeader
    {
    public:
      uint16_t oculusId;         // Fixed ID 0x4f53
      uint16_t srcDeviceId;      // The device id of the source
      uint16_t dstDeviceId;      // The device id of the destination
      uint16_t msgId;            // Message identifier
      uint16_t msgVersion;
      uint32_t payloadSize;      // The size of the message payload (header not included)
      uint16_t spare2;
    };

    struct OculusSimpleFireMessage
    {
    public:
      OculusMessageHeader head;     // The standard message header

      uint8_t masterMode;           // mode 0 is flexi mode, needs full fire message (not available for third party developers)
                                    // mode 1 - Low Frequency Mode (wide aperture, navigation)
                                    // mode 2 - High Frequency Mode (narrow aperture, target identification)
      PingRateType pingRate;        // Sets the maximum ping rate.
      uint8_t networkSpeed;         // Used to reduce the network comms speed (useful for high latency shared links)
      uint8_t gammaCorrection;      // 0 and 0xff = gamma correction = 1.0
                                    // Set to 127 for gamma correction = 0.5
      uint8_t flags;                // bit 0: 0 = interpret range as percent, 1 = interpret range as meters
                                    // bit 1: 0 = 8 bit data, 1 = 16 bit data
                                    // bit 2: 0 = wont send gain, 1 = send gain
                                    // bit 3: 0 = send full return message, 1 = send simple return message
      double range;                 // The range demand in percent or m depending on flags
      double gainPercent;           // The gain demand
      double speedOfSound;          // ms-1, if set to zero then internal calc will apply using salinity
      double salinity;              // ppt, set to zero if we are in fresh water
    };

    // -----------------------------------------------------------------------------
    struct OculusSimplePingResult
    {
    public:
        OculusSimpleFireMessage fireMessage;
        uint32_t pingId; 			/* An incrementing number */
        uint32_t status;
        double frequency;				/* The acoustic frequency (Hz) */
        double temperature;				/* The external temperature (deg C) */
        double pressure;				/* The external pressure (bar) */
        double speeedOfSoundUsed;		/* The actual used speed of sound (m/s). May be different to the speed of sound set in the fire message */
        uint32_t pingStartTime;
        DataSizeType dataSize; 			/* The size of the individual data entries */
        double rangeResolution;			/* The range in metres corresponding to a single range line */
        uint16_t nRanges;			/* The number of range lines in the image*/
        uint16_t nBeams;			/* The number of bearings in the image */
        uint32_t imageOffset; 		/* The offset in bytes of the image data from the start of the network message */
        uint32_t imageSize; 		/* The size in bytes of the image data */
        uint32_t messageSize; 		/* The total size in bytes of the network message */
        // *** NOT ADDITIONAL VARIABLES BEYOND THIS POINT ***
        // There will be an array of bearings (shorts) found at the end of the message structure
        // Allocated at run time
        // short bearings[];
        // The bearings to each of the beams in 0.01 degree resolution
    };
    #pragma pack(pop)

    QFile   m_file;            // The file
    QString m_fileName;        // The name of the last file opened
    quint64 m_fileSize;        // The size of the file being read
    quint64 m_totalBytes;      // The amount of data read so far from the file

    static const unsigned s_fileHeader = 0x11223344;

    static const unsigned s_itemHeader = 0xaabbccdd;

    static const unsigned s_oculusId = 0x4F53;

    int  backSize = 122;

public:
    bool OpenFile(QString file);

    RmLogHeader header;
    RmLogItem header2;
    OculusSimplePingResult data_first_frame ;

    OculusSimplePingResult getnDataFrame ;

    cv::Mat Array2Mat(const uchar *array, int row, int col);
    QImage MatToQImage(const cv::Mat& mat);


};

#endif // MYTHREAD_H
