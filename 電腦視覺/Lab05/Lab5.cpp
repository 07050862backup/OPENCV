#include <opencv2/opencv.hpp>
#include <iostream>
using namespace cv;
using namespace std;

const String keys =
"{help h usage ? |                     | print this message     }"
"{@input          |img0001-spnoise.bmp | �����Q���T���Ƕ��v��     }"
"{@lightPattern   |lightpattern.bmp    | �T�w�I���v��            }"
;
Mat Problem1(int argc, char* argv[])
{
    CommandLineParser parser(argc, argv, keys);

    Mat image1 = imread(parser.get<String>("@input"));

    return image1;
}
Mat Problem2(Mat image1)
{
    Mat noiseremoved;
    medianBlur(image1, noiseremoved, 5);//�i�H�խ�

    return noiseremoved;
}
Mat Problem3(int argc, char* argv[])
{
    CommandLineParser parser(argc, argv, keys);
    Mat image2 = imread(parser.get<String>("@lightPattern"));
    return image2;
}
Mat Problem4(Mat noiseremoved, Mat image2)
{
    Mat bgremoved;
    absdiff(noiseremoved, image2, bgremoved);
    return bgremoved;
}
Mat Problem5(Mat bgremoved)
{
    // ��Ϲ��i��G�ȤƳB�z 
    Mat thresho;
    threshold(bgremoved, thresho, 40, 255, THRESH_BINARY);//40�O�ۤv�C�C�ժ�
    //imshow("Threshold", thresho);

    return thresho;
}
Mat Problem6(Mat thresho)
{
    Mat connectedCompo;
    cv::cvtColor(thresho, connectedCompo, cv::COLOR_RGB2GRAY);//RGB to Gray


    Mat bw = (connectedCompo > 40);//�NconnectedCompo�j��40���ȡA�]��255�A��l���ȳ]��0
    Mat labelImage(connectedCompo.size(), CV_32S);
    int nLabels = connectedComponents(bw, labelImage, 8);
    std::vector<Vec3b> colors(nLabels);
    colors[0] = Vec3b(0, 0, 0);//background
    for (int label = 1; label < nLabels; ++label) {
        colors[label] = Vec3b((rand() & 255), 0, (rand() & 255));
    }
    Mat dst5(connectedCompo.size(), CV_8UC3);
    for (int r = 0; r < dst5.rows; ++r) {
        for (int c = 0; c < dst5.cols; ++c) {
            int label = labelImage.at<int>(r, c);
            Vec3b& pixel = dst5.at<Vec3b>(r, c);
            pixel = colors[label];
        }
    }
    //imshow("Connected Components", dst5);
    return dst5;
}

Mat Problem7_2(Mat img)
{
    // �ƥ���l�Ϲ��A�Ψ���ܬd�䪺���� 
    Mat dst = img.clone();
    // �m��Ϲ��ഫ���ǫ׹Ϲ� 
    cvtColor(img, dst, COLOR_BGR2GRAY);

    // ��Ϲ��i��G�ȤƳB�z 
    threshold(dst, dst, 40, 255, THRESH_BINARY_INV);
    std::vector<std::vector<Point> > contours;
    std::vector<Vec4i> hierarchy;
    // �d��Ϲ������� �d�䪺�������զ�ϰ쪺���� 
    findContours(dst, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);
    // �w�q�~���x�� 
    std::vector<Rect> boundRect(contours.size());
    dst = img.clone();
    for (int i = 0; i < contours.size(); i++)
    {
        // ����̤p�~���x�� 
        boundRect[i] = boundingRect(contours[i]);
        // �b�Ϲ��Wø�s�̤p�~���x�� 
        rectangle(dst, boundRect[i], Scalar(0, 255, 0));
    }



    return dst;
}

int main(int argc, char* argv[])
{



    /*���D�@  :  Ū�J"img0001-spnoise.bmp"�A����ܦb"Input"�����C*/
    Mat image1 = Problem1(argc, argv);
    /*���D�G  :  �Q��medianBlur()��"img0001-gray.bmp"�i����Q���T�����A����ܦb"Noise removed"�����C*/
    Mat noiseremoved = Problem2(image1);
    /*���D�T  :  Ū�J"lightpattern.bmp" �A����ܦb" Background "�����C*/
    Mat image2 = Problem3(argc, argv);
    /*���D�|  :  �Q��absdiff()�P�I���v���۴�A�i��I�������A����ܦb"BG removed"�����C*/
    Mat bgremoved = Problem4(noiseremoved, image2);
    /*���D��  :  �Q��threshold()�i��G�ȤƳB�z�A����ܦb"Threshold"�����C*/
    Mat thresho = Problem5(bgremoved);
    /*���D��  :  �Q��connectedComponents()�i�椣�P����d����μЩw�A����ܦb" connectedComponents"�����C*/
    Mat connectedCo = Problem6(thresho);
    /*���D�C  :  �Q��findContours()��X�C�Ӫ��󪺽����A�A�N�]�t�C�Ӫ��󪺽������̤p�x�εe�b��l�ϤW�A����ܦb"Result"�����C
*///�Ѯv�����ΰ�
    //Mat result = Problem7_2(connectedCo);
    vector   <Mat>   name    = { image1,noiseremoved,image2,bgremoved,thresho,connectedCo};
    vector <String> Filename = { "Input","Noise removed","Background","BG removed","Threshold","connectedComponents" };
    
    for (int i = 0; i < 6; i++)
    {
        ostringstream ss;
        ss << Filename[i];
        if (i < 3)//�e�T�i��ܦb�W��
        {
            namedWindow(ss.str());
            moveWindow(ss.str(), 640 * i, 5);
            imshow(ss.str(), name[i]);
        }
        else
        {
            namedWindow(ss.str());
            moveWindow(ss.str(), 640 * (i-3), 485);
            imshow(ss.str(), name[i]);
        }
        
    }
    //imshow("Result", result);


    waitKey(0);
    return 0;

}

