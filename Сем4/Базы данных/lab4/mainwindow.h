#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QTableWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QSqlDatabase dbconn;

public slots:
    void dbconnect();
    void selectAllProducts();
    void addProduct();
    void editProduct();
    void deleteProduct();
    void addItem();
    void editItem();
    void deleteItem();
    void selectItemsByProduct();
    void onOrgRowClicked(int, int);
    void onItemRowClicked(int, int);
};

#endif // MAINWINDOW_H
