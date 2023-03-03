#include "mythread.h"

#include <cstring>

MyThread::MyThread()
{

}

//读取文件头

bool MyThread::OpenFile(QString file)
{
    // Keep the last file name
    m_fileName = file;
    qDebug()<<m_fileName;

    m_file.setFileName(m_fileName);

    if (m_file.open(QIODevice::ReadOnly ))
    {
        qDebug()<<"open";

        m_fileSize = m_file.size();

        //qDebug()<<"m_fileSize";

        if (m_fileSize < sizeof(RmLogHeader))
            return false;

        if (m_fileSize < sizeof(RmLogItem))
            return false;

        // Check the file header
        //  RmLogHeader header;

        m_totalBytes = m_file.read((char*)&header, sizeof(RmLogHeader));

        if (m_totalBytes != sizeof(RmLogHeader))
            return false; //size不匹配

        if (header.fileHeader != s_fileHeader)
            return false; //内容不匹配

        if (header.sizeHeader != sizeof(RmLogHeader))
            return  false; //size不匹配
        qDebug()<< header.fileHeader;
        //return true;

        /*********************************************************************************************/

        // Check the item header

        //RmLogItem header2;

        m_totalBytes = m_file.read((char*)&header2, sizeof(RmLogItem));

        if (m_totalBytes != sizeof(RmLogItem))
            return false; //size不匹配

        if (header2.itemHeader != s_itemHeader)
            return false; //内容不匹配

        if (header2.sizeHeader != sizeof(RmLogItem))
            return  false; //size不匹配
        qDebug()<< header2.itemHeader;
        //  return true;


        /*********************************************************************************************/
        //第一ping数据

        //OculusSimplePingResult data_first_frame ;

        m_totalBytes = m_file.read((char*)&data_first_frame, sizeof(OculusSimplePingResult));

        if (m_totalBytes != sizeof(OculusSimplePingResult))
        {
            return false; //size不匹配
        }
        qDebug()<< data_first_frame.fireMessage.head.msgId;

        qDebug()<< "payloadSize" <<data_first_frame.fireMessage.head.payloadSize;

//        int range_num =data_first_frame.nRanges;

//        int beam_num = data_first_frame.nBeams;

//        qDebug()<<range_num<< ' '<<beam_num;

//        int offset =data_first_frame.imageOffset - 122;
//        qDebug()<<"offset"<<offset;
//        uchar tmp[offset];

//        int a = m_file.read((char*)&tmp, sizeof(tmp));
//        qDebug()<<"here1";
//        uchar image[range_num*beam_num] ;
//        int b = m_file.read((char*)&image, sizeof(image));
//        qDebug()<<"here2";

    }

}

