#include <opencv2/opencv.hpp>  
#include <iostream>

using namespace cv;
using namespace std;
int hox = 0, hoy = 0;
vector<Mat> horizontalProjectionMat(Mat vertical_gray, Mat srcImg_copy)//������v
{
    Mat binImg;
    cv::Mat ori_copy = srcImg_copy.clone();

    blur(vertical_gray, binImg, Size(3, 3));//blur�����o�i��ơA�o�i�֤j�p��3*3
    threshold(binImg, binImg, 0, 255, THRESH_OTSU);//threshold��ƬO�i��Ϲ��G�ȤƳB�z�A�ϥ�OTSU�t��k�T�w�����H��
    int perPixelValue = 0;
    int width = vertical_gray.cols;
    int height = vertical_gray.rows;
    int* projectValArry = new int[height];//�Ыؤ@�Ӧs�x�C��զ�Ϥ��Ӽƪ��}�C
    memset(projectValArry, 0, height * 4);//��l�ư}�C: ��pmemset(ch,0,8),�N�O��}�Cch�e�K���m���s�A�᭱�����@�w���s�C
    for (int col = 0; col < height; col++)//�M���C�ӹϤ��I
    {
        for (int row = 0; row < width; row++)
        {
            perPixelValue = binImg.at<uchar>(col, row);//img.at<uchar>(i, j)=128�o�O���N�ǫ׹Ϫ���x��A��y�C���m���ǫ׬�128���Ǧ�]0���¦�A255���զ�^
            //�p�G�O�m�ϡA�h�g��ima.at<cv::Vec3b>(x, y)[0]�Aima.at<cv::Vec3b>(x, y)[1]�Aima.at<cv::Vec3b>(x, y)[2]�A���O��ܬ����š]���Ǥ��T�w�^�T���C�⪺���
            if (perPixelValue == 0)//�p�G�O�թ��¦r
            {
                projectValArry[col]++;
            }
        }
    }
    Mat horizontalProjectionMat(height, width, CV_8UC1);//�Ыصe��
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            perPixelValue = 255;
            horizontalProjectionMat.at<uchar>(i, j) = perPixelValue;//�]�m�I�����զ�
        }
    }
    for (int i = 0; i < height; i++)//����������
    {
        for (int j = 0; j < projectValArry[i]; j++)
        {
            perPixelValue = 0;
            horizontalProjectionMat.at<uchar>(i, width - 1 - j) = perPixelValue;//�]�m�����Ϭ��¦�
        }
    }
    namedWindow("horizontal", WINDOW_NORMAL);
    imshow("horizontal", horizontalProjectionMat);
    //waitKey(0);
    vector<Mat> roilist;//�Ω�s�x���ΥX�Ӫ��C�Ӧr��
    int startIndex = 0;//�O���i�J�r���Ϫ�����
    int endIndex = 0;//�O���i�J�ťհϰ쪺����
    bool inBlock = false;//�O�_�M����F�r���Ϥ�
    for (int i = 0; i < vertical_gray.rows; i++)
    {
        if (!inBlock && projectValArry[i] != 0)//�i�J�r����
        {
            inBlock = true;
            startIndex = i;
        }
        else if (inBlock && projectValArry[i] == 0)//�i�J�ťհ�
        {
            endIndex = i;
            inBlock = false;
            Mat roiImg = vertical_gray(Range(startIndex, endIndex + 1), Range(0, vertical_gray.cols));//�q��Ϥ��I�����Ϲ����ϰ�
            roilist.push_back(roiImg);
            rectangle(ori_copy, Point(hox, startIndex), Point(hoy, endIndex + 1), Scalar((rand() & 255), (rand() & 255), (rand() & 255)), 3, 4, 0);



            cout << "startIndex = " << startIndex << endl;
            cout << "endIndex + 1 = " << endIndex + 1 << endl;
            cout << "srcImg_gray.cols = " << vertical_gray.cols << endl;
            cout << "srcImg_gray.rows = " << vertical_gray.rows << endl;
        }
    }
    namedWindow("001", WINDOW_NORMAL);
    imshow("001", ori_copy);
    delete[] projectValArry;
    return roilist;
}
vector<Mat> verticalProjectionMat(Mat srcImg_copy_gray, Mat srcImg)//������v
{
    Mat binImg;
    cv::Mat ori_copy = srcImg.clone();
    blur(srcImg_copy_gray, binImg, Size(3, 3));
    threshold(binImg, binImg, 0, 255, THRESH_OTSU);
    int perPixelValue;//�C�ӹϤ�����
    int width = srcImg_copy_gray.cols;
    int height = srcImg_copy_gray.rows;
    int* projectValArry = new int[width];//�ЫإΩ�s�x�C�C�զ�Ϥ��Ӽƪ��}�C
    memset(projectValArry, 0, width * 4);//��l�ư}�C
    for (int col = 0; col < width; col++)
    {
        for (int row = 0; row < height; row++)
        {
            perPixelValue = binImg.at<uchar>(row, col);
            if (perPixelValue == 0)//�p�G�թ��¦r
            {
                projectValArry[col]++;
            }
        }
    }
    Mat verticalProjectionMat(height, width, CV_8UC1);//������v���e��
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            perPixelValue = 255;//�I���]�m���զ�
            verticalProjectionMat.at<uchar>(i, j) = perPixelValue;
        }
    }
    for (int i = 0; i < width; i++)//������v������
    {
        for (int j = 0; j < projectValArry[i]; j++)
        {
            perPixelValue = 0;//�����ϳ]�m���¦�
            verticalProjectionMat.at<uchar>(height - 1 - j, i) = perPixelValue;
        }
    }
    namedWindow("vertical", WINDOW_NORMAL);
    imshow("vertical", verticalProjectionMat);
    //waitKey(0);
    vector<Mat> roilist;//�Ω�s�x���ΥX�Ӫ��C�Ӧr��
    int startIndex = 0;//�O���i�J�r���Ϫ�����
    int endIndex = 0;//�O���i�J�ťհϰ쪺����
    bool inBlock = false;//�O�_�M����F�r���Ϥ�
    for (int i = 0; i < srcImg_copy_gray.cols; i++)//cols=width
    {
        if (!inBlock && projectValArry[i] != 0)//�i�J�r����
        {
            inBlock = true;
            startIndex = i;
        }
        else if (projectValArry[i] == 0 && inBlock)//�i�J�ťհ�
        {
            endIndex = i;
            inBlock = false;
            Mat roiImg = srcImg_copy_gray(Range(0, srcImg_copy_gray.rows), Range(startIndex, endIndex + 1));
            roilist.push_back(roiImg);//push_back() �bVector�̫�K�[�@�Ӥ����]�ѼƬ��n���J���ȡ^
            hox = endIndex;
            hoy = startIndex;

            // rectangle(ori_copy, Point(srcImg_copy_gray.rows, startIndex), Point(endIndex + 1, srcImg_copy_gray.cols), Scalar((rand() & 255), (rand() & 255), (rand() & 255)), 3, 4, 0);
             //rectangle(ori_copy, Point(srcImg_copy_gray.rows,startIndex), Point(srcImg_copy_gray.cols, endIndex + 1), Scalar((rand() & 255), (rand() & 255), (rand() & 255)), 3, 4, 0);
             //rectangle(ori_copy, Point(srcImg_copy_gray.rows, srcImg_copy_gray.cols), Point(endIndex + 1, startIndex), Scalar((rand() & 255), (rand() & 255), (rand() & 255)), 3, 4, 0);
            // rectangle(ori_copy, Point(srcImg_copy_gray.rows, srcImg_copy_gray.cols), Point(startIndex, endIndex + 1), Scalar((rand() & 255), (rand() & 255), (rand() & 255)), 3, 4, 0);
            //rectangle(ori_copy, Point(srcImg_copy_gray.rows, endIndex + 1), Point(startIndex, srcImg_copy_gray.cols), Scalar((rand() & 255), (rand() & 255), (rand() & 255)), 3, 4, 0);
            // rectangle(ori_copy, Point(srcImg_copy_gray.rows, endIndex + 1), Point(srcImg_copy_gray.cols, startIndex), Scalar((rand() & 255), (rand() & 255), (rand() & 255)), 3, 4, 0);



            cout << "startIndex = " << startIndex << endl;
            cout << "endIndex + 1 = " << endIndex + 1 << endl;
            cout << "srcImg_copy_gray.cols = " << srcImg_copy_gray.cols << endl;
            cout << "srcImg_copy_gray.rows = " << srcImg_copy_gray.rows << endl;
        }
    }

    namedWindow("ans", WINDOW_NORMAL);
    imshow("ans", ori_copy);
    delete[] projectValArry;
    return roilist;
}
int main(int argc, char* argv[])
{
    Mat srcImg = imread("text.jpg", 1);
    cv::Mat srcImg_copy_gray = srcImg.clone();
    cvtColor(srcImg_copy_gray, srcImg_copy_gray, COLOR_BGR2GRAY);
    namedWindow("ori", WINDOW_NORMAL);
    imshow("ori", srcImg);
    char szName[30] = { 0 };
    vector<Mat> b = verticalProjectionMat(srcImg_copy_gray, srcImg);//���i�櫫����v

    cv::Mat mDebug = srcImg.clone();

    for (int i = 0; i < b.size(); i++)
    {
        vector<Mat> a = horizontalProjectionMat(b[i], srcImg);//������v
        //sprintf_s(szName, "E:\\picture\\%d.jpg", i);

        for (int j = 0; j < a.size(); j++)
        {
            cout << "a=" << i << "�Ab=" << j << endl;
            // imshow(szName, a[j]);
            Mat img = a[j];
            string s = to_string(i);
            namedWindow(s, WINDOW_NORMAL);
            imshow(s, img);

            cv::Size szRoi;
            cv::Point ptRoi;
            img.locateROI(szRoi, ptRoi);
            cv::rectangle(mDebug, cv::Rect(ptRoi, img.size()), cv::Scalar(0, 0, 255));
        }
        Mat img2 = b[i];
        string s2 = to_string(i);
        namedWindow(s2 + "ABC", WINDOW_NORMAL);

        imshow(s2 + "ABC", img2);
    }

    imshow("mDebug", mDebug);
    waitKey(0);

    return 0;
}
