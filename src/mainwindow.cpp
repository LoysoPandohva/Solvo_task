#include "include/mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), _ui(new Ui::MainWindow), _contextMenu(new ContextMenu) {

    _ui->setupUi(this);
    _table.initTable(_ui->tableWidget);
    _contextMenu->createContextMenu(&_table, this);

    this->setWindowTitle("My Excel app.");
    _ui->statusbar->showMessage("Welcome to the new breath of Excel)");
}
MainWindow::~MainWindow() {
    delete _ui;
    delete _contextMenu;
}

void MainWindow::on_actionNew_triggered() {
    _table.clearTable();
    _table.newFile();
    _ui->statusbar->showMessage("New file was created");
}
void MainWindow::on_actionOpen_triggered() {
    QString path = QFileDialog::getOpenFileName
            (0, "Open Dialog", "", "Таблица XML 2003 (*.xml);;Книга Excel (*.xlsx)");
    if(!path.isEmpty()){
        _table.clearTable();
        _table.openFile(path);
        _ui->statusbar->showMessage("File " + path + " was opened");
    }
}
void MainWindow::on_actionSave_triggered() {
    if(!_table.getPath().isEmpty())
        _table.saveFile();
    else on_actionSaveAs_triggered();
    _ui->statusbar->showMessage("File was saved");
}
void MainWindow::on_actionSaveAs_triggered() {
    QString path = QFileDialog::getSaveFileName(0, "Save file", QDir::currentPath(),
                "Таблица XML 2003 (*.xml);;Книга Excel (*.xlsx)");
    if(!path.isEmpty())
        _table.saveAs(path);
    _ui->statusbar->showMessage("File was saved");
}
void MainWindow::on_actionQuit_triggered() {
    QApplication::exit(0);
}

void MainWindow::showContextMenu(const QPoint &pos) {
    QTableWidgetItem *item = _ui->tableWidget->itemAt(pos);
    for (int i = 0; i < _contextMenu->getContextMenu()->actions().size(); i++){
        _contextMenu->getContextMenu()->actions().at(i)->setData(QVariant(_ui->tableWidget->column(item)));
    }
    _contextMenu->getContextMenu()->exec(QCursor::pos());
}

void MainWindow::on_pB_addRow_clicked() {
    _table.addRow(_ui->tableWidget->currentRow() + 1);
    _ui->statusbar->showMessage("Row " + QString::number(_ui->tableWidget->currentRow() + 2) + " was added");
}
void MainWindow::on_pB_addColumn_clicked() {
    _table.addColumn(_ui->tableWidget->currentColumn() + 1);
    _ui->statusbar->showMessage("Column " + QString::number(_ui->tableWidget->currentColumn() + 2) + " was added");
}
void MainWindow::on_pB_delRow_clicked() {
    _table.delRow(_ui->tableWidget->currentRow());
    _ui->statusbar->showMessage("Row " + QString::number(_ui->tableWidget->currentRow() + 1) + " was deleted");
}
void MainWindow::on_pB_delColumn_clicked() {
    _table.delColumn(_ui->tableWidget->currentColumn());
    _ui->statusbar->showMessage("Column " + QString::number(_ui->tableWidget->currentColumn() + 1) + " was deleted");
}
void MainWindow::on_pB_sortByColumn_clicked() {
    _table.sortByCol();
    _ui->statusbar->showMessage("Table was sorted by " + QString::number(_ui->tableWidget->currentColumn() + 1) + " column");

}
