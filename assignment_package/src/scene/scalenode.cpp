#include "scalenode.h"
#include <glm/gtx/matrix_transform_2d.hpp>

ScaleNode::ScaleNode(float x, float y, QString name, Polygon2D *geo, glm::vec3 color)
    : Node(geo, name, color), scaCoords(x, y), spinboxCoords(1, 1) {}

ScaleNode::ScaleNode(const ScaleNode &sca)
    : Node(static_cast<const Node&>(sca)), scaCoords(sca.scaCoords), spinboxCoords(sca.spinboxCoords) {}

ScaleNode& ScaleNode::operator=(const ScaleNode &sca) {
    this->copyNode(static_cast<const Node&>(sca));
    this->scaCoords = sca.scaCoords;
    this->spinboxCoords = sca.spinboxCoords;
    return *this;
}

void ScaleNode::setSpinboxX(double x) {
    this->spinboxCoords[0] = x;
}

void ScaleNode::setSpinboxY(double y) {
    this->spinboxCoords[1] = y;
}

glm::mat3 ScaleNode::getTransformMatrix() const {
    return glm::scale(glm::mat3(), this->scaCoords * this->spinboxCoords);
}

int ScaleNode::getNodeType() const {
    return 3;
}
