#include "mygl.h"
#include <la.h>
#include <iostream>
#include <QApplication>
#include <QKeyEvent>
#include "scene/translatenode.h"
#include "scene/rotatenode.h"
#include "scene/scalenode.h"

MyGL::MyGL(QWidget *parent)
    : OpenGLContext(parent),
      prog_flat(this),
      m_geomGrid(this), m_geomSquare(this, {glm::vec3(0.5f, 0.5f, 1.f),
                                            glm::vec3(-0.5f, 0.5f, 1.f),
                                            glm::vec3(-0.5f, -0.5f, 1.f),
                                            glm::vec3(0.5f, -0.5f, 1.f)}),
      m_geomCircle(this, 360),
      m_geomTriangle(this, 3),
      m_geomPenta(this, 5),
      m_showGrid(true),
      mp_selectedNode(nullptr),
    root()
{
    setFocusPolicy(Qt::StrongFocus);
}

MyGL::~MyGL()
{
    makeCurrent();

    glDeleteVertexArrays(1, &vao);
    m_geomSquare.destroy();
    m_geomGrid.destroy();
}

void MyGL::initializeGL()
{
    // Create an OpenGL context using Qt's QOpenGLFunctions_3_2_Core class
    // If you were programming in a non-Qt context you might use GLEW (GL Extension Wrangler)instead
    initializeOpenGLFunctions();
    // Print out some information about the current OpenGL context
    debugContextVersion();

    // Set a few settings/modes in OpenGL rendering
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    // Set the size with which points should be rendered
    glPointSize(5);
    // Set the color with which the screen is filled at the start of each render call.
    glClearColor(135.f / 256.f, 206.f / 256.f, 235.f / 256.f, 1);

    printGLErrorLog();

    // Create a Vertex Attribute Object
    glGenVertexArrays(1, &vao);

    //Create the scene geometry
    m_geomGrid.create();
    m_geomSquare.create();
    m_geomCircle.create();
    m_geomTriangle.create();
    m_geomPenta.create();

    // Create and set up the flat lighting shader
    prog_flat.create(":/glsl/flat.vert.glsl", ":/glsl/flat.frag.glsl");

    // We have to have a VAO bound in OpenGL 3.2 Core. But if we're not
    // using multiple VAOs, we can just bind one once.
    glBindVertexArray(vao);


    // TODO: Call your scene graph construction function here
    root = constructSceneGraph();
    emit this->sig_sendRootNode(root.get());
}

