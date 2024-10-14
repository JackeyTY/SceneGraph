#pragma once
#include <QTreeWidgetItem>
#include <vector>
#include "smartpointerhelp.h"
#include "polygon.h"

class Node : public QTreeWidgetItem {
private:
    std::vector<uPtr<Node>> children;
    Polygon2D *geometry;
    glm::vec3 color;
    QString name;

protected:
    void copyNode(const Node &node);
    void copyChildren(const Node &node);

public:
    Node(Polygon2D *geo, QString name, glm::vec3 color);
    Node(const Node &node);
    Node& operator=(const Node &node);
    virtual ~Node() {};

    const std::vector<uPtr<Node>>& getChildren();
    void setGeometry(Polygon2D *geo);
    Polygon2D* getGeometry() const;
    void setColor(glm::vec3 c);
    glm::vec3 getColor() const;
    void setName(QString n);
    QString getName() const;

    virtual int getNodeType() const = 0;
    virtual glm::mat3 getTransformMatrix() const = 0;
    Node& addChildren(uPtr<Node>);
};