void MyThread::run()
{
    bool flag;
    //qDebug()<<"run";
    flag = OpenFile(filename);
    qDebug()<<flag;
   // qDebug()<<"run2";



    int range_num =data_first_frame.nRanges;

    int beam_num = data_first_frame.nBeams;

    qDebug()<<range_num<< ' '<<beam_num;

    int offset =data_first_frame.imageOffset - 122;
    qDebug()<<"offset"<<offset;
    uchar tmp[offset];

    int a = m_file.read((char*)&tmp, sizeof(tmp));
  //  qDebug()<<"here1";
    uchar image[range_num*beam_num] ;
    int b = m_file.read((char*)&image, sizeof(image));
  //  qDebug()<<"here2";

    Mat img= Rect2Sect(image,range_num,beam_num,60,1);
    rotate(img, img, ROTATE_180);
    //Mat img = Array2Mat(image,range_num,beam_num);

    m_img = MatToQImage(img);
    emit sendimg();



    int i = 1;
    while (i)
    {
       // qDebug()<<"whilie";
        m_semaphore.acquire();    //加入信号量【0,1】      信号量。accure（）

        // Check the item header

        m_totalBytes = m_file.read((char*)&header2, sizeof(RmLogItem));

        if (m_totalBytes != sizeof(RmLogItem))
        {

            qDebug() << "false,size不匹配"; //size不匹配
            break;
        }

        if (header2.itemHeader != s_itemHeader)
        {
            qDebug() <<header2.itemHeader<<' '<<s_itemHeader;
            qDebug() << "false,内容不匹配"; //内容不匹配
            break;
            // return false; //内容不匹配
        }
        if (header2.sizeHeader != sizeof(RmLogItem))
        {
            qDebug() << "size不匹配"; //size不匹配

            // return  false; //size不匹配
            qDebug()<< header2.itemHeader;

            break;
            //  return true;
        }
        /****************************************************************************************/
        //第N ping数据
        //  OculusSimplePingResult getnDataFrame ;

        m_totalBytes = m_file.read((char*)&getnDataFrame, sizeof(OculusSimplePingResult));

        if (m_totalBytes != sizeof(OculusSimplePingResult))
        {
            qDebug() << "false,size不匹配"; //size不匹配
            break;
            // return false; //size不匹配
        }

        int range_num2 =getnDataFrame.nRanges;

        int beam_num2 = getnDataFrame.nBeams;

        int offset2 =getnDataFrame.imageOffset - 122;

        uchar tmp2[offset2];

        int a2 = m_file.read((char*)&tmp2, sizeof(tmp2));
        //qDebug()<<"here1";
        uchar image2[range_num*beam_num] ;
        int b2 = m_file.read((char*)&image2, sizeof(image2));
       // qDebug()<<"here2";

/*ostu

        if(type8==1){
            qDebug()<<"ostu";
            Mat img = Array2Mat(image2,range_num,beam_num);


            if (img.channels() > 1)
                cv::cvtColor(img, img, cv::COLOR_RGB2GRAY );

            int thresh=0;
            thresh=Otsu(img , img, thresh); //Otsu
            std::cout << "Mythresh=" << thresh << std::endl;

            int row = img.rows;
            int col = img.cols;
            uchar image2[row*col] ;


        }



*/

        Mat img2= Rect2Sect(image2,range_num,beam_num,60,1);
        rotate(img2, img2, ROTATE_180);
    //    Mat img2 = Array2Mat(image2,range_num2,beam_num2);





        if(type2 == 1)
        {
            qDebug()<<"均值滤波";
            blur(img2, img2 , Size(3, 3)); //均值滤波
        }
        else if(type2 == 2){
            qDebug()<<"GaussianBlur";
            GaussianBlur(img2,img2, Size(3, 3), 0, 0); //高斯滤波
        }
        else if(type2 == 3){
            qDebug()<<"medianBlur";
            medianBlur(img2,img2, 3);
        }

        if(type3 == 1)
        {
            qDebug()<<"Equalized";
            equalizeHist(img2, img2); // 直方图均衡
        }

        if(type4 == 1)              //伽马变换
        {
            qDebug()<<"伽马变换";
            int gamma=50;
           // Mat dst;

          //  if (gamma < 0)
           //     dst=img2;

            Mat lookUpTable(1, 256, CV_8U);                                    // 查找表
            uchar* p = lookUpTable.ptr();
            for (int i = 0; i < 256; ++i)
                p[i] = saturate_cast<uchar>(pow(i / 255.0, gamma/100.0)*255.0);      // pow()是幂次运算

            LUT(img2, lookUpTable, img2);                                   // LUT

        }
        if(type5 == 1)              //伽马变换
        {
            qDebug()<<"对数变换";
            int c=180;
           // Mat dst;

          //  if (gamma < 0)
           //     dst=img2;

            Mat lookUpTable(1, 256, CV_8U);                                    // 查找表
            uchar* p = lookUpTable.ptr();
            for (int i = 0; i < 256; ++i)
                p[i] = saturate_cast<uchar>((c/100.0)*log(1 + i / 255.0)*255.0);      // pow()是幂次运算

            LUT(img2, lookUpTable, img2);                                   // LUT

        }

        if(type6 ==1){              //Canny算子
            qDebug()<<"canny算子";
            cv::Canny(img2, img2, 50, 150);
        }
        else if (type6==2) {        //laplacian算子
            qDebug()<<"laplacian算子";
            // Laplacian检测
            cv::Laplacian(img2, img2, CV_16S);
            //将图片转化成为8位图形进行显示
            cv::convertScaleAbs(img2, img2);
        }



        if(type8==1){
            qDebug()<<"ostu二值化";


            if (img2.channels() > 1)
                cv::cvtColor(img2, img2, cv::COLOR_RGB2GRAY );

            cv::threshold(img2, img2, 50, 255, THRESH_BINARY );

//           // cv::Mat dst,dst2;
//            int thresh=0;
//           // double t2 = (double)cv::getTickCount();
//            thresh=Otsu(img2 , img2, thresh); //Otsu
//            std::cout << "Mythresh=" << thresh << std::endl;
//           // t2 = (double)cv::getTickCount() - t2;
//           // double time2 = (t2 *1000.) / ((double)cv::getTickFrequency());
//           // std::cout << "my_process=" << time2 << " ms. " << std::endl << std::endl;
//            //double  Otsu = 0;

//           // Otsu=cv::threshold(src, dst2, Otsu, 255, THRESH_BINARY + THRESH_OTSU);
//           // std::cout << "OpenCVthresh=" << Otsu << std::endl;
        }
        else if (type8==2) {
            qDebug()<<"MaxEntropy分割法";

            img2=MaxEntropySegment(img2);


        }


        if(type7==1){               //腐蚀
             qDebug()<<"腐蚀";

             int erosion_type ;
             erosion_type = MORPH_RECT;
             //erosion_type = MORPH_CROSS;
             //erosion_type = MORPH_ELLIPSE;

             Mat element = getStructuringElement(erosion_type,Size( 3,  3), Point(1, 1));
             erode(img2, img2, element, Point(-1, -1),1);
        }
        else if (type7==2) {        //膨胀
            qDebug()<<"膨胀";

            int dilation_type ;
            dilation_type = MORPH_RECT;
            Mat element = getStructuringElement(dilation_type, Size(3,  3), Point(1, 1));
            dilate(img2, img2, element, Point(-1, -1), 1);
        }
        else if (type7==3) {        //开运算
            qDebug()<<"开运算";
            //int erosion_type=MORPH_RECT;
            //int dilation_type=MORPH_RECT;
            Mat element = getStructuringElement(MORPH_RECT,Size( 3, 3), Point( 1,  1));
            morphologyEx(img2, img2, MORPH_OPEN, element, Point(-1, -1), 1);
        }
        else if(type7==4){
            qDebug()<<"闭运算";       //闭运算
            Mat element = getStructuringElement(MORPH_RECT,Size( 3, 3), Point( 1,  1));
            morphologyEx(img2, img2, MORPH_CLOSE, element, Point(-1, -1), 1);

        }


        if(type1==1){
            qDebug()<<"JET";
            applyColorMap(img2, img2, COLORMAP_JET);

              }
        else if (type1==2) {

            qDebug()<<"HOT";
            applyColorMap(img2, img2, COLORMAP_HOT);

        }
        else if (type1==3) {

            qDebug()<<"TURBO";
            applyColorMap(img2, img2, COLORMAP_TURBO);
        }
        else if (type1==4) {

            qDebug()<<"INFERNO";
            applyColorMap(img2, img2, COLORMAP_INFERNO);
        }



/********************************************************************/

        m_img = MatToQImage(img2);

        emit sendimg();

        //        imshow("source image",img2);
        //        waitKey(3000);

        i++;
        qDebug() << QString("第N ping N=: %1").arg(i);
    }


}