uPtr<Node> MyGL::constructSceneGraph() {
//    /// create left forearm
    uPtr<Node> leftForeArm = mkU<TranslateNode>(0.5, 0, "Left Forearm Translate 1", &(this->m_geomSquare), glm::vec3(0, 0, 1));
    /// create right forearm
    uPtr<Node> rightForeArm = mkU<TranslateNode>(-0.5, 0, "Right Forearm Translate 1", &(this->m_geomSquare), glm::vec3(0, 0, 1));

    uPtr<Node> s1 = mkU<ScaleNode>(1, 0.5, "Left Forearm Scale 1");
    s1->addChild(leftForeArm.get());
    s1->addChildren(std::move(leftForeArm));

    uPtr<Node> s2 = mkU<ScaleNode>(1, 0.5, "Right Forearm Scale 1");
    s2->addChild(rightForeArm.get());
    s2->addChildren(std::move(rightForeArm));

    uPtr<Node> r1 = mkU<RotateNode>(30, "Left Forearm Rotate 1");
    r1->addChild(s1.get());
    r1->addChildren((std::move(s1)));

    uPtr<Node> r2 = mkU<RotateNode>(330, "Right Forearm Rotate 1");
    r2->addChild(s2.get());
    r2->addChildren((std::move(s2)));

    // connection between leftArm - leftForearm and rightArm - rightForearm
    uPtr<Node> t1 = mkU<TranslateNode>(1, 0, "Left Forearm Translate 2");
    t1->addChild(r1.get());
    t1->addChildren(std::move(r1));

    uPtr<Node> t2 = mkU<TranslateNode>(-1, 0, "Right Forearm Translate 2");
    t2->addChild(r2.get());
    t2->addChildren(std::move(r2));

    // create left arm
    uPtr<Node> leftArm = mkU<TranslateNode>(0.5, 0, "Left Arm Translate 1", &(this->m_geomSquare), glm::vec3(0, 1, 0));
    /// create right arm
    uPtr<Node> rightArm = mkU<TranslateNode>(-0.5, 0, "Right Arm Translate 1", &(this->m_geomSquare), glm::vec3(0, 1, 0));

    uPtr<Node> s3 = mkU<ScaleNode>(1, 0.5, "Left Arm Scale 1");
    s3->addChild(leftArm.get());
    s3->addChildren(std::move(leftArm));

    uPtr<Node> s4 = mkU<ScaleNode>(1, 0.5, "Right Arm Scale 1");
    s4->addChild(rightArm.get());
    s4->addChildren(std::move(rightArm));

    uPtr<Node> r3 = mkU<RotateNode>(30, "Left Arm Rotate 1");
    r3->addChild(s3.get());
    r3->addChild(t1.get());
    r3->addChildren((std::move(s3)));
    r3->addChildren(std::move(t1));

    uPtr<Node> r4 = mkU<RotateNode>(330, "Right Arm Rotate 1");
    r4->addChild(s4.get());
    r4->addChild(t2.get());
    r4->addChildren(std::move(s4));
    r4->addChildren(std::move(t2));

    // connection between root - leftArm and root - rightArm
    uPtr<Node> t3 = mkU<TranslateNode>(0.5, 0.5, "Left Arm Translate 2");
    t3->addChild(r3.get());
    t3->addChildren(std::move(r3));

    uPtr<Node> t4 = mkU<TranslateNode>(-0.5, 0.5, "Right Arm Translate 2");
    t4->addChild(r4.get());
    t4->addChildren(std::move(r4));

    /// create head
    uPtr<Node> head = mkU<ScaleNode>(0.5, 0.5, "Head Scale 1", &(this->m_geomSquare), glm::vec3(0.7, 0.7, 0));

    uPtr<Node> r5 = mkU<RotateNode>(45, "Head Rotate 1");
    r5->addChild(head.get());
    r5->addChildren(std::move(head));

    //connection between root - head
    uPtr<Node> t5 = mkU<TranslateNode>(0, 0.5, "Head Translate 1");
    t5->addChild(r5.get());
    t5->addChildren(std::move(r5));

    /// create body
    uPtr<Node> body = mkU<ScaleNode>(1, 1, "Body Scale 1", &(this->m_geomSquare), glm::vec3(1, 0, 0));

    /// Root transformations
    uPtr<Node> rootT = mkU<TranslateNode>(0, 0, "Root Translate");
    uPtr<Node> rootR = mkU<RotateNode>(0, "Root Rotate");
    uPtr<Node> rootS = mkU<ScaleNode>(1, 1, "Root Scale");

    rootS->addChild(body.get());
    rootS->addChildren(std::move(body));
    rootS->addChild(t5.get());
    rootS->addChildren(std::move(t5));
    rootS->addChild(t3.get());
    rootS->addChildren(std::move(t3));
    rootS->addChild(t4.get());
    rootS->addChildren(std::move(t4));

    rootR->addChild(rootS.get());
    rootR->addChildren(std::move(rootS));
    rootT->addChild(rootR.get());
    rootT->addChildren(std::move(rootR));

    return rootT;

/// Test Copy Constructor
/// One remaining issue: QTreeWidget will not display the name of the children of the copied Node
#if 0
    uPtr<Node> root1 = mkU<TranslateNode>(0, 0, "Root1 Translate");
    uPtr<Node> cir = mkU<TranslateNode>(2, 0, "Test Circle", &(this->m_geomCircle), glm::vec3(0, 1, 0));
    uPtr<Node> tri = mkU<TranslateNode>(-2, 0, "Test Triangle", &(this->m_geomTriangle), glm::vec3(0, 0, 1));
    root1->addChild(cir.get());
    Node &testnode = root1->addChildren(std::move(cir));
    root1->addChild(tri.get());
    root1->addChildren(std::move(tri));

    uPtr<Node> t = mkU<TranslateNode>(dynamic_cast<const TranslateNode&>(*(root1.get())));
    uPtr<Node> root2 = mkU<TranslateNode>(0, 3, "Test copy Root2 Translate");
    root2->addChild(t.get());
    Node &copynode = root2->addChildren(std::move(t));

    uPtr<Node> root = mkU<TranslateNode>(0, 0, "Root Translate");
    root->addChild(root1.get());
    root->addChildren(std::move(root1));
    root->addChild(root2.get());
    root->addChildren(std::move(root2));

    testnode.setColor(glm::vec3(0, 1, 1));

    std::cout << copynode.getName().toStdString() << std::endl;
    for (const uPtr<Node> &n : copynode.getChildren()) {
        std::cout << n->getName().toStdString() << std::endl;
    }

    return root;
#endif
}

