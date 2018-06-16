#ifndef OBJECTS_H_INCLUDED
#define OBJECTS_H_INCLUDED


#include "Document.h"

class IObject{



public:
    IObject(){}

    virtual ~IObject(){}

    void virtual GetDataForDrawing() const  = 0;
    void virtual ReactOnClick( size_t, size_t ) {}
    void virtual ReactOnDragAndDrop() {}
    void virtual ReactOnDoubleClick() {}

    IObject* Processor = nullptr;
};



#endif // OBJECTS_H_INCLUDED
