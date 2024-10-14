#include "mainwindow.h"
#include <ui_mainwindow.h>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->mygl->setFocus();

    /// Stylesheet optimization
    this->setStyleSheet("background-color: beige;");
    ui->treeWidget->setHeaderLabel("Scene Graph Structure");
    ui->treeWidget->setStyleSheet("background-color: beige; "
                                  "selection-background-color: rgb(135, 206, 235); "
                                  "border-style: outset; "
                                  "border-width: 5px; "
                                  "border-radius: 10px; "
                                  "border-color: white;");
    ui->txLabel->setStyleSheet("border-style: outset; "
                               "border-width: 3px; "
                               "border-radius: 5px; "
                               "border-color: white;");
    ui->tyLabel->setStyleSheet("border-style: outset; "
                               "border-width: 3px; "
                               "border-radius: 5px; "
                               "border-color: white;");
    ui->rLabel->setStyleSheet( "border-style: outset; "
                               "border-width: 3px; "
                               "border-radius: 5px; "
                               "border-color: white;");
    ui->sxLabel->setStyleSheet("border-style: outset; "
                               "border-width: 3px; "
                               "border-radius: 5px; "
                               "border-color: white;");
    ui->syLabel->setStyleSheet("border-style: outset; "
                               "border-width: 3px; "
                               "border-radius: 5px; "
                               "border-color: white;");
    ui->tNodeAddButton->setStyleSheet("border-style: outset; "
                                      "border-width: 3px; "
                                      "border-radius: 5px; "
                                      "border-color: white;");
    ui->rNodeAddButton->setStyleSheet("border-style: outset; "
                                      "border-width: 3px; "
                                      "border-radius: 5px; "
                                      "border-color: white;");
    ui->sNodeAddButton->setStyleSheet("border-style: outset; "
                                      "border-width: 3px; "
                                      "border-radius: 5px; "
                                      "border-color: white;");
    ui->geomSetButton->setStyleSheet("border-style: outset; "
                                      "border-width: 3px; "
                                      "border-radius: 5px; "
                                      "border-color: white;");

    // Connects MyGL's signal that contains the root node of
    // your scene graph to a slot in MainWindow that adds the
    // root node to the GUI's Tree Widget.
            // Widget that emits the signal
    connect(ui->mygl,
            // Signal name
            SIGNAL(sig_sendRootNode(QTreeWidgetItem*)),
            // Widget with the slot that receives the signal
            this,
            // Slot name
            SLOT(slot_addRootToTreeWidget(QTreeWidgetItem*)));

    // Connects the Tree Widget's signal containing the Node that you
    // clicked on to MyGL's slot that updates MyGL's mp_selectedNode
    // member variable to the clicked Node.
    connect(ui->treeWidget, SIGNAL(itemClicked(QTreeWidgetItem*,int)),
            ui->mygl, SLOT(slot_setSelectedNode(QTreeWidgetItem*)));

    connect(ui->mygl, SIGNAL(sig_onlyTranslateSpinbox()),
            this, SLOT(slot_onlyTranslateSpinbox()));

    connect(ui->mygl, SIGNAL(sig_onlyRotateSpinbox()),
            this, SLOT(slot_onlyRotateSpinbox()));

    connect(ui->mygl, SIGNAL(sig_onyScaleSpinbox()),
            this, SLOT(slot_onlyScaleSpinbox()));

    connect(ui->mygl, SIGNAL(sig_enableAllSpinbox()),
            this, SLOT(slot_enableAllSpinbox()));

    // Connects the X-translate spin box's signal containing its new value
    // to MyGL, which has a slot that will update the selected node's
    // X-translate value (you have to go to mygl.cpp and implement
    // the slot, we have just provided a dummy definition)
    connect(ui->txSpinBox, SIGNAL(valueChanged(double)),
            ui->mygl, SLOT(slot_setTX(double)));

    connect(ui->tySpinBox, SIGNAL(valueChanged(double)),
            ui->mygl, SLOT(slot_setTY(double)));

    // TODO: Mirroring the above syntax, connect spin box signals
    // to slots in MyGL that update the other transformation attributes
    connect(ui->rSpinBox, SIGNAL(valueChanged(double)),
            ui->mygl, SLOT(slot_setR(double)));

    connect(ui->sxSpinBox, SIGNAL(valueChanged(double)),
            ui->mygl, SLOT(slot_setSX(double)));

    connect(ui->sySpinBox, SIGNAL(valueChanged(double)),
            ui->mygl, SLOT(slot_setSY(double)));

    // Connects the "Add Translate Node" button's "clicked" signal
    // to a slot in MyGL that will add a child to the currently selected
    // Node.
    connect(ui->tNodeAddButton, SIGNAL(clicked()),
            ui->mygl, SLOT(slot_addTranslateNode()));

    connect(ui->rNodeAddButton, SIGNAL(clicked()),
            ui->mygl, SLOT(slot_addRotateNode()));

    connect(ui->sNodeAddButton, SIGNAL(clicked()),
            ui->mygl, SLOT(slot_addScaleNode()));

    connect(ui->geomSetButton, SIGNAL(clicked()),
            ui->mygl, SLOT(slot_setGeometry()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionQuit_triggered()
{
    QApplication::exit();
}

void MainWindow::slot_addRootToTreeWidget(QTreeWidgetItem *i) {
    ui->treeWidget->addTopLevelItem(i);
}

void MainWindow::slot_onlyTranslateSpinbox() {
    this->ui->txSpinBox->setEnabled(true);
    this->ui->tySpinBox->setEnabled(true);
    this->ui->rSpinBox->setEnabled(false);
    this->ui->sxSpinBox->setEnabled(false);
    this->ui->sySpinBox->setEnabled(false);
}

void MainWindow::slot_onlyRotateSpinbox() {
    this->ui->txSpinBox->setEnabled(false);
    this->ui->tySpinBox->setEnabled(false);
    this->ui->rSpinBox->setEnabled(true);
    this->ui->sxSpinBox->setEnabled(false);
    this->ui->sySpinBox->setEnabled(false);
}

void MainWindow::slot_onlyScaleSpinbox() {
    this->ui->txSpinBox->setEnabled(false);
    this->ui->tySpinBox->setEnabled(false);
    this->ui->rSpinBox->setEnabled(false);
    this->ui->sxSpinBox->setEnabled(true);
    this->ui->sySpinBox->setEnabled(true);
}

void MainWindow::slot_enableAllSpinbox() {
    this->ui->txSpinBox->setEnabled(true);
    this->ui->tySpinBox->setEnabled(true);
    this->ui->rSpinBox->setEnabled(true);
    this->ui->sxSpinBox->setEnabled(true);
    this->ui->sySpinBox->setEnabled(true);
}
