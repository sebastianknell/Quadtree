//
// Created by Sebastian Knell on 30/04/22.
//

#include "Quadtree.h"

static void show(Node* node, cv::InputOutputArray &img) {
    if (!node) return;
    auto x = node->square.x;
    auto y = node->square.y;
    auto w = node->square.w;
    cv::rectangle(img, cv::Point(x, y), cv::Point(x + w, y + w), cv::Scalar(0, 0, 0), 2);
    if (node->hasPoint) {
        cv::circle(img, cv::Point(node->point.x, node->point.y), 4, cv::Scalar(0, 0, 255), -1);
    }
    else {
        ::show(node->ne, img);
        ::show(node->se, img);
        ::show(node->sw, img);
        ::show(node->nw, img);
    }
}

Node::Node(Square s): hasPoint(false), square(s) {}

Node::Node(Point p, Square s): hasPoint(true), point(p), square(s) {}

Quadtree::Quadtree(int gridSize): gridSize(gridSize) {
    root = nullptr;
    minGridSize = 32;
}

Node** Node::getQuadrant(Point p) {
    auto quadrantSize = square.w / 2;
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
        root = new Node(point, {gridSize, 0, 0});
    }
    auto curr = root;
    while (!curr->hasPoint) {
        auto quadrant = curr->getQuadrant(point);
        if (!(*quadrant)->hasPoint) {
            (*quadrant)->point = point;
            return;
        }
        curr = *quadrant;
    }
    // Dividir
    auto currPoint = curr->point;
    Node **quadrant1, **quadrant2;
    do {
        auto newWidth = curr->square.w/2;
        auto x = curr->square.x;
        auto y = curr->square.y;
        if (newWidth <= minGridSize) return;
        curr->ne = new Node({newWidth, x + newWidth/2, y});
        curr->se = new Node({newWidth, x + newWidth/2, y + newWidth/2});
        curr->sw = new Node({newWidth, x, y + newWidth/2});
        curr->nw = new Node({newWidth, x, y});
        curr->hasPoint = false;
        quadrant1 = curr->getQuadrant(currPoint);
        quadrant2 = curr->getQuadrant(point);
        curr = *quadrant1;
    } while (*quadrant1 == *quadrant2);
    // Mover el punto actual a un nuevo nodo e insertar el nuevo
    (*quadrant1)->point = currPoint;
    (*quadrant1)->hasPoint = true;
    (*quadrant2)->point = point;
    (*quadrant2)->hasPoint = true;
}

void Quadtree::insert(int x, int y) {
    insert({x, y});
}

void Quadtree::show(cv::InputOutputArray &img) {
    ::show(root, img);
}
