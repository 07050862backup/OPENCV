#include <opencv2/opencv.hpp>  
#include <iostream>

using namespace cv;
using namespace std;
int hox = 0, hoy = 0;
vector<Mat> horizontalProjectionMat(Mat vertical_gray, Mat srcImg_copy)//水平投影
{
    Mat binImg;
    cv::Mat ori_copy = srcImg_copy.clone();

    blur(vertical_gray, binImg, Size(3, 3));//blur均值濾波函數，濾波核大小為3*3
    threshold(binImg, binImg, 0, 255, THRESH_OTSU);//threshold函數是進行圖像二值化處理，使用OTSU演算法確定分割閾值
    int perPixelValue = 0;
    int width = vertical_gray.cols;
    int height = vertical_gray.rows;
    int* projectValArry = new int[height];//創建一個存儲每行白色圖元個數的陣列
    memset(projectValArry, 0, height * 4);//初始化陣列: 比如memset(ch,0,8),就是把陣列ch前八項置為零，後面的不一定為零。
    for (int col = 0; col < height; col++)//遍歷每個圖元點
    {
        for (int row = 0; row < width; row++)
        {
            perPixelValue = binImg.at<uchar>(col, row);//img.at<uchar>(i, j)=128這是指將灰度圖的第x行，第y列的置為灰度為128的灰色（0為黑色，255為白色）
            //如果是彩圖，則寫為ima.at<cv::Vec3b>(x, y)[0]，ima.at<cv::Vec3b>(x, y)[1]，ima.at<cv::Vec3b>(x, y)[2]，分別表示紅綠藍（順序不確定）三種顏色的色度
            if (perPixelValue == 0)//如果是白底黑字
            {
                projectValArry[col]++;
            }
        }
    }
    Mat horizontalProjectionMat(height, width, CV_8UC1);//創建畫布
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            perPixelValue = 255;
            horizontalProjectionMat.at<uchar>(i, j) = perPixelValue;//設置背景為白色
        }
    }
    for (int i = 0; i < height; i++)//水平長條圖
    {
        for (int j = 0; j < projectValArry[i]; j++)
        {
            perPixelValue = 0;
            horizontalProjectionMat.at<uchar>(i, width - 1 - j) = perPixelValue;//設置長條圖為黑色
        }
    }
    namedWindow("horizontal", WINDOW_NORMAL);
    imshow("horizontal", horizontalProjectionMat);
    //waitKey(0);
    vector<Mat> roilist;//用於存儲分割出來的每個字元
    int startIndex = 0;//記錄進入字元區的索引
    int endIndex = 0;//記錄進入空白區域的索引
    bool inBlock = false;//是否遍歷到了字元區內
    for (int i = 0; i < vertical_gray.rows; i++)
    {
        if (!inBlock && projectValArry[i] != 0)//進入字元區
        {
            inBlock = true;
            startIndex = i;
        }
        else if (inBlock && projectValArry[i] == 0)//進入空白區
        {
            endIndex = i;
            inBlock = false;
            Mat roiImg = vertical_gray(Range(startIndex, endIndex + 1), Range(0, vertical_gray.cols));//從原圖中截取有圖像的區域
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
vector<Mat> verticalProjectionMat(Mat srcImg_copy_gray, Mat srcImg)//垂直投影
{
    Mat binImg;
    cv::Mat ori_copy = srcImg.clone();
    blur(srcImg_copy_gray, binImg, Size(3, 3));
    threshold(binImg, binImg, 0, 255, THRESH_OTSU);
    int perPixelValue;//每個圖元的值
    int width = srcImg_copy_gray.cols;
    int height = srcImg_copy_gray.rows;
    int* projectValArry = new int[width];//創建用於存儲每列白色圖元個數的陣列
    memset(projectValArry, 0, width * 4);//初始化陣列
    for (int col = 0; col < width; col++)
    {
        for (int row = 0; row < height; row++)
        {
            perPixelValue = binImg.at<uchar>(row, col);
            if (perPixelValue == 0)//如果白底黑字
            {
                projectValArry[col]++;
            }
        }
    }
    Mat verticalProjectionMat(height, width, CV_8UC1);//垂直投影的畫布
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            perPixelValue = 255;//背景設置為白色
            verticalProjectionMat.at<uchar>(i, j) = perPixelValue;
        }
    }
    for (int i = 0; i < width; i++)//垂直投影長條圖
    {
        for (int j = 0; j < projectValArry[i]; j++)
        {
            perPixelValue = 0;//長條圖設置為黑色
            verticalProjectionMat.at<uchar>(height - 1 - j, i) = perPixelValue;
        }
    }
    namedWindow("vertical", WINDOW_NORMAL);
    imshow("vertical", verticalProjectionMat);
    //waitKey(0);
    vector<Mat> roilist;//用於存儲分割出來的每個字元
    int startIndex = 0;//記錄進入字元區的索引
    int endIndex = 0;//記錄進入空白區域的索引
    bool inBlock = false;//是否遍歷到了字元區內
    for (int i = 0; i < srcImg_copy_gray.cols; i++)//cols=width
    {
        if (!inBlock && projectValArry[i] != 0)//進入字元區
        {
            inBlock = true;
            startIndex = i;
        }
        else if (projectValArry[i] == 0 && inBlock)//進入空白區
        {
            endIndex = i;
            inBlock = false;
            Mat roiImg = srcImg_copy_gray(Range(0, srcImg_copy_gray.rows), Range(startIndex, endIndex + 1));
            roilist.push_back(roiImg);//push_back() 在Vector最後添加一個元素（參數為要插入的值）
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
    vector<Mat> b = verticalProjectionMat(srcImg_copy_gray, srcImg);//先進行垂直投影

    cv::Mat mDebug = srcImg.clone();

    for (int i = 0; i < b.size(); i++)
    {
        vector<Mat> a = horizontalProjectionMat(b[i], srcImg);//水平投影
        //sprintf_s(szName, "E:\\picture\\%d.jpg", i);

        for (int j = 0; j < a.size(); j++)
        {
            cout << "a=" << i << "，b=" << j << endl;
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
