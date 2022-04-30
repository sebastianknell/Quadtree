//
// Created by Sebastian Knell on 30/04/22.
//

#ifndef QUADTREE_QUADTREE_H
#define QUADTREE_QUADTREE_H

#include <cmath>

using Point = struct{double x; double y;};

struct Node {
    Point point;
    bool isLeaf;
    Node* ne = nullptr;
    Node* se = nullptr;
    Node* sw = nullptr;
    Node* nw = nullptr;
    Node() = default;
    explicit Node(Point);
    Node** getQuadrant(Point, unsigned long);
};

class Quadtree {
    Node* root;
    unsigned long initialGridSize = 16;
    int minGridSize = 1;
public:
    Quadtree();
    void insert(Point point);
    void insert(double x, double y);
    void remove(Point point);
    void remove(double x, double y);
};


#endif //QUADTREE_QUADTREE_H
