#include <iostream>
//#include <opencv2/opencv.hpp>
#include "Quadtree.h"

using namespace std;

int main() {
    auto quadtree = new Quadtree();
    quadtree->insert(10, 2);
    quadtree->insert(-10, 2);
    quadtree->insert(-10, -2);
    quadtree->insert(10, -2);

    quadtree->insert(4, 8);
    return 0;
}
