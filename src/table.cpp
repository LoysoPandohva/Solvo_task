#include "include/table.h"

#include <memory>
#include <QAxObject>
#include <QtGui>

Table::Table() {}

void Table::initTable(QTableWidget* table) {
    _table = table;
    _table->setStyleSheet("QTableWidget{background-color: rgb(255,255,255);}"
                          "QHeaderView::section{background-color: rgb(211,223,237);}"
                          "QTableWidget::item::selected{background-color: rgb(211,223,237);"
                          "color: black;}");
}

void Table::newFile(int row, int col){
    _table->setRowCount(row);
    _table->setColumnCount(col);
    for ( int row = 0; row < _table->rowCount(); row++ ){
        for ( int column = 0; column < _table->columnCount(); column++ ){
            _table->setItem(row, column, new QTableWidgetItem());
        }
    }
}
void Table::openFile(const QString &path){
    _path = path;

    std::unique_ptr<QAxObject> excel(new QAxObject("Excel.Application", 0));
    std::unique_ptr<QAxObject> workbooks(excel->querySubObject("Workbooks"));
    std::unique_ptr<QAxObject> workbook(workbooks->querySubObject("Open(const QString&)", QDir::toNativeSeparators(_path)));
    std::unique_ptr<QAxObject> sheets(workbook->querySubObject("Worksheets"));
    std::unique_ptr<QAxObject> sheet(sheets->querySubObject("Item(int)", 1));

    sheets->property("Count").toInt();

    std::unique_ptr<QAxObject> usedRange(sheet->querySubObject("UsedRange"));

    std::unique_ptr<QAxObject> rows(usedRange->querySubObject("Rows"));
    int countRows = rows->property("Count").toInt();

    std::unique_ptr<QAxObject> columns(usedRange->querySubObject("Columns"));
    int countCols = columns->property("Count").toInt();

    _table->setRowCount(countRows);
    _table->setColumnCount(countCols);
    for ( int row = 0; row < countRows; row++ ){
        for ( int column = 0; column < countCols; column++ ){
            std::unique_ptr<QAxObject> cell(sheet->querySubObject("Cells(int,int)", row + 1, column + 1));
            QVariant value = cell->property("Value");
            _table->setItem(row, column, new QTableWidgetItem(value.toString()));
        }
    }

    workbook->dynamicCall("Close(Boolean)", false);
    excel->dynamicCall("Quit()");
}
void Table::saveFile(){
    std::unique_ptr<QAxObject> excel(new QAxObject("Excel.Application", 0));
    std::unique_ptr<QAxObject> workbooks(excel->querySubObject("Workbooks"));
    std::unique_ptr<QAxObject> workbook(workbooks->querySubObject("Open(const QString&)", _path));
    std::unique_ptr<QAxObject> sheets(workbook->querySubObject("Worksheets"));
    std::unique_ptr<QAxObject> sheet(sheets->querySubObject("Item(int)", 1));
    std::unique_ptr<QAxObject> usedRange(sheet->querySubObject("UsedRange"));

    usedRange->dynamicCall("select()");
    usedRange->dynamicCall("delete()");

    for ( int row = 0; row < _table->rowCount(); row++ ){
        for ( int column = 0; column < _table->columnCount(); column++ ){
            std::unique_ptr<QAxObject> cell(sheet->querySubObject("Cells(int,int)", row + 1, column + 1));
            cell->dynamicCall("SetValue(const QVariant&)", _table->item(row, column)->text());
        }
    }

    workbook->dynamicCall("Save()");
    workbook->dynamicCall("Close(Boolean)", false);
    excel->dynamicCall("Quit()");
}
void Table::saveAs(const QString& path){
    _path = path;

    std::unique_ptr<QAxObject> excel(new QAxObject("Excel.Application", 0));
    std::unique_ptr<QAxObject> workbooks(excel->querySubObject("Workbooks"));
    std::unique_ptr<QAxObject> workbook(workbooks->querySubObject("Add()"));
    std::unique_ptr<QAxObject> sheets(workbook->querySubObject("Worksheets"));
    std::unique_ptr<QAxObject> sheet(sheets->querySubObject("Item(int)", 1));

    for ( int row = 0; row < _table->rowCount(); row++ ){
        for ( int column = 0; column < _table->columnCount(); column++ ){
            std::unique_ptr<QAxObject> cell(sheet->querySubObject("Cells(int,int)", row + 1, column + 1));
            cell->dynamicCall("SetValue(const QVariant&)", _table->item(row, column)->text());
        }
    }

    workbook->dynamicCall("SaveAs(const QString&)", QDir::toNativeSeparators(_path));
    workbook->dynamicCall("Close(Boolean)", false);
    excel->dynamicCall("Quit()");
}

void Table::addRow(int row){
    _table->insertRow(row);
    for ( int column = 0; column < _table->columnCount(); column++ ){
        _table->setItem(row, column, new QTableWidgetItem());
    }
}
void Table::addColumn(int col){
    _table->insertColumn(col);
    for ( int row = 0; row < _table->rowCount(); row++ ){
        _table->setItem(row, col, new QTableWidgetItem());
    }
}
void Table::delRow(int row){
    _table->removeRow(row);
}
void Table::delColumn(int col){
    _table->removeColumn(col);
}

bool Table::rowIsEmpty(int row){
    for ( int col = 0; col < _table->columnCount(); col++ ){
        if(!_table->item(row, col)->text().isEmpty()) return false;
    }
    return true;
}
void Table::delEmptyRows(){
    for ( int row = 0; row < _table->rowCount(); row++ ){
        if(rowIsEmpty(row)) {
            delRow(row);
            row--;
        }
    }
}

void Table::sortByCol(){
    int numSortColumn = _table->currentColumn();
    if(numSortColumn < 0) return;

    QVector<QString> sortColumn;
    QVector<std::pair<QVector<QString>, bool>> allRows;

    delEmptyRows();

    allRows.reserve(_table->rowCount());
    for ( int row = 0; row < _table->rowCount(); row++ ){
        sortColumn.push_back(_table->item(row, numSortColumn)->text());

        allRows.push_back(std::pair<QVector<QString>, bool>(QVector<QString>(), false));
        for ( int col = 0; col < _table->columnCount(); col++ ){
            allRows[row].first.push_back(_table->item(row, col)->text());
        }
    }

    std::sort(sortColumn.begin(), sortColumn.end());

    for ( int row = 0; row < _table->rowCount(); row++ ){
        for ( auto it = allRows.begin(); it < allRows.end(); it++ ){
            if(sortColumn[row] == it->first[numSortColumn] && it->second == false){
                it->second = true;
                for( int col = 0; col < _table->columnCount(); col++ ){
                    QTableWidgetItem* item = _table->item(row, col);
                    if(item)
                        item->setText(it->first[col]);
                }
                break;
            }
        }
    }
}

void Table::clearTable(){
    _table->clear();
}

const QString& Table::getPath(){
    return _path;
}
QTableWidget* Table::getTableWidget(){
    return _table;
}
