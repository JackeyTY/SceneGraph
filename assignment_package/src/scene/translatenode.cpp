#include "translatenode.h"
#include <glm/gtx/matrix_transform_2d.hpp>

TranslateNode::TranslateNode(float x, float y, QString name, Polygon2D *geo, glm::vec3 color)
    : Node(geo, name, color), transCoords(x, y), spinboxCoords(0, 0) {}

TranslateNode::TranslateNode(const TranslateNode &trans)
    : Node(static_cast<const Node&>(trans)), transCoords(trans.transCoords), spinboxCoords(trans.spinboxCoords) {}

TranslateNode& TranslateNode::operator=(const TranslateNode &trans) {
    this->copyNode(static_cast<const Node&>(trans));
    this->transCoords = trans.transCoords;
    this->spinboxCoords = trans.spinboxCoords;
    return *this;
}

void TranslateNode::setSpinboxX(double x) {
    this->spinboxCoords[0] = x;
}

void TranslateNode::setSpinboxY(double y) {
    this->spinboxCoords[1] = y;
}

glm::mat3 TranslateNode::getTransformMatrix() const {
    return glm::translate(glm::mat3(), this->transCoords + this->spinboxCoords);
}

int TranslateNode::getNodeType() const {
    return 1;
}
