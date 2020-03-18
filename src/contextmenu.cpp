#include "include/contextmenu.h"

#include <QMenu>
#include <QAction>

ContextMenu::ContextMenu() {}
ContextMenu::~ContextMenu(){
    delete _contextMenu;
    for(int i = 0; i < 5; i++)
       delete _contextAction[i];
}

void ContextMenu::createContextMenu(Table* table, MainWindow* mw){
    table->getTableWidget()->setSelectionMode(QAbstractItemView::SingleSelection);
    table->getTableWidget()->setContextMenuPolicy(Qt::CustomContextMenu);

    _contextAction[0] = new QAction(QIcon(":/img/img/add.png"), "Add Row",mw);
    _contextAction[1] = new QAction(QIcon(":/img/img/add.png"), "Add Column",mw);
    _contextAction[2] = new QAction(QIcon(":/img/img/delete.png"), "Delete Row",mw);
    _contextAction[3] = new QAction(QIcon(":/img/img/delete.png"), "Delete Column",mw);
    _contextAction[4] = new QAction(QIcon(":/img/img/sort.png"), "Sort by Column",mw);

    _contextMenu = new QMenu();
    for (size_t i = 0; i < 5; i++) {
        _contextMenu->addAction(_contextAction[i]);
    }

    QObject::connect(table->getTableWidget(), SIGNAL(customContextMenuRequested(const QPoint &)), mw, SLOT(showContextMenu(const QPoint &)));
    QObject::connect(_contextAction[0], SIGNAL(triggered()), mw, SLOT(on_pB_addRow_clicked()));
    QObject::connect(_contextAction[1], SIGNAL(triggered()), mw, SLOT(on_pB_addColumn_clicked()));
    QObject::connect(_contextAction[2], SIGNAL(triggered()), mw, SLOT(on_pB_delRow_clicked()));
    QObject::connect(_contextAction[3], SIGNAL(triggered()), mw, SLOT(on_pB_delColumn_clicked()));
    QObject::connect(_contextAction[4], SIGNAL(triggered()), mw, SLOT(on_pB_sortByColumn_clicked()));
}

QMenu* ContextMenu::getContextMenu(){
    return _contextMenu;
}
