#ifndef FIGURES_H_INCLUDED
#define FIGURES_H_INCLUDED

#include "Document.h"
#include "Objects.h"



class Canvas final : public IObject {
    Document* doc = nullptr;
    size_t ObjectID;

public:
    // обработчик, если действие пришлось на объект Canvas
    IObject* Processor = nullptr;

    Canvas( Document* document, int PositionX, int PositionY, int Height, int Width )
    {
        doc = document;
        document->AddNewObject( );
        ObjectID = document->GetObjectID();
    }

    void GetDataForDrawing() const override {}
    void ReactOnClick( size_t x, size_t y ) override
    {
        RealReactionOnClick( x, y );
    }

    void virtual ReactOnDragAndDrop() override
    {
        RealReactionOnDragAndDrop();
    }

private:
    void RealReactionOnClick( size_t x, size_t y )
    {
        if( Processor )
        {
            Processor->ReactOnClick( x, y );
        }
    }

    void RealReactionOnDragAndDrop()
    {
        if( Processor )
        {
            Processor->ReactOnDragAndDrop();
        }

    }
};

class FiguresMenu_Circle final : public IObject {
    Document* doc = nullptr;
    size_t ObjectID;

public:
    FiguresMenu_Circle( Document* document, int PositionX, int PositionY, int Height, int Width )
    {
        doc = document;
        document->AddNewObject( );
        ObjectID = document->GetObjectID();
    }

    void GetDataForDrawing() const override {}
    void ReactOnClick( size_t x, size_t y ) override
    {
        RealReactionOnClick( x, y );
    }

    void virtual ReactOnDragAndDrop() override
    {
        RealReactionOnDragAndDrop();
    }

private:
    void RealReactionOnClick( size_t x , size_t y )
    {
        if( Processor != this )
        {
            doc->GetCanvas()->Processor = this;
        }
    }

    void RealReactionOnDragAndDrop()
    {
        doc->AddNewObject();
    }
};

class FiguresMenu_DeleteFigure final : public IObject {
    Document* doc = nullptr;
    size_t ObjectID;

public:
    FiguresMenu_DeleteFigure( Document* document, int PositionX, int PositionY, int Height, int Width )
    {
        doc = document;
        document->AddNewObject( );
        ObjectID = document->GetObjectID();
    }

    void GetDataForDrawing() const override {}
    void ReactOnClick( size_t x, size_t y ) override
    {
        RealReactionOnClick( x, y );
    }

private:
    void RealReactionOnClick( size_t x, size_t y )
    {
        if( Processor != this )
        {
            doc->GetCanvas()->Processor = this;
        }
        else
        {
            auto n = doc->FindObject( x, y );
            doc->DeleteObject( n );
            doc->AskGUI_Redraw();
        }
    }
};

#endif // FIGURES_H_INCLUDED
