#include "TreeUtils.h"

TreeUtils::TreeUtils()
{
}

void TreeUtils::setTreeWidget(QTreeWidget *tree)
{
    this->mTreeWidget = tree;

    // when tree item clicked, emits signal to mainwindow update custom struct form (with clicked item data)
    connect(mTreeWidget, &QTreeWidget::itemClicked, this, [this](QTreeWidgetItem* item, int col){
        auto obj = TreeUtils::getTreeItemData<CustomStruct>(item, col);
        emit signalUpdateForm(obj->getText(), obj->getItemType());
    }, Qt::UniqueConnection);

    // creates and handle context menu over clicked treewidgetitem
    connect(mTreeWidget, &QTreeWidget::customContextMenuRequested, this, &TreeUtils::slotHandleContextMenu, Qt::UniqueConnection);

    // updates json when treewidgetitem text gets changed
    connect(mTreeWidget, &QTreeWidget::itemChanged, this, [this](){
        this->updateTreeJson();
    }, Qt::UniqueConnection);
}

void TreeUtils::parseJsonToTree(QJsonDocument jsonDoc)
{
    this->mTreeWidget->clear();
    this->mTreeWidget->blockSignals(true);
    this->createJsonTreeItems(this->mTreeWidget->invisibleRootItem(), jsonDoc.array());
    this->mTreeWidget->blockSignals(false);
    this->updateTreeJson();
}

void TreeUtils::createJsonTreeItems(QTreeWidgetItem *item, QJsonArray jsonArray)
{
    std::for_each(jsonArray.begin(), jsonArray.end(), [this, item](QJsonValue value){
        auto itemObject = value.toObject();
        auto children = itemObject["children"].toArray();
        auto newItem = this->createTreeItem<CustomStruct>(item);
        newItem->setCheckState(0, itemObject["checked"].toBool() ? Qt::Checked : Qt::Unchecked);
        newItem->setText(0, itemObject["name"].toString());
        auto itemData = this->getTreeItemData<CustomStruct>(newItem, 0);
        itemData->setText(itemObject["text"].toString());
        itemData->setItemType(static_cast<CustomStruct::ItemType>(itemObject["type"].toInt()));
        this->createJsonTreeItems(newItem, children);
        item->addChild(newItem);
    });
}

void TreeUtils::slotAddItemInTree()
{
    if(this->hasSelectedItem()){
        auto crrtItem = this->mTreeWidget->currentItem();
        crrtItem->addChild(this->createTreeItem<CustomStruct>(crrtItem));
    }
    else
        this->mTreeWidget->addTopLevelItem(this->createTreeItem<CustomStruct>());

    this->updateTreeJson();
}

void TreeUtils::slotRemoveTreeItem()
{
    if(this->hasSelectedItem()){
       delete this->mTreeWidget->currentItem();
    }
    else
        emit signalShowStatusbarMessage("No selected item in tree", 3000);

    this->updateTreeJson();
}

void TreeUtils::slotClearTree()
{
    this->mTreeWidget->clear();
    this->updateTreeJson();
}

bool TreeUtils::hasSelectedItem()
{
    return !this->mTreeWidget->selectedItems().isEmpty();
}

QList<QTreeWidgetItem *> TreeUtils::getChildren(QTreeWidgetItem *item)
{
    QList<QTreeWidgetItem*> list;
    for(int i = 0; i < item->childCount(); i++) list.append(item->child(i));
    return list;
}

QJsonArray TreeUtils::getItemJson(QTreeWidgetItem *item, QJsonArray &jsonArray)
{
    for(int i = 0; i < item->childCount(); i++){

        auto data = this->getTreeItemData<CustomStruct>(item->child(i), 0); // column 0
        QJsonObject object;
        object["name"] = item->child(i)->text(0);
        object["checked"] = item->child(i)->checkState(0) ? true : false;
        object["type"] = data->getItemType();
        object["text"] = data->getText();

        QJsonArray childArray;
        this->getItemJson(item->child(i), childArray);
        object["children"] = childArray;
        jsonArray.append(object);
    }
    return jsonArray;
}

void TreeUtils::updateTreeJson()
{
    QJsonDocument doc;
    QJsonArray childrenArray;
    doc.setArray(this->getItemJson(mTreeWidget->invisibleRootItem(), childrenArray));
    emit signalUpdateJsonText(doc.toJson());
}

void TreeUtils::slotClearSelection()
{
    this->mTreeWidget->clearSelection();
}

void TreeUtils::slotApplyFilter(QString text)
{
    auto items = this->mTreeWidget->findItems(text, Qt::MatchContains | Qt::MatchRecursive);
    auto allItems = this->mTreeWidget->findItems("*", Qt::MatchRecursive | Qt::MatchWrap | Qt::MatchWildcard);

    for(auto item : allItems){
        if(items.contains(item)) item->setHidden(false);
        else item->setHidden(true);
    }
}

void TreeUtils::slotUpdateItemText(QString newText)
{
    if(this->hasSelectedItem()){
        auto obj = this->getTreeItemData<CustomStruct>(this->mTreeWidget->currentItem(), 0); // column 0
        obj->setText(newText);
    }
    else
        emit signalShowStatusbarMessage("No selected item in tree", 3000);
    this->updateTreeJson();
}

void TreeUtils::slotUpdateItemType(int newIndex)
{
    if(this->hasSelectedItem()){
        auto obj = this->getTreeItemData<CustomStruct>(this->mTreeWidget->currentItem(), 0); // column 0
        obj->setItemType(static_cast<CustomStruct::ItemType>(newIndex));
    }
    else
        emit signalShowStatusbarMessage("No selected item in tree", 3000);
    this->updateTreeJson();
}

void TreeUtils::slotExpandAll()
{
    this->mTreeWidget->expandAll();
}

void TreeUtils::slotCollapseAll()
{
    this->mTreeWidget->collapseAll();
}

void TreeUtils::slotHandleContextMenu(const QPoint& point)
{
    auto currentItem = this->mTreeWidget->itemAt(point);
    if(!currentItem) return;

    this->mTreeWidget->clearSelection();
    currentItem->setSelected(true);

    QMenu menu;
    menu.addAction("Add Child", this, &TreeUtils::slotAddItemInTree);
    menu.addAction("Remove Children", this, [this, currentItem](){
        for(auto child : this->getChildren(currentItem)) currentItem->removeChild(child);
        this->updateTreeJson();
    });
    menu.addAction("Remove Item", this, &TreeUtils::slotRemoveTreeItem);

    menu.exec(this->mTreeWidget->viewport()->mapToGlobal(point));
}


