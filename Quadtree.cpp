//
// Created by Sebastian Knell on 30/04/22.
//

#include "Quadtree.h"

int radius = 4;

static void show(Node* node, cv::InputOutputArray &img) {
    if (!node) return;
    auto x = node->rectangle.x;
    auto y = node->rectangle.y;
    auto w = node->rectangle.w;
    auto h = node->rectangle.h;
    cv::rectangle(img, cv::Point(x, y), cv::Point(x + w - 1, y + h - 1), cv::Scalar(0, 0, 0), 2);
    if (node->point.has_value()) {
        auto point = node->point.value();
        cv::circle(img, cv::Point(point.x, point.y), radius, cv::Scalar(0, 0, 255), -1);
    }
    if (node->isDivided){
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

Node::Node(Rectangle s): isDivided(false), rectangle(s) {}

Node::Node(Point p, Rectangle s): isDivided(false), rectangle(s) {
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
//    auto quadrantSize = rectangle.w / 2;
    if (!point.has_value()) return nullptr;
    auto x = point.value().x;
    auto y = point.value().y;
    if (p.x >= x && p.y >= y)
        return &se;
    if(p.x >= x && p.y < y)
        return &ne;
    if(p.x < x && p.y >= y)
        return &sw;
    if (p.x < x && p.y < y)
        return &nw;
    return nullptr;
}

void Quadtree::insert(Point point) {
    if (!root) {
        root = new Node(point, {gridSize, gridSize, 0, 0});
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

    auto p = curr->point.value();
    auto rect = curr->rectangle;
    auto x = curr->rectangle.x;
    auto y = curr->rectangle.y;
//        if (newWidth < minGridSize) return;
    curr->ne = new Node({x + rect.w - p.x, p.y - rect.y, p.x, y});
    curr->se = new Node({x + rect.w - p.x, rect.y + rect.h - p.y, p.x, p.y});
    curr->sw = new Node({p.x - rect.x, rect.y + rect.h - p.y, x, p.y});
    curr->nw = new Node({p.x - rect.x, p.y - rect.y, x, y});
    curr->isDivided = true;
    auto quadrant = curr->getQuadrant(point);
    (*quadrant)->point = point;
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
