#include <opencv2/opencv.hpp>  
#include <iostream>

using namespace cv;
using namespace std;

const String keys =
"{help h usage ? |             | print this message      }"
"{input          |airplane.jpg | image1 for read         }"
"{color          |0            | 0:red, 1:Green, 2:Blue  }"
;

Scalar color=Scalar(0,0,255);

void Problem1()
{
	Mat image(640, 480, CV_8UC3, cv::Scalar(0, 255, 0));
	namedWindow("Problem1", WINDOW_NORMAL);
	imshow("Problem1", image);
	waitKey(0);
	Mat image2 = imread("images/shape.png");
	Mat image3(image2, Rect(200, 60, 140, 120));
	flip(image3, image3, 1);
	imshow("Problem1", image2);
	waitKey(0);
}

static void onMouse(int event, int x, int y, int, void* userInput)
{
	if (event != EVENT_LBUTTONDOWN)
		return;

	// Get the pointer input image
	Mat* img = (Mat*)userInput;

	// Draw circle
	circle(*img, Point(x, y), 5, color, 2);
	imshow("Problem2", *img);
}

void Problem2(int argc, char *argv[])
{
	CommandLineParser parser(argc, argv, keys);

	Mat src = imread(parser.get<String>("input"));
	int select = parser.get<int>("color");
	if (select == 0)
		color = Scalar(0, 0, 255);
	else if(select == 1)
		color = Scalar(0, 255, 0);
	else
		color = Scalar(255, 0, 0);

	namedWindow("Problem2");

	setMouseCallback("Problem2", onMouse, &src);
	imshow("Problem2", src);
	waitKey(0);
}

int countPixels(Mat &src, Scalar color)
{
	int count = 0;
	for (int i = 0; i < src.rows; i++)
	{
		for (int j = 0; j < src.cols; j++)
		{
			if ((src.at<Vec3b>(i, j)[0] == color[0]) && (src.at<Vec3b>(i, j)[1] == color[1]) && (src.at<Vec3b>(i, j)[2] == color[2]))
			{
				count++;
			}
		}
	}
	return count;
}

void Problem3()
{
	namedWindow("Problem3");
	double alpha;
	double beta;

	Mat img1 = imread("images/pic1.jpg");
	Mat img2 = imread("images/shape.png");

	//imshow("pic1.jpg", img1);
	//imshow("shape.png", img2);
	for (int i = 0; i < img2.rows; i++)
		for (int j = 0; j < img2.cols; j++)
		{
			if (img2.at<Vec3b>(i, j)[0] == 255 && img2.at<Vec3b>(i, j)[1] == 255 && img2.at<Vec3b>(i, j)[2] == 0) { // LightBlue color
				alpha = 0.6;
			}
			else if (img2.at<Vec3b>(i, j)[0] == 0 && img2.at<Vec3b>(i, j)[1] == 255 && img2.at<Vec3b>(i, j)[2] == 0) { //Green color
				alpha = 0.3;
			}
			else if (img2.at<Vec3b>(i, j)[0] == 0 && img2.at<Vec3b>(i, j)[1] == 0 && img2.at<Vec3b>(i, j)[2] == 255) { //Red color
				alpha = 0.1;
			}
			else
				alpha = 1.0;
			beta = 1 - alpha;
			img1.at<Vec3b>(i, j)[0] = saturate_cast<uchar>(img2.at<Vec3b>(i, j)[0] * beta + img1.at<Vec3b>(i, j)[0] * alpha);
			img1.at<Vec3b>(i, j)[1] = saturate_cast<uchar>(img2.at<Vec3b>(i, j)[1] * beta + img1.at<Vec3b>(i, j)[1] * alpha);
			img1.at<Vec3b>(i, j)[2] = saturate_cast<uchar>(img2.at<Vec3b>(i, j)[2] * beta + img1.at<Vec3b>(i, j)[2] * alpha);
		}
	imshow("Problem3", img1);
	waitKey(0);
}

void Problem4()
{
	Mat src = imread("images/shape.png");
	int lightblueCount = countPixels(src, Scalar(255, 255, 0));
	int greenCount = countPixels(src, Scalar(0, 255, 0));
	int redCount = countPixels(src, Scalar(0, 0, 255));

	//String text = "(" + to_string(lightblueCount) + "," + to_string(greenCount) + "," + to_string(redCount) + ")";
	String text = "(xxxx,yyyy,zzzz)";
	putText(src, text, Point(10, 50), FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 0), 2, 8, 0);
	imshow("Problem4", src);
	waitKey(0);
}

int main(int argc, char *argv[])
{

	Problem1();
	Problem2(argc, argv);
	Problem3();
	Problem4();
	return 0;
}

