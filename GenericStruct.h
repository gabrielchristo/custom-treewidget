#ifndef GENERICSTRUCT_H
#define GENERICSTRUCT_H
#pragma once

#include <QSharedDataPointer>
#include "CustomStruct.h"

template <class T>
class GenericStruct : public QSharedData {

public:

    GenericStruct()
    {
        this->object = new T;
        this->size = sizeof(T);
    }

    T* getObject()
    {
        return this->object;
    }

    void setObject(T* obj)
    {
        this->object = obj;
    }

private:

    T* object = nullptr;
    std::size_t size = 0;

};

Q_DECLARE_METATYPE(GenericStruct<CustomStruct>)


#endif