//结束多线程


cv::Mat  MyThread::Array2Mat(const uchar *array, int row, int col)
{

    cv::Mat img(row ,col,  CV_8UC1);

    for (int i = 0; i <row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            img.at<uchar>(i,j) =  *(array + i*col + j) ;
        }
    }
    return img;

    cv::Mat img2(row ,col,  CV_8UC1);

    for (int i = 0; i <row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            img2.at<uchar>(i,j) =  *(array + i*col + j) ;
        }
    }
    return img2;

}

// cv::Mat转换成QImage

QImage  MyThread::MatToQImage(const cv::Mat& mat)
{
    // 8-bits unsigned, NO. OF CHANNELS = 1
    if (mat.type() == CV_8UC1)
    {
        QImage image(mat.cols, mat.rows, QImage::Format_Indexed8);
        // Set the color table (used to translate colour indexes to qRgb values)
        image.setColorCount(256);
        for (int i = 0; i < 256; i++)
        {
            image.setColor(i, qRgb(i, i, i));
        }
        // Copy input Mat
        uchar *pSrc = mat.data;
        for (int row = 0; row < mat.rows; row++)
        {
            uchar *pDest = image.scanLine(row);
            memcpy(pDest, pSrc, mat.cols);
            pSrc += mat.step;
        }
        return image;
    }
    // 8-bits unsigned, NO. OF CHANNELS = 3
    else if (mat.type() == CV_8UC3)
    {
        // Copy input Mat
        const uchar *pSrc = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
        return image.rgbSwapped();
    }
    else if (mat.type() == CV_8UC4)
    {
        // Copy input Mat
        const uchar *pSrc = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);
        return image.copy();
    }
    else
    {
        return QImage();
    }
}


