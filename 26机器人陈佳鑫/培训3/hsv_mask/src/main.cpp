#include <opencv2/opencv.hpp>
using namespace cv;

int main()
{
    Mat img = imread("images/armor.png");
    Mat hsv;
    cvtColor(img, hsv, COLOR_BGR2HSV);

    Mat ch[3];
    split(hsv, ch);
    imshow("H", ch[0]);
    imshow("S", ch[1]);
    imshow("V", ch[2]);

    Mat m1, m2;
    inRange(hsv, Scalar(0, 80, 80), Scalar(10, 255, 255), m1);
    inRange(hsv, Scalar(170, 80, 80), Scalar(179, 255, 255), m2);
    Mat mask = m1 + m2;

    imshow("red_mask", mask);
    imwrite("results/red_mask.png", mask);
    waitKey(0);
    return 0;
}
