#pragma once
#include "node.h"

class ScaleNode : public Node {
private:
    glm::vec2 scaCoords;
    glm::vec2 spinboxCoords;

public:
    ScaleNode(float x, float y, QString name, Polygon2D *geo = nullptr, glm::vec3 color = glm::vec3(1, 0, 0));
    ScaleNode(const ScaleNode &sca);
    ScaleNode& operator=(const ScaleNode &sca);
    virtual ~ScaleNode() {};

    void setSpinboxX(double x);
    void setSpinboxY(double y);
    glm::mat3 getTransformMatrix() const override;
    int getNodeType() const override;
};
