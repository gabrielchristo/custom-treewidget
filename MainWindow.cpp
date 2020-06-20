#include "MainWindow.h"


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    this->setupUi(this);

    this->mTreeWidget->setHeaderHidden(true);
    this->mTreeWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    this->mTreeWidget->setAcceptDrops(true);
    this->mTreeWidget->setDragEnabled(true);
    this->mTreeWidget->setDragDropMode(QAbstractItemView::InternalMove);

    this->mTreeUtils.setTreeWidget(this->mTreeWidget);
    this->connects();
}

MainWindow::~MainWindow()
{

}

void MainWindow::connects()
{
    connect(mButtonAdd, &QPushButton::clicked, &mTreeUtils, &TreeUtils::slotAddItemInTree);
    connect(mButtonClear, &QPushButton::clicked, &mTreeUtils, &TreeUtils::slotClearTree);
    connect(mButtonRemove, &QPushButton::clicked, &mTreeUtils, &TreeUtils::slotRemoveTreeItem);
    connect(mButtonExpand, &QPushButton::clicked, &mTreeUtils, &TreeUtils::slotExpandAll);
    connect(mButtonCollapse, &QPushButton::clicked, &mTreeUtils, &TreeUtils::slotCollapseAll);
    connect(mButtonClearSelection, &QPushButton::clicked, &mTreeUtils, &TreeUtils::slotClearSelection);

    connect(mFilterLineEdit, &QLineEdit::textEdited, &mTreeUtils, &TreeUtils::slotApplyFilter);

    connect(mItemLineEdit, &QLineEdit::textEdited, &mTreeUtils, &TreeUtils::slotUpdateItemText);
    connect(mItemTypeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), &mTreeUtils, &TreeUtils::slotUpdateItemType);

    connect(&mTreeUtils, &TreeUtils::signalUpdateForm, this, [this](QString newText, int newIndex){
        this->mItemTypeCombo->setCurrentIndex(newIndex);
        this->mItemLineEdit->setText(newText);
    }, Qt::UniqueConnection);
    connect(&mTreeUtils, &TreeUtils::signalShowStatusbarMessage, this->statusbar, &QStatusBar::showMessage);
    connect(&mTreeUtils, &TreeUtils::signalUpdateJsonText, this->mJsonOutput, &QPlainTextEdit::setPlainText);
}





