#ifndef TABLE_H
#define TABLE_H

#include <QTableWidget>

class Table
{
public:
    Table();

    void initTable(QTableWidget* table);

    void newFile(int row = 5, int col = 5);
    void openFile(const QString &path);
    void saveFile();
    void saveAs(const QString& path);

    void addRow(int row);
    void addColumn(int col);

    void delRow(int row);
    void delColumn(int col);

    bool rowIsEmpty(int row);
    void delEmptyRows();
    void sortByCol();

    void clearTable();

    const QString& getPath();
    QTableWidget* getTableWidget();

private:
    QString _path;
    QTableWidget* _table;
};

#endif // TABLE_H
