#pragma once

#include <vector>

#include "Document.h"

class Document;

class GUI{

std::vector<Document*> documents;

void API_draw_all( Document* );

public:
    GUI( ){}
    GUI( const GUI& )            = delete;
    GUI( GUI&& )                 = delete;
    GUI& operator=( const GUI& ) = delete;
    ~GUI(){}

    void AddNewDocument( Document* document );
    void DeleteDocument( size_t DocumentID );

    size_t GetDocumentID();

    void Redraw( size_t );
};

