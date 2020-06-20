#ifndef CUSTOMSTRUCT_H
#define CUSTOMSTRUCT_H
#pragma once

#include <QString>
#include <QMetaType>

class CustomStruct {

public:

    enum ItemType {
        TYPE_1 = 0,
        TYPE_2,
        TYPE_3
    };

    CustomStruct()
    {
        this->mItemType = ItemType::TYPE_1;
        this->mText = QString("default text");
    }

    ItemType getItemType()
    {
        return this->mItemType;
    }

    void setItemType(ItemType type)
    {
        this->mItemType = type;
    }

    QString getText()
    {
        return this->mText;
    }

    void setText(QString text)
    {
        this->mText = text;
    }

private:

    ItemType mItemType;
    QString mText;


};
Q_DECLARE_METATYPE(CustomStruct)

#endif
