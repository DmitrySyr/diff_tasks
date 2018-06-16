#pragma once

#include "GUI.h"
#include "Document.h"
//#include "Objects.h"
//#include "Figures.h"
//#include "Menu.h"

class Application{

    Application(){}
    size_t CurrentDocument;

public:

    static Application& Instance()
    {
        static Application instance;
        return instance;
    }

    void Init() {

        if( !gui )
        {
            gui = std::make_shared<GUI>( );
        }

        if( documents.empty() )
        {
            documents.push_back( std::make_shared<Document>( gui.get() ) );
            CurrentDocument = documents.size() - 1;
        }
    }

    void ShutDown() {}

    void MainLoop()
    {
        // receiving user reaction
    }


    Application(const Application& root)       = delete;
    Application& operator=(const Application&) = delete;

    void ChangeDocument();
    void DeleteDocument();
    void AddDocument();
    Document* GetCurrentDocument()
    {
        return documents[CurrentDocument].get();
    }

    std::shared_ptr<GUI> gui;
    std::vector<std::shared_ptr<Document>> documents;
};
