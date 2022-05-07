//
// Created by Sebastian Knell on 30/04/22.
//

#include "Quadtree.h"

int radius = 4;

static void show(Node* node, cv::InputOutputArray &img) {
    if (!node) return;
    auto x = node->square.x;
    auto y = node->square.y;
    auto w = node->square.w;
    cv::rectangle(img, cv::Point(x, y), cv::Point(x + w - 1, y + w - 1), cv::Scalar(0, 0, 0), 2);
    if (node->point.has_value()) {
        auto point = node->point.value();
        cv::circle(img, cv::Point(point.x, point.y), radius, cv::Scalar(0, 0, 255), -1);
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

static bool isInCircle(Point p, Point circleP, int radius) {
    return (pow(p.x - circleP.x, 2) + pow(p.y - circleP.y, 2)) <= pow(radius, 2);
}

Node::Node(Square s): isDivided(false), square(s) {}

Node::Node(Point p, Square s): isDivided(false), square(s) {
    point = p;
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
    auto x = square.x;
    auto y = square.y;
    if (p.x >= quadrantSize + x && p.y >= quadrantSize + y)
        return &se;
    if(p.x >= quadrantSize + x && p.y < quadrantSize + y)
        return &ne;
    if(p.x < quadrantSize + x && p.y >= quadrantSize + y)
        return &sw;
    if (p.x < quadrantSize + x && p.y < quadrantSize + y)
        return &nw;
    return nullptr;
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
    Node **quadrant1, **quadrant2;
    do {
        auto newWidth = curr->square.w/2;
        auto x = curr->square.x;
        auto y = curr->square.y;
        if (newWidth < minGridSize) return;
        curr->point = nullopt;
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

void Quadtree::remove(Point point) {
    auto curr = root;
    stack<Node*> fathers;
    while (curr) {
        if (!curr->isDivided && curr->point.has_value()) {
            if (isInCircle(point, curr->point.value(), radius)) {
                curr->point = nullopt;
                if (curr == root) {
                    delete root;
                    root = nullptr;
                    return;
                }
                break;
            }
            else break;
        }
        else if (curr->isDivided) {
            auto quadrant = curr->getQuadrant(point);
            if (*quadrant == nullptr) return;
            fathers.push(curr);
            curr = *quadrant;
        }
        else break;
    }
    while (!fathers.empty()) {
        curr = fathers.top();
        fathers.pop();
        // Si ninguno esta dividido
        if (!curr->ne->isDivided && !curr->se->isDivided && !curr->sw->isDivided && !curr->nw->isDivided) {
            // Si solo hay punto en uno unir
            auto points = 0;
            Point p;
            if (curr->ne->point.has_value()) {
                points++;
                p = curr->ne->point.value();
            }
            if (curr->se->point.has_value()) {
                points++;
                p = curr->se->point.value();
            }
            if (curr->sw->point.has_value()) {
                points++;
                p = curr->sw->point.value();
            }
            if (curr->nw->point.has_value()) {
                points++;
                p = curr->nw->point.value();
            }
            if (points <= 1) {
                delete curr->ne;
                delete curr->se;
                delete curr->sw;
                delete curr->nw;
                curr->ne = nullptr;
                curr->se = nullptr;
                curr->sw = nullptr;
                curr->nw = nullptr;
                curr->point = p;
                curr->isDivided = false;
            }
            else break;
        }
        else break;
    }
}

void Quadtree::remove(int x, int y) {
    remove({x, y});
}

void Quadtree::show(cv::InputOutputArray &img) {
    ::show(root, img);
}
