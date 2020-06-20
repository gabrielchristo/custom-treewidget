#ifndef TREEUTILS_H
#define TREEUTILS_H
#pragma once

#include <QObject>
#include <QDebug>
#include <QTreeWidget>
#include <QMenu>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

#include "GenericStruct.h"

class TreeUtils : public QObject
{
    Q_OBJECT

public:

    /**
     * @brief constructor
     */
    TreeUtils();

    /**
     * @brief inject treewidget which class will work
     * @param tree treewidget pointer
     */
    void setTreeWidget(QTreeWidget* tree);

    /**
     * @brief get treewidgetitem variant data (in this case a generic struct)
     * @param item treewidgetitem
     * @param col treewidgetitem column
     * @return custom type inside generic struct
     */
    template <class T>
    static T* getTreeItemData(QTreeWidgetItem* item, int col)
    {
        return item->data(col, Qt::UserRole).value<GenericStruct<T>>().getObject();
    };


private:

    QTreeWidget* mTreeWidget = nullptr; ///< treewidget class member

    /**
     * @brief creates a treewidgetitem with a custom struct inside its variant data
     * @param parent parent treewidgetitem
     * @return created treewidgetitem
     */
    template <class T>
    QTreeWidgetItem* createTreeItem(QTreeWidgetItem* parent = nullptr)
    {
        auto item = new QTreeWidgetItem(parent);

        // verifying if treeitem is a toplevel/child node
        auto itemText = parent == nullptr ? QString("Top Level Item %1").arg(this->mTreeWidget->topLevelItemCount() + 1) :
                                            parent->text(0).append(" Child %1").arg(parent->childCount());

        item->setText(0, itemText); // column 0
        item->setFlags(item->flags() | Qt::ItemIsEditable | Qt::ItemIsUserCheckable);
        item->setCheckState(0, Qt::Unchecked); // column 0
        item->setData(0, Qt::UserRole, QVariant::fromValue(GenericStruct<T>())); // column 0

        return item;
    };

    /**
     * @brief verify if treewidget has a selected treewidgetitem
     * @return bool
     */
    bool hasSelectedItem();

    /**
     * @brief get children of given tree node
     * @param item treewidgetitem
     * @return children list
     */
    QList<QTreeWidgetItem *> getChildren(QTreeWidgetItem* item);

    /**
     * @brief gets json array based on desired treewidgetitem recursively
     * @param item treewidgetitem
     * @param jsonArray base item json array
     * @return json array
     */
    QJsonArray getItemJson(QTreeWidgetItem* item, QJsonArray& jsonArray);

    /**
     * @brief updates json text on mainwindow
     */
    void updateTreeJson();

public slots:

    /**
     * @brief add node in tree, with item text based on current selected item
     */
    void slotAddItemInTree();

    /**
     * @brief remove the selected treewidgetitem and all its children from tree
     */
    void slotRemoveTreeItem();

    /**
     * @brief clear tree items
     */
    void slotClearTree();

    /**
     * @brief update selected treewidgetitem custom struct text value
     * @param newText text in line edit
     */
    void slotUpdateItemText(QString newText);

    /**
     * @brief update selected treewidgetitem custom struct type value
     * @param newIndex index of type combobox
     */
    void slotUpdateItemType(int newIndex);

    /**
     * @brief expand all treewidgetitems
     */
    void slotExpandAll();

    /**
     * @brief collapse all trewidgetitems
     */
    void slotCollapseAll();

    /**
     * @brief clear treewidget selection
     */
    void slotClearSelection();

    /**
     * @brief show only matched treewidget items at tree
     * @param text text filter
     */
    void slotApplyFilter(QString text);

private slots:

    /**
     * @brief handle context menu over treewidgetitem
     * @param point screen clicked location
     */
    void slotHandleContextMenu(const QPoint& point);

signals:

    /**
     * @brief signal to mainwindow update custom struct form with clicked tree item data
     * @param newText updated lineedit text
     * @param newIndex updated combobox index
     */
    void signalUpdateForm(QString newText, int newIndex);

    /**
     * @brief signal to mainwindow show message in the status bar
     * @param message text
     * @param timeout milliseconds to show message
     */
    void signalShowStatusbarMessage(QString message, int timeout);

    /**
     * @brief signal to mainwindow update json textedit
     * @param text json array
     */
    void signalUpdateJsonText(QString text);

};

#endif
