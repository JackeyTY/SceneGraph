#pragma once

#include <QMainWindow>
#include <QTreeWidgetItem>


namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionQuit_triggered();
    void slot_addRootToTreeWidget(QTreeWidgetItem*);
    void slot_onlyTranslateSpinbox();
    void slot_onlyRotateSpinbox();
    void slot_onlyScaleSpinbox();
    void slot_enableAllSpinbox();

private:
    Ui::MainWindow *ui;
};
