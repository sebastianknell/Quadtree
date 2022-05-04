#include <iostream>
#include <opencv2/opencv.hpp>
#include "Quadtree.h"

using namespace std;

void test() {
    auto quadtree = new Quadtree(512);
    quadtree->insert(10, 2);
    quadtree->insert(-10, 2);
    quadtree->insert(-10, -2);
    quadtree->insert(10, -2);

    quadtree->insert(4, 8);
}

const int width = 512;
int mouseX, mouseY;

static void clickHandler(int event, int x, int y, int, void*) {
    if (event == cv::EVENT_LBUTTONDOWN)
        cout << x << " " << y << endl;
}

int main() {
    cv::Mat img(width, width, CV_8UC3, cv::Scalar(255, 255, 255));
    cv::imshow("img", img);
    cv::setWindowProperty("img", cv::WND_PROP_TOPMOST, 1);
    cv::setMouseCallback("img", clickHandler);
    cv::waitKey(4000);

    cv::rectangle( img, cv::Point(0, 0), cv::Point(width, width), cv::Scalar(0, 0, 0), 2);
    cv::imshow("img", img);
    cv::waitKey(2000);

    cv::rectangle( img, cv::Point(0, 0), cv::Point(width/2, width/2), cv::Scalar(0, 0, 0), 2);
    cv::imshow("img", img);
    cv::waitKey(2000);
    cv::rectangle( img, cv::Point(width/2, 0), cv::Point(width, width/2), cv::Scalar(0, 0, 0), 2);
    cv::imshow("img", img);
    cv::waitKey(2000);

    return 0;
}