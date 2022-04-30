//
// Created by Sebastian Knell on 30/04/22.
//

#include "Quadtree.h"

Node::Node(Point p): isLeaf(true) {
    point = p;
}

Quadtree::Quadtree() {
    root = nullptr;
}

quadrant Quadtree::getQuadrant(Point p, unsigned long gridSize) {
    auto quadrantSize = gridSize / 2;
    if (p.x >= quadrantSize && p.y >= quadrantSize)
        return NE;
    if(p.x >= quadrantSize && p.y < quadrantSize)
        return SE;
    if(p.x < quadrantSize && p.y < quadrantSize)
        return SW;
    else
        return NW;
}

void Quadtree::insert(Point point) {
    if (!root) {
        root = new Node(point);
    }

}
