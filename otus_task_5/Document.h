#pragma once

#include <memory>
#include <vector>
#include <map>

#include "Objects.h"
#include "GUI.h"

enum class ActionType{
    Click,
    DragAndDrop,
    DoubleClick
};


class GUI;

class Document{

    using ObjectStorage = std::vector<IObject*>;

    ObjectStorage objects;

    GUI* gui;

public:
    size_t DocumentID;

    struct ObjectsAreas
    {
        IObject* CurrentProcessor = nullptr;

        std::map<std::array<int, 4>, size_t> areas;

    } area;

public:

    Document( GUI* gui );

    Document( const Document& )            = delete;
    Document( Document&& )                 = delete;
    Document& operator=( const Document& ) = delete;

    const ObjectStorage& GetAllObjectForDrawing() const
    {
        return const_cast<const ObjectStorage&>( objects );
    }

    IObject* GetCanvas()
    {
        return this->objects[0];
    }

    size_t GetObjectID();
    void AddNewObject( ){}
    void DeleteObject( size_t ) {}
    size_t FindObject(size_t x, size_t y );

    void ProcessAction( size_t, size_t, size_t, size_t, ActionType);

    void AskGUI_Redraw();

    void NewDocument();
    void SaveDocument();
    void LoadDocument();
    void Quit();

};
