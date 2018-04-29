#pragma once

#include "GUIImpl.h"
#include "Figures.h"
#include "GUIForms.h"
#include "Drawing.h"

class Application{

public:

    static Application& Instance()
    {
        static Application instance;
        return instance;
    }

    Application* GetParent() {

        return this;
    }

    void Init() {

        if( !gui ) {
            gui = new GUIImpl();
            gui -> Show();
        }
    }

    void ShutDown() {}

    void MainLoop() {}





    Application() = default;
    Application(const Application& root) = delete;
    Application& operator=(const Application&) = delete;


    GUIImpl* gui = nullptr;
};
