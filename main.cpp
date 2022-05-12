#include "Quadtree.h"

const int width = 512;
const string windowName = "Quadtree Visualization";
cv::Mat img(width, width, CV_8UC3, cv::Scalar(255, 255, 255));
Quadtree quadtree(width);

static void clickHandler(int event, int x, int y, int, void*) {
    if (event == cv::EVENT_LBUTTONDOWN) {
        cout << x << " " << y << endl;
        quadtree.insert({x, y});
        img.setTo(cv::Scalar(255, 255, 255));
        quadtree.show(img);
        cv::imshow(windowName, img);
    }
    if (event == cv::EVENT_RBUTTONDOWN) {
        quadtree.remove({x, y});
        img.setTo(cv::Scalar(255, 255, 255));
        quadtree.show(img);
        cv::imshow(windowName, img);
    }
}

int main() {
    cv::imshow(windowName, img);
    cv::setWindowProperty(windowName, cv::WND_PROP_TOPMOST, 1);
    cv::setMouseCallback(windowName, clickHandler);

    char c;
    do {
        c = (char)cv::waitKey(0);
    } while (c != 'q');

    return 0;
}