void MyGL::traverseSceneGraph(const uPtr<Node> &node, glm::mat3 trans) {
    trans *= node->getTransformMatrix();
    for (const uPtr<Node> &n : node->getChildren()) {
        traverseSceneGraph(n, trans);
    }
    if (node->getGeometry() != nullptr) {
        node->getGeometry()->setColor(node->getColor());
        prog_flat.setModelMatrix(trans);
        prog_flat.draw(*this, *(node->getGeometry()));
    }
}

void MyGL::resizeGL(int w, int h)
{
    glm::mat3 viewMat = glm::scale(glm::mat3(), glm::vec2(0.2, 0.2)); // Screen is -5 to 5

    // Upload the view matrix to our shader (i.e. onto the graphics card)
    prog_flat.setViewMatrix(viewMat);

    printGLErrorLog();
}

// This function is called by Qt any time your GL window is supposed to update
// For example, when the function update() is called, paintGL is called implicitly.
void MyGL::paintGL()
{
    // Clear the screen so that we only see newly drawn images
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (m_showGrid)
    {
        prog_flat.setModelMatrix(glm::mat3());
        prog_flat.draw(*this, m_geomGrid);
    }

    //VVV CLEAR THIS CODE WHEN YOU IMPLEMENT SCENE GRAPH TRAVERSAL VVV///////////////////

    // Shapes will be drawn on top of one another, so the last object
    // drawn will appear in front of everything else
#if 0
    prog_flat.setModelMatrix(glm::mat3());
    m_geomSquare.setColor(glm::vec3(0,1,0));
    prog_flat.draw(*this, m_geomSquare);

    m_geomSquare.setColor(glm::vec3(1,0,0));
    prog_flat.setModelMatrix(glm::translate(glm::mat3(), glm::vec2(-1.f, 0.f)) * glm::rotate(glm::mat3(), glm::radians(-30.f)));
    prog_flat.draw(*this, m_geomSquare);

    m_geomSquare.setColor(glm::vec3(0,0,1));
    prog_flat.setModelMatrix(glm::translate(glm::mat3(), glm::vec2(1.f, 0.f)) * glm::rotate(glm::mat3(), glm::radians(30.f)));
    prog_flat.draw(*this, m_geomSquare);

    //^^^ CLEAR THIS CODE WHEN YOU IMPLEMENT SCENE GRAPH TRAVERSAL ^^^/////////////////
#endif
    // Here is a good spot to call your scene graph traversal function.
    // Any time you want to draw an instance of geometry, call
    // prog_flat.draw(*this, yourNonPointerGeometry);
    traverseSceneGraph(this->root, glm::mat3());
}