/********************************************几何变换******************************************/

//转换为扇形

Mat  MyThread::Rect2Sect(const uchar *raw_data, int rows, int cols, int angles, int method)
{
    float half_angles = angles/2;
    int radius = rows;
    float half_angle_rad = half_angles/180*PI;
//    qDebug()<<"half_angle_rad"<<half_angle_rad;
    int fan_width = 2*ceil(radius * (float)sin(half_angle_rad)) + 1;
    cv::Mat fan_Image = cv::Mat::zeros(radius, fan_width, CV_8UC1);  //3channel
    int middle_line = (fan_width+1)/2 ;
//    qDebug()<<"middle_line"<<middle_line;
    int step = 1;

    float angle_scale ;
    float range_scale ;

    uint tmp;
    float range_2point;
    float angel_2point;
//    qDebug()<<rows;
//    qDebug()<<cols;
    if (method == 1){
        if (rows%2){
            int old_middle_line =(cols+1)/2;
            for (int row = 1; row < rows; ++row){

                fan_Image.at<uchar>(row, middle_line-1) = *(raw_data + row*cols + old_middle_line-2);
                step = 1;
                while ((range_2point = sqrt(row*row+step*step) < radius) && ( angel_2point = atan((float)step/row) < half_angle_rad)){
                     angle_scale = angel_2point / half_angle_rad;
                     range_scale = range_2point / radius;
//tmp = BilinearInterpolation2(raw_data, rows, cols, angle_scale, range_scale);
                    fan_Image.at<uchar>( row, middle_line-1-step ) = 250;//tmp[0];
                    fan_Image.at<uchar>( row, middle_line-1+step ) = 250;//tmp[1];
                    step = step + 1;
                }
            }
        }
        else{
            int old_middle_line = cols/2;

            for (int row = 1; row < rows; ++row){

/* 伪彩色映射
                int tmp0 =  (*(raw_data + row*cols + old_middle_line-2) + *(raw_data + row*cols + old_middle_line-1))/2;
               //qDebug()<<tmp;


               if(0 <  tmp0 && tmp0 <= 31 ){
                fan_Image.at<Vec3b>( row, middle_line-1 )[0]= 0;
                fan_Image.at<Vec3b>( row, middle_line-1 )[1] = 0;
                fan_Image.at<Vec3b>( row, middle_line-1)[2] = 128+4* tmp0;
               }
               else if(tmp0==32){
                   fan_Image.at<Vec3b>( row, middle_line-1 )[0]= 0;
                   fan_Image.at<Vec3b>(row, middle_line-1 )[1] =0 ;
                   fan_Image.at<Vec3b>( row, middle_line-1 )[2] = 255;

               }
               else if (32 < tmp0 && tmp0 <= 95) {
                   fan_Image.at<Vec3b>( row, middle_line-1 )[0]= 0;
                   fan_Image.at<Vec3b>(row, middle_line-1 )[1] =4+4*tmp0 ;
                   fan_Image.at<Vec3b>( row, middle_line-1 )[2] = 255;

               }
               else if(tmp0==96){
                   fan_Image.at<Vec3b>( row, middle_line-1 )[0]= 2;
                   fan_Image.at<Vec3b>(row, middle_line-1 )[1] =255 ;
                   fan_Image.at<Vec3b>( row, middle_line-1 )[2] = 254;

               }
               else if (96<  tmp0 && tmp0 <= 158) {
                   fan_Image.at<Vec3b>( row, middle_line-1 )[0]=6+4*tmp0;
                   fan_Image.at<Vec3b>( row, middle_line-1 )[1] =255;
                   fan_Image.at<Vec3b>( row, middle_line-1 )[2] =250-4*tmp0;

               }
               else if(tmp0==159){
                   fan_Image.at<Vec3b>( row, middle_line-1 )[0]= 254;
                   fan_Image.at<Vec3b>(row, middle_line-1 )[1] =255 ;
                   fan_Image.at<Vec3b>( row, middle_line-1 )[2] = 1;

               }
               else if (159<  tmp0 && tmp0 <= 223) {
                   fan_Image.at<Vec3b>( row, middle_line-1 )[0]=255;
                   fan_Image.at<Vec3b>( row, middle_line-1 )[1] =252-4*tmp0;
                   fan_Image.at<Vec3b>( row, middle_line-1 )[2] =0;

               }


               else{
                   fan_Image.at<Vec3b>( row, middle_line-1 )[0]=252-4*tmp0;
                   fan_Image.at<Vec3b>( row, middle_line-1 )[1] =0;
                   fan_Image.at<Vec3b>( row, middle_line-1 )[2] =0;

               }


*/
                fan_Image.at<uchar>(row, middle_line-1) = (*(raw_data + row*cols + old_middle_line-2) + *(raw_data + row*cols + old_middle_line-1))/2;
                step = 1;

                while (( sqrt(row*row+step*step) < radius) && ( angel_2point = atan((float)step/row) < half_angle_rad)){
                     angle_scale = atan((float)step/row) / half_angle_rad;
                     range_scale = sqrt(row*row+step*step) / radius;
//qDebug()<<step;
                    tmp = BilinearInterpolation2(raw_data, rows, cols, angle_scale, range_scale);
/*

                    int tmp1 = (tmp&0xff);
                   //qDebug()<<tmp;
                   if(0 < tmp1 && tmp1 <= 31 ){
                    fan_Image.at<Vec3b>( row, middle_line-1-step )[0]= 0;
                    fan_Image.at<Vec3b>( row, middle_line-1-step )[1] = 0;
                    fan_Image.at<Vec3b>( row, middle_line-1-step)[2] = 128+4* tmp1;
                   }
                   else if(tmp1==32){
                       fan_Image.at<Vec3b>( row, middle_line-1-step )[0]= 0;
                       fan_Image.at<Vec3b>(row, middle_line-1-step )[1] =0 ;
                       fan_Image.at<Vec3b>( row, middle_line-1-step )[2] = 255;

                   }
                   else if (32 <tmp1&& tmp1 <= 95) {
                       fan_Image.at<Vec3b>( row, middle_line-1-step )[0]= 0;
                       fan_Image.at<Vec3b>(row, middle_line-1-step )[1] =4+4*tmp1;
                       fan_Image.at<Vec3b>( row, middle_line-1-step )[2] =255;

                   }
                   else if(tmp1==96){
                       fan_Image.at<Vec3b>( row, middle_line-1-step )[0]= 2;
                       fan_Image.at<Vec3b>(row, middle_line-1-step )[1] =255;
                       fan_Image.at<Vec3b>( row, middle_line-1-step )[2] = 254;

                   }
                   else if (96< tmp1 && tmp1 <= 158) {
                       fan_Image.at<Vec3b>( row, middle_line-1-step )[0]=6+4*tmp1;
                       fan_Image.at<Vec3b>( row, middle_line-1-step )[1] =255;
                       fan_Image.at<Vec3b>( row, middle_line-1-step )[2] =250-4*tmp1;

                   }
                   else if(tmp1==159){
                       fan_Image.at<Vec3b>( row, middle_line-1-step )[0]= 254;
                       fan_Image.at<Vec3b>(row, middle_line-1-step )[1] =255;
                       fan_Image.at<Vec3b>( row, middle_line-1-step )[2] = 1;

                   }

                   else if (159< tmp1 && tmp1 <= 223) {
                       fan_Image.at<Vec3b>( row, middle_line-1-step )[0]=255;
                       fan_Image.at<Vec3b>( row, middle_line-1-step )[1] =252-4*tmp1;
                       fan_Image.at<Vec3b>( row, middle_line-1-step )[2] =0;

                   }
                   else{
                       fan_Image.at<Vec3b>( row, middle_line-1-step )[0]=252-4*tmp1;
                       fan_Image.at<Vec3b>( row, middle_line-1-step )[1] =0;
                       fan_Image.at<Vec3b>( row, middle_line-1-step )[2] =0;

                   }



                   int tmp2= (tmp&0xff00)>>8;

                   if(0 <tmp2 && tmp2 <=31 ){
                    fan_Image.at<Vec3b>( row, middle_line-1+step )[0]= 0;
                    fan_Image.at<Vec3b>( row, middle_line-1+step )[1] =0;
                    fan_Image.at<Vec3b>( row, middle_line-1+step)[2] = 128+4* tmp2;
                   }
                   else if(tmp2==32){
                       fan_Image.at<Vec3b>( row, middle_line-1+step )[0]= 0;
                       fan_Image.at<Vec3b>(row, middle_line-1+step )[1] =0;
                       fan_Image.at<Vec3b>( row, middle_line-1+step )[2] = 255;

                   }
                   else if (32 <tmp2 && tmp2 <= 95) {
                       fan_Image.at<Vec3b>( row, middle_line-1+step )[0]= 0;
                       fan_Image.at<Vec3b>(row, middle_line-1+step )[1] =4+4*tmp2;
                       fan_Image.at<Vec3b>( row, middle_line-1+step )[2] = 255;

                   }
                   else if(tmp2==96){
                       fan_Image.at<Vec3b>( row, middle_line-1+step )[0]= 2;
                       fan_Image.at<Vec3b>(row, middle_line-1+step )[1] =255;
                       fan_Image.at<Vec3b>( row, middle_line-1+step )[2] = 254;

                   }
                   else if (96< tmp2 && tmp2 <= 158) {
                       fan_Image.at<Vec3b>( row, middle_line-1+step )[0]=6+4*tmp2;
                       fan_Image.at<Vec3b>( row, middle_line-1+step )[1] =255;
                       fan_Image.at<Vec3b>( row, middle_line-1+step )[2] =250-4*tmp2;

                   }
                   else if(tmp2==159){
                       fan_Image.at<Vec3b>( row, middle_line-1+step )[0]= 254;
                       fan_Image.at<Vec3b>(row, middle_line-1+step )[1] =255;
                       fan_Image.at<Vec3b>( row, middle_line-1+step )[2] = 1;

                   }

                   else if (159< tmp2 && tmp2 <= 223) {
                       fan_Image.at<Vec3b>( row, middle_line-1+step )[0]=255;
                       fan_Image.at<Vec3b>( row, middle_line-1+step )[1] =252-4*tmp2;
                       fan_Image.at<Vec3b>( row, middle_line-1+step )[2] =0;

                   }

                   else{
                       fan_Image.at<Vec3b>( row, middle_line-1+step )[0]=252-4*tmp2;
                       fan_Image.at<Vec3b>( row, middle_line-1+step )[1] =0;
                       fan_Image.at<Vec3b>( row,middle_line-1+step )[2] =0;

                   }

*/
                    fan_Image.at<uchar>( row, middle_line-1-step ) = (uchar)(tmp&0xff);
                   fan_Image.at<uchar>( row, middle_line-1+step ) = (uchar)((tmp&0xff00)>>8);

                   // qDebug()<<fan_Image.at<uchar>( row, middle_line-1+step );
                    //qDebug()<<fan_Image.at<uchar>( row, middle_line-1-step );

                    step = step + 1;

                }

            }
        }

       // cvtColor(fan_Image,fan_Image,cv::COLOR_BGR2RGB);

        return fan_Image;
    }
}


