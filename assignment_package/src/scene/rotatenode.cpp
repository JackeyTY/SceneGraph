#include "rotatenode.h"
#include <glm/gtx/matrix_transform_2d.hpp>

RotateNode::RotateNode(float deg, QString name, Polygon2D *geo, glm::vec3 color)
    : Node(geo, name, color), rotRad(glm::radians(deg)), spinboxRad(0) {}

RotateNode::RotateNode(const RotateNode &rot)
    : Node(static_cast<const Node&>(rot)), rotRad(rot.rotRad), spinboxRad(rot.spinboxRad) {}

RotateNode& RotateNode::operator=(const RotateNode &rot) {
    this->copyNode(static_cast<const Node&>(rot));
    this->rotRad = rot.rotRad;
    this->spinboxRad = rot.spinboxRad;
    return *this;
}

void RotateNode::setSpinboxRad(double r) {
    this->spinboxRad = glm::radians(r);
}

glm::mat3 RotateNode::getTransformMatrix() const {
    return glm::rotate(glm::mat3(), this->rotRad + this->spinboxRad);
}

int RotateNode::getNodeType() const {
    return 2;
}
