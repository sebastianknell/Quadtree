//
// Created by Sebastian Knell on 30/04/22.
//

#ifndef QUADTREE_QUADTREE_H
#define QUADTREE_QUADTREE_H

#include <cmath>

// mouse listener gives integers
using Point = struct{int x; int y;};

struct Node {
    Point point;
    bool isLeaf;
    Node* ne = nullptr;
    Node* se = nullptr;
    Node* sw = nullptr;
    Node* nw = nullptr;
    Node() = default;
    explicit Node(Point);
    Node** getQuadrant(Point, int);
};

class Quadtree {
    Node* root;
    int initialGridSize;
    int minGridSize;
public:
    Quadtree(int gridSize);
    void insert(Point point);
    void insert(int x, int y);
    void remove(Point point);
    void remove(int x, int y);
};


#endif //QUADTREE_QUADTREE_H
