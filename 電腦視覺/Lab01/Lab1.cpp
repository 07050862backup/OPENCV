#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
    /*���D�@:�Q�Τ@��Mat���غc�l�ӫإߤ@�j�p��320x240����q�D�Ƕ��v���x�}image1�A �C�ӳq�D�����ACV_8U�A�ñN�C�ӹ������ȳ]100�C
�M��A�Q��imshow()��ܸӼv��, �èϥ�waitKey(0)���ݫ����N��C*/
    cv::Mat image1(240, 320, CV_8UC1, Scalar(100));
    imshow("image1", image1);
    waitKey(0);

    /*���D�G:�Q��Mat.create()�禡�A���s�t�mimage1�A�Ϩ�j�p��200x200�C�A�N�Ҧ��������ȳ]150 �C
�M��A�Q��imshow()��ܸӼv��, �èϥ�waitKey(0)���ݫ����N��C
*/
    image1.create(200, 200, CV_8UC1);
    image1.setTo(cv::Scalar(150));
    imshow("image1", image1);
    waitKey(0);

    /*���D�T:�Q�Τ@��Mat���غc�l�ӫإߤ@�j�p��320x240���T�q�D�m��v���x�}image2�A �C�ӳq�D�����ACV_8U�A�q�D�����Ǭ�BGR�A�ñN�C�ӹ������C��]�¬���C
�M��A�Q��imshow()��ܸӼv��, �èϥ�waitKey(0)���ݫ����N��C
*/
    Mat image2(Size(320, 240), CV_8UC3);
    image2 = Scalar(0, 0, 255);
    imshow("image2", image2);
    waitKey(0);

    /*���D�|:�Q��imreadŪ��"boldt.jpg" �s��image3�C
�M��A�Nimage1��image4���]�w�����Vimage3����ư϶��C
�Nimage2��image5�ƻs��image3�����G�A���W�ߩ�image3����ư϶��C
�M��A����cv::flip(image3,image3,1);
�Q��imshow()��ܤ��O���image1, image2, image3, image4��image5�A�[��̤������t�O, �èϥ�waitKey(0)���ݫ����N��C*/

    Mat image3;
    image3 = imread("boldt.jpg", IMREAD_COLOR);
    imshow("image3", image3);
    waitKey(0);


    // 1 4���V3  ����ؼg�k
    //Mat image4, image5;
    cv::Mat image4(image3);
    image1 = image3;
    // 2 5�ƻs3   ����ؼg�k
    image3.copyTo(image2);
    cv::Mat image5 = image3.clone();
    imshow("image1", image1);
    imshow("image2", image2);
    imshow("image3", image3);
    imshow("image4", image4);
    imshow("image5", image5);
    waitKey(0);


    cv::flip(image3, image3, 1);
    imshow("image1", image1);
    imshow("image2", image2);
    imshow("image3", image3);
    imshow("image4", image4);
    imshow("image5", image5);
    waitKey(0);
    /*���D��:�Q��imread�H�Ƕ��v�����覡Ū��"boldt.jpg" �s��image6�C
�M��A�Q��convertTo()�禡�Nimage6�ন���ث��A��CV_32F���B�I�Ưx�}�A�ϨC�ӹ����Ȥ���[0,1]���d��C
�M��A�Q��imshow()��ܸӼv��image6, �èϥ�waitKey(0)���ݫ����N��C*/
    cv::Mat image6;
    image6 = imread("boldt.jpg", IMREAD_GRAYSCALE);


    
    image6.convertTo(image6, CV_32F);
    image6 = image6 / 255;
    
    imshow("image1", image1);
    imshow("image2", image2);
    imshow("image3", image3);
    imshow("image4", image4);
    imshow("image5", image5);
    imshow("image6", image6);
    waitKey(0);
    return 0;
}

