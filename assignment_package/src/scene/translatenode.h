#pragma once
#include "node.h"

class TranslateNode : public Node {
private:
    glm::vec2 transCoords;
    glm::vec2 spinboxCoords;

public:
    TranslateNode(float x, float y, QString name, Polygon2D *geo = nullptr, glm::vec3 color = glm::vec3(1, 0, 0));
    TranslateNode(const TranslateNode &trans);
    TranslateNode& operator=(const TranslateNode &node);
    virtual ~TranslateNode() {};

    void setSpinboxX(double x);
    void setSpinboxY(double y);
    glm::mat3 getTransformMatrix() const override;
    int getNodeType() const override;
};
