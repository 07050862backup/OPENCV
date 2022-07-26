#include <cstdio>  
#include <opencv2/opencv.hpp>  
using namespace cv;

int sliderValue;
bool applysave = false;
Mat small, bigbackground, imgROI, imgROI2, temp, ans;
void applyFilters();
void saveCallback(int state, void* userData);
void on_trackbar(int, void*);
int main() {

    sliderValue = 0;
    int sliderMaxValue = 100;

    bigbackground = imread("pic3.jpg", -1);

    small = imread("penguin.jpg", -1);
    imgROI = bigbackground(Rect(125, 30, small.cols, small.rows));

    imshow("img___", bigbackground);

    createTrackbar("Ratio", "img___", &sliderValue, sliderMaxValue, on_trackbar);
    on_trackbar(sliderValue, 0);
    
    createButton("save", saveCallback, NULL, QT_PUSH_BUTTON, 0);
    
    
    waitKey(0);
    return 0;
}
void applyFilters() {
    if (applysave) {
        imshow("overlap.jpg", ans);
        imwrite("overlap.jpg", ans);
    }
}
void saveCallback(int state, void* userData)
{
    applysave = !applysave;
    applyFilters();
}
void on_trackbar(int, void*) {

    double alpha = (double)sliderValue / 100.0;
    double beta = (1.0 - alpha);

    addWeighted(small, beta, imgROI, alpha, 0, imgROI2);

    cv::Mat temp = imgROI2.clone();//temp 是 ok 的感興趣圖
    cv::Mat ans_center_roi = imgROI2.clone();//ans_center_roi 是 ok 的感興趣圖

    ans = bigbackground.clone();
    temp = ans(Rect(125, 30, small.cols, small.rows));
    ans_center_roi.copyTo(temp);

    imshow("img___", ans);
}