//双线性插值

uint MyThread::BilinearInterpolation2(const uchar *data, int rows, int cols, float angle_scale, float range_scale)
{
    uchar left_value;
    uchar right_value;

    if (rows%2){
       int middle_line = (cols+1)/2 ;
       float left_star = middle_line - 1;

       float row = rows * range_scale;
       float col = (left_star+0.5) * angle_scale-0.5;

       uchar xl_self =  *(data + (uchar)floor(row)*cols + middle_line-(uchar)floor(col) - 1);

       uchar xl_up =  *(data + (uchar)(floor(row) + 1)*cols + middle_line-(uchar)floor(col)-1);
       uchar xl_left =  *(data + (uchar)floor(row)*cols + middle_line-(uchar)floor(col)-2);
       uchar xl_up_left =  *(data + (uchar)(floor(row) + 1)*cols + middle_line-(uchar)floor(col)-2);

       uchar xr_self =  *(data + (uchar)floor(row) * cols + middle_line + (uchar)floor(col) - 1);
       uchar xr_up =  *(data + (uchar)(floor(row) + 1) * cols + middle_line + (uchar)floor(col) - 1);
       uchar xr_right =  *(data + (uchar)floor(row)*cols + middle_line + (uchar)floor(col) + 1 -1);
       uchar xr_up_right =  *(data + (uchar)(floor(row) + 1) * cols + middle_line + (uchar)floor(col) + 1 - 1);

       left_value = xl_self * ((angle_scale-floor(angle_scale))* (range_scale - floor(angle_scale)))+
                    xl_up * ((angle_scale-floor(angle_scale))* (1-(range_scale - floor(angle_scale))))+
                    xl_left * ((1-(angle_scale-floor(angle_scale)))* (range_scale - floor(angle_scale)))+
                    xl_up_left * ((1-(angle_scale-floor(angle_scale)))* ((1-range_scale - floor(angle_scale))));

      right_value = xr_self * ((angle_scale-floor(angle_scale))* (range_scale - floor(angle_scale)))+
                    xr_up * ((angle_scale-floor(angle_scale))* (1-(range_scale - floor(angle_scale))))+
                    xr_right * ((1-(angle_scale-floor(angle_scale)))* (range_scale - floor(angle_scale)))+
                    xr_up_right * ((1-(angle_scale-floor(angle_scale)))* ((1-range_scale - floor(angle_scale))));

    }
    else{
        int middle_line = cols/2 ;

         int left_star = middle_line ;
         int right_star = middle_line - 1;

         float  row = rows* range_scale  ;
         float col = middle_line* angle_scale ;

         uchar xl_self =  *(data + (int)floor(row)*cols + left_star-(int)floor(col) - 1);
         uchar xl_up =  *(data + (int)(floor(row) + 1)*cols + left_star-(int)floor(col)-1);
         uchar xl_left =  *(data + (int)floor(row)*cols + left_star-(int)floor(col)-2);
         uchar xl_up_left =  *(data + (int)(floor(row) + 1)*cols + left_star-(int)floor(col)-2);

         uchar xr_self =  *(data + (int)floor(row) * cols + right_star + (int)floor(col) - 1);
         uchar xr_up =  *(data + (int)(floor(row) + 1) * cols + right_star + (int)floor(col) - 1);
         uchar xr_right =  *(data + (int)floor(row)*cols + right_star + (int)floor(col) + 1 -1);
         uchar xr_up_right =  *(data + (int)(floor(row) + 1) * cols + right_star + (int)floor(col) + 1 - 1);

         left_value = xl_self * ((angle_scale-floor(angle_scale))* (range_scale - floor(angle_scale)))+
                      xl_up * ((angle_scale-floor(angle_scale))* (1-(range_scale - floor(angle_scale))))+
                      xl_left * ((1-(angle_scale-floor(angle_scale)))* (range_scale - floor(angle_scale)))+
                      xl_up_left * ((1-(angle_scale-floor(angle_scale)))* ((1-range_scale - floor(angle_scale))));

        right_value = xr_self * ((angle_scale-floor(angle_scale))* (range_scale - floor(angle_scale)))+
                      xr_up * ((angle_scale-floor(angle_scale))* (1-(range_scale - floor(angle_scale))))+
                      xr_right * ((1-(angle_scale-floor(angle_scale)))* (range_scale - floor(angle_scale)))+
                      xr_up_right * ((1-(angle_scale-floor(angle_scale)))* ((1-range_scale - floor(angle_scale))));

    }
    //qDebug()<<"left_value "<<(uint)left_value <<"right_value"<<(uint)right_value;
        uint left_right_value = ((uint)left_value) << 8 ;
        left_right_value += (uint)right_value;
return left_right_value;

}

