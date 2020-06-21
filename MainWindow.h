#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#pragma once

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
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

private slots:

    /**
     * @brief save json text to local file
     */
    void slotSaveJson();

    /**
     * @brief load json file into the treewidget
     */
    void slotLoadJson();


};
#endif
