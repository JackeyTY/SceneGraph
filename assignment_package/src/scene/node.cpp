#include "node.h"
#include "smartpointerhelp.h"
#include "translatenode.h"
#include "rotatenode.h"
#include "scalenode.h"

Node::Node(Polygon2D *geo, QString name, glm::vec3 color)
    : children(), geometry(geo),  color(color), name(name) {
    this->setText(0, this->name);
}

Node::Node(const Node &node)
    : children(), geometry(node.geometry), color(node.color), name(node.name) {
    copyChildren(node);
    this->setText(0, this->name);
}

Node& Node::operator=(const Node &node) {
    copyNode(node);
    this->setText(0, this->name);
    return *this;
}

void Node::copyNode(const Node &node) {
    this->geometry = node.geometry;
    this->color = node.color;
    this->name = node.name;
    this->children.clear();
    copyChildren(node);
}

void Node::copyChildren(const Node &node) {
    for (const uPtr<Node> &n : node.children) {
        uPtr<Node> childNode;
        switch (n->getNodeType()) {
        case 1:
            childNode = mkU<TranslateNode>(dynamic_cast<const TranslateNode&>(*(n.get())));
            break;
        case 2:
            childNode = mkU<RotateNode>(dynamic_cast<const RotateNode&>(*(n.get())));
            break;
        case 3:
            childNode = mkU<ScaleNode>(dynamic_cast<const ScaleNode&>(*(n.get())));
            break;
        }
        Node &child = this->addChildren(std::move(childNode));
        child.setText(0, child.getName());
    }
}

const std::vector<uPtr<Node>>& Node::getChildren() {
    return this->children;
}

void Node::setGeometry(Polygon2D *geo) {
    this->geometry = geo;
}

Polygon2D* Node::getGeometry() const {
    return this->geometry;
}

void Node::setColor(glm::vec3 c) {
    this->color = c;
}

glm::vec3 Node::getColor() const {
    return this->color;
}

void Node::setName(QString n) {
    this->name = n;
    this->setText(0, this->name);
}

QString Node::getName() const {
    return this->name;
}

Node& Node::addChildren(uPtr<Node> child) {
    Node &currentChild = *child;
    this->children.push_back(std::move(child));
    return currentChild;
}
