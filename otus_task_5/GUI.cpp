#include "GUI.h"

void GUI::AddNewDocument( Document* document )
{
    documents.push_back( document );
}

size_t GUI::GetDocumentID()
{
    return ( documents.size() - 1 );
}

void GUI::Redraw( size_t ID )
{
    this->API_draw_all( this->documents[ID] );
}

void GUI::API_draw_all( Document* doc )
{
    for( auto object : doc->GetAllObjectForDrawing() )
    {
        object->GetDataForDrawing();
    }
}
