//
// Created by Sebastian Knell on 30/04/22.
//

#ifndef QUADTREE_QUADTREE_H
#define QUADTREE_QUADTREE_H

#include <iostream>
#include <optional>
#include <stack>
#include <cmath>
#include <opencv2/opencv.hpp>

using namespace std;

// mouse listener gives integers
using Point = struct{int x; int y;};
using Rectangle = struct{int w; int h; int x; int y;};
extern int radius;

struct Node {
    Rectangle rectangle;
    optional<Point> point;
    bool isDivided;
    Node* ne = nullptr;
    Node* se = nullptr;
    Node* sw = nullptr;
    Node* nw = nullptr;
    explicit Node(Rectangle);
    explicit Node(Point, Rectangle);
    ~Node();
    Node** getQuadrant(Point);
};

class Quadtree {
    Node* root;
    int gridSize;
    int minGridSize;
public:
    explicit Quadtree(int gridSize);
    ~Quadtree();
    void insert(Point point);
    void insert(int x, int y);
    void remove(Point point);
    void remove(int x, int y);
    void show(cv::InputOutputArray);
};


#endif //QUADTREE_QUADTREE_H
