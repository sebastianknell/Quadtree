//
// Created by Sebastian Knell on 30/04/22.
//

#ifndef QUADTREE_QUADTREE_H
#define QUADTREE_QUADTREE_H

#include <cmath>

enum quadrant {
    NE,
    SE,
    SW,
    NW
};

using Point = struct{double x; double y;};

struct Node {
    Point point;
    bool isLeaf;
    unsigned long gridSize = 0;
    Node* ne = nullptr;
    Node* se = nullptr;
    Node* sw = nullptr;
    Node* nw = nullptr;
    Node() = default;
    explicit Node(Point);
};

class Quadtree {
    Node* root;
    unsigned long gridSize = pow(2, 42);
    int minGridSize = 1;
    quadrant getQuadrant(Point, unsigned long);
public:
    Quadtree();
    void insert(Point point);
    void remove(Point point);
};


#endif //QUADTREE_QUADTREE_H
