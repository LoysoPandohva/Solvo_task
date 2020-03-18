#ifndef CONTEXTMENU_H
#define CONTEXTMENU_H

#include "mainwindow.h"

class MainWindow;

class ContextMenu
{
public:
    ContextMenu();
    ~ContextMenu();
    void createContextMenu(Table* table, MainWindow* mw);

    QMenu* getContextMenu();

private:
    QMenu* _contextMenu;
    QAction* _contextAction[5];
};

#endif // CONTEXTMENU_H