void MyGL::keyPressEvent(QKeyEvent *e)
{
    // http://doc.qt.io/qt-5/qt.html#Key-enum
    switch(e->key())
    {
    case(Qt::Key_Escape):
        QApplication::quit();
        break;

    case(Qt::Key_G):
        m_showGrid = !m_showGrid;
        break;
    }
}

void MyGL::slot_setSelectedNode(QTreeWidgetItem *i) {
    mp_selectedNode = static_cast<Node*>(i);

    /// grey out spinboxes based on the type of node that is selected
    switch (mp_selectedNode->getNodeType()) {
    case 1:
        emit this->sig_onlyTranslateSpinbox();
        break;
    case 2:
        emit this->sig_onlyRotateSpinbox();
        break;
    case 3:
        emit this->sig_onyScaleSpinbox();
        break;
    default:
        emit this->sig_enableAllSpinbox();
    }
}

void MyGL::slot_setTX(double x) {
    // TODO update the currently selected Node's
    // X translation value IF AND ONLY IF
    // the currently selected node can be validly
    // dynamic_cast to a TranslateNode.
    // Remember that a failed dynamic_cast
    // will return a null pointer.
    if (mp_selectedNode != nullptr) {
        TranslateNode *transNode = dynamic_cast<TranslateNode*>(this->mp_selectedNode);
        transNode->setSpinboxX(x);
    }
}

void MyGL::slot_setTY(double y) {
    if (mp_selectedNode != nullptr) {
        TranslateNode *transNode = dynamic_cast<TranslateNode*>(this->mp_selectedNode);
        transNode->setSpinboxY(y);
    }
}

void MyGL::slot_setR(double r) {
    if (mp_selectedNode != nullptr) {
        RotateNode *rotNode = dynamic_cast<RotateNode*>(this->mp_selectedNode);
        rotNode->setSpinboxRad(r);
    }
}

void MyGL::slot_setSX(double x) {
    if (mp_selectedNode != nullptr) {
        ScaleNode *scaNode = dynamic_cast<ScaleNode*>(this->mp_selectedNode);
        scaNode->setSpinboxX(x);
    }
}

void MyGL::slot_setSY(double y) {
    if (mp_selectedNode != nullptr) {
        ScaleNode *scaNode = dynamic_cast<ScaleNode*>(this->mp_selectedNode);
        scaNode->setSpinboxY(y);
    }
}

void MyGL::slot_addTranslateNode() {
    // TODO invoke the currently selected Node's
    // addChild function on a newly-instantiated
    // TranslateNode.
    uPtr<Node> newchild = mkU<TranslateNode>(0, 0, "New Translate Node");
    this->mp_selectedNode->addChild(newchild.get());
    this->mp_selectedNode->addChildren(std::move(newchild));
}

void MyGL::slot_addRotateNode() {
    // TODO invoke the currently selected Node's
    // addChild function on a newly-instantiated
    // TranslateNode.
    uPtr<Node> newchild = mkU<RotateNode>(0, "New Rotate Node");
    this->mp_selectedNode->addChild(newchild.get());
    this->mp_selectedNode->addChildren(std::move(newchild));
}

void MyGL::slot_addScaleNode() {
    // TODO invoke the currently selected Node's
    // addChild function on a newly-instantiated
    // TranslateNode.
    uPtr<Node> newchild = mkU<ScaleNode>(1, 1, "New Scale Node");
    this->mp_selectedNode->addChild(newchild.get());
    this->mp_selectedNode->addChildren(std::move(newchild));
}

void MyGL::slot_setGeometry() {
    this->mp_selectedNode->setGeometry(&(this->m_geomSquare));
    this->mp_selectedNode->setColor(glm::vec3(1, 0, 0.765));
}
