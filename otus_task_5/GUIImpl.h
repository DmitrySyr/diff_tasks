#pragma once

#include "GUIForms.h"

class IForm;

class Canvas;
class MainWindow;
class MainMenu;
class FiguresMenu;

class GUIImpl {

public:


    GUIImpl();

    GUIImpl(const GUIImpl& ) = delete;
    GUIImpl& operator=(const GUIImpl&) = delete;


    void Refresh( IForm* );

    void Show() {}

    void MainLoop(){}


    Canvas* canva;
    MainWindow* mainWindow;
    MainMenu* mainMenu;
    FiguresMenu* figuresMenu;
};