//ostu

int MyThread::Otsu(cv::Mat& src, cv::Mat& dst, int thresh){
    const int Grayscale = 256;
    int graynum[Grayscale] = { 0 };
    int r = src.rows;
    int c = src.cols;
    for (int i = 0; i < r; ++i){
        const uchar* ptr = src.ptr<uchar>(i);
        for (int j = 0; j < c; ++j){        //直方图统计
            graynum[ptr[j]]++;
        }
    }

    double P[Grayscale] = { 0 };
    double PK[Grayscale] = { 0 };
    double MK[Grayscale] = { 0 };
    double srcpixnum = r*c, sumtmpPK = 0, sumtmpMK = 0;
    for (int i = 0; i < Grayscale; ++i){
        P[i] = graynum[i] / srcpixnum;   //每个灰度级出现的概率
        PK[i] = sumtmpPK + P[i];         //概率累计和
        sumtmpPK = PK[i];
        MK[i] = sumtmpMK + i*P[i];       //灰度级的累加均值
        sumtmpMK = MK[i];
    }

    //计算类间方差
    double Var=0;
    for (int k = 0; k < Grayscale; ++k){
        if ((MK[Grayscale-1] * PK[k] - MK[k])*(MK[Grayscale-1] * PK[k] - MK[k]) / (PK[k] * (1 - PK[k])) > Var){
            Var = (MK[Grayscale-1] * PK[k] - MK[k])*(MK[Grayscale-1] * PK[k] - MK[k]) / (PK[k] * (1 - PK[k]));
            thresh = k;
        }
    }

    //阈值处理
    src.copyTo(dst);
    for (int i = 0; i < r; ++i){
        uchar* ptr = dst.ptr<uchar>(i);
        for (int j = 0; j < c; ++j){
            if (ptr[j]> thresh)
                ptr[j] = 255;
            else
                ptr[j] = 0;
        }
    }
    return thresh;
}

