#include "Menu.h"
#include <iostream>

MainMenuNew::MainMenuNew( Document* document, int PositionX, int PositionY, int Height, int Width )
{
    doc = document;
    document->AddNewObject(  );
    ObjectID = document->GetObjectID();
    document->area.areas[{PositionX, PositionY, Height, Width}] = ObjectID;
}

void MainMenuNew::ReactOnClick( size_t, size_t )
{
    this->doc->NewDocument();
}
