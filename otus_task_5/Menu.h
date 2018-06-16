#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#include "Document.h"
#include "Objects.h"

class MainMenuNew final : public IObject {

    Document* doc = nullptr;
    size_t ObjectID;

public:
    MainMenuNew( Document* document, int PositionX, int PositionY, int Height, int Width );

    void GetDataForDrawing() const override {}
    virtual void ReactOnClick( size_t, size_t ) override;

};

class MainMenuLoad : public IObject {

};

class MainMenuSave : public IObject {

};

class MainMenuQuit : public IObject {

};



#endif // MENU_H_INCLUDED