//最大熵阈值分割
Mat MyThread::MaxEntropySegment(Mat src)
{
    int tbHist[256] = { 0 };
    int index = 0;
    double Property = 0.0;
    double maxEntropy = -1.0;
    double frontEntropy = 0.0;
    double backEntropy = 0.0;
    //纳入计算的总像素数
    int TotalPixel = 0;
    int nCol = src.cols * src.channels();
    for (int i = 0; i < src.rows; i++)
    {
        uchar* pData = src.ptr<uchar>(i);
        for (int j = 0; j < nCol; ++j)
        {
            ++TotalPixel;
            tbHist[pData[j]] += 1;
        }
    }

    for (int i = 0; i < 256; i++)
    {
        //计算背景像素数
        double backTotal = 0;
        for (int j = 0; j < i; j++)
        {
            backTotal += tbHist[j];
        }

        //背景熵
        for (int j = 0; j < i; j++)
        {
            if (tbHist[j] != 0)
            {
                Property = tbHist[j] / backTotal;
                backEntropy += -Property * logf((float)Property);
            }
        }
        //前景熵
        for (int k = i; k < 256; k++)
        {
            if (tbHist[k] != 0)
            {
                Property = tbHist[k] / (TotalPixel - backTotal);
                frontEntropy += -Property * logf((float)Property);
            }
        }

        if (frontEntropy + backEntropy > maxEntropy)    //得到最大熵
        {
            maxEntropy = frontEntropy + backEntropy;
            index = i;
        }
        //清空本次计算熵值
        frontEntropy = 0.0;
        backEntropy = 0.0;
    }
    Mat dst;
    //cout << "index:" << index;
    cv::threshold(src, dst, index, 255, 0);             //进行阈值分割
    return dst.clone();
}






