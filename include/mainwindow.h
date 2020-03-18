#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "table.h"
#include "contextmenu.h"
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class ContextMenu;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:  
    void on_actionNew_triggered();
    void on_actionOpen_triggered();
    void on_actionSave_triggered();
    void on_actionSaveAs_triggered();
    void on_actionQuit_triggered();

    void showContextMenu(const QPoint &pos);

    void on_pB_addRow_clicked();
    void on_pB_addColumn_clicked();
    void on_pB_delRow_clicked();
    void on_pB_delColumn_clicked();
    void on_pB_sortByColumn_clicked();

private:
    Ui::MainWindow* _ui;
    Table _table;
    ContextMenu* _contextMenu;
};

#endif // MAINWINDOW_H
