#pragma once
#include "node.h"

class RotateNode : public Node {
private:
    float rotRad;
    float spinboxRad;

public:
    RotateNode(float deg, QString name, Polygon2D *geo = nullptr, glm::vec3 color = glm::vec3(1, 0, 0));
    RotateNode(const RotateNode &rot);
    RotateNode& operator=(const RotateNode &rot);
    virtual ~RotateNode() {};

    void setSpinboxRad(double r);
    glm::mat3 getTransformMatrix() const override;
    int getNodeType() const override;
};
