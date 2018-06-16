#include "Document.h"

Document::Document( GUI* GuiPassed )
    {
        gui = GuiPassed;
        gui->AddNewDocument( this );
        DocumentID = gui->GetDocumentID();
    }

void Document::AskGUI_Redraw()
    {
        this->gui->Redraw( this->DocumentID );
    }

size_t Document::GetObjectID()
{
    return this->objects.size() - 1;
}

void Document::NewDocument(){}

void Document::ProcessAction( size_t, size_t, size_t, size_t, ActionType){}

size_t Document::FindObject(size_t x, size_t y )
{
    return x * y;
}
