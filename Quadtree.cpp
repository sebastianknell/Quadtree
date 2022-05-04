//
// Created by Sebastian Knell on 30/04/22.
//

#include "Quadtree.h"

Node::Node(Point p): isLeaf(true) {
    point = p;
}

Quadtree::Quadtree(int gridSize): initialGridSize(gridSize) {
    root = nullptr;
    minGridSize = 40;
}

Node** Node::getQuadrant(Point p, unsigned long gridSize) {
    // TODO fix comparation
    auto quadrantSize = gridSize / 2;
    if (p.x >= quadrantSize && p.y >= quadrantSize)
        return &ne;
    if(p.x >= quadrantSize && p.y < quadrantSize)
        return &se;
    if(p.x < quadrantSize && p.y < quadrantSize)
        return &sw;
    else
        return &nw;
}

void Quadtree::insert(Point point) {
    if (!root) {
        root = new Node(point);
    }
    auto curr = root;
    auto gridSize = initialGridSize;
    while (curr) {
        auto quadrant = curr->getQuadrant(point, gridSize);
        // TODO if quadrant <= minGridSize
        if (!(*quadrant)) {
            (*quadrant) = new Node(point);
            if (curr->isLeaf) {
                quadrant = curr->getQuadrant(curr->point, gridSize);
                (*quadrant) = new Node(curr->point);
                curr->isLeaf = false;
            }
            return;
        }
        curr = *quadrant;
        gridSize /= 2;
    }
}

void Quadtree::insert(double x, double y) {
    insert({x, y});
}
