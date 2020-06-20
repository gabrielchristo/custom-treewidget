#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#pragma once

#include <QMainWindow>
#include "TreeUtils.h"
#include "ui_MainWindow.h"

class MainWindow : public QMainWindow, public Ui_MainWindow
{
    Q_OBJECT

public:

    /**
     * @brief constructor
     * @param parent parent qwidget
     */
    MainWindow(QWidget *parent = nullptr);

    /**
     * @brief destructor
     */
    ~MainWindow();

private:

    /**
     * @brief create objects connections
     */
    void connects();

    TreeUtils mTreeUtils; ///< treeutils class member


};
#endif
