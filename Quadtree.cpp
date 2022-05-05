//
// Created by Sebastian Knell on 30/04/22.
//

#include "Quadtree.h"

static void show(Node* node, cv::InputOutputArray &img) {
    if (!node) return;
    auto x = node->square.x;
    auto y = node->square.y;
    auto w = node->square.w;
    cv::rectangle(img, cv::Point(x, y), cv::Point(x + w - 1, y + w - 1), cv::Scalar(0, 0, 0), 2);
    if (node->point.has_value()) {
        auto point = node->point.value();
        cv::circle(img, cv::Point(point.x, point.y), 4, cv::Scalar(0, 0, 255), -1);
    }
    else if (node->isDivided){
        ::show(node->ne, img);
        ::show(node->se, img);
        ::show(node->sw, img);
        ::show(node->nw, img);
    }
}

static bool operator==(Point &a, Point &b) {
    return a.x == b.x && a.y == b.y;
}

Node::Node(Square s): isDivided(false), square(s) {
    cout << "New node with w=" << s.w << " x=" << s.x << " y=" << s.y << endl;
}

Node::Node(Point p, Square s): isDivided(false), square(s) {
    point = p;
    cout << "New node with w=" << s.w << " x=" << s.x << " y=" << s.y << " and point = (" << p.x << ", " << p.y << ")" << endl;
}

Node::~Node() {
    delete ne;
    delete se;
    delete sw;
    delete nw;
}

Quadtree::Quadtree(int gridSize): gridSize(gridSize) {
    root = nullptr;
    minGridSize = 32;
}

Quadtree::~Quadtree() {
    delete root;
}

Node** Node::getQuadrant(Point p) {
    auto quadrantSize = square.w / 2;
    if (p.x >= quadrantSize && p.y >= quadrantSize)
        return &ne;
    if(p.x >= quadrantSize && p.y < quadrantSize)
        return &se;
    if(p.x < quadrantSize && p.y >= quadrantSize)
        return &sw;
    else
        return &nw;
}

void Quadtree::insert(Point point) {
    if (!root) {
        root = new Node(point, {gridSize, 0, 0});
        return;
    }
    auto curr = root;
    while (curr && curr->isDivided) {
        auto quadrant = curr->getQuadrant(point);
        if (!(*quadrant)->isDivided && !(*quadrant)->point.has_value()) {
            (*quadrant)->point = point;
            return;
        }
        curr = *quadrant;
    }
    if (!curr) return;
    // Dividir
    Point currPoint;
    if (curr->point.has_value())
        currPoint = curr->point.value();
    else return;
    if (currPoint == point) return;
    curr->point = nullopt;
    Node **quadrant1, **quadrant2;
    do {
        auto newWidth = curr->square.w/2;
        auto x = curr->square.x;
        auto y = curr->square.y;
        if (newWidth <= minGridSize) return;
        curr->ne = new Node({newWidth, x + newWidth, y});
        curr->se = new Node({newWidth, x + newWidth, y + newWidth});
        curr->sw = new Node({newWidth, x, y + newWidth});
        curr->nw = new Node({newWidth, x, y});
        curr->isDivided = true;
        quadrant1 = curr->getQuadrant(currPoint);
        quadrant2 = curr->getQuadrant(point);
        curr = *quadrant1;
    } while (*quadrant1 == *quadrant2);
    // Mover el punto actual a un nuevo nodo e insertar el nuevo
    (*quadrant1)->point = currPoint;
    (*quadrant2)->point = point;
}

void Quadtree::insert(int x, int y) {
    insert({x, y});
}

void Quadtree::show(cv::InputOutputArray &img) {
    ::show(root, img);
}
