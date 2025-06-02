#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->btnConnect, SIGNAL(clicked(bool)), this, SLOT(dbconnect()));
    connect(ui->btnSelectAll, SIGNAL(clicked()), this, SLOT(selectAllProducts()));
    connect(ui->btnAdd, SIGNAL(clicked()), this, SLOT(addProduct()));
    connect(ui->btnEdit, SIGNAL(clicked()), this, SLOT(editProduct()));
    connect(ui->btnDel, SIGNAL(clicked()), this, SLOT(deleteProduct()));
    connect(ui->btnAdd2, SIGNAL(clicked()), this, SLOT(addItem()));
    connect(ui->btnEdit2, SIGNAL(clicked()), this, SLOT(editItem()));
    connect(ui->btnDel2, SIGNAL(clicked()), this, SLOT(deleteItem()));
    connect(ui->btnSelectByProduct, SIGNAL(clicked()), this, SLOT(selectItemsByProduct()));
    connect(ui->twOrg, &QTableWidget::cellClicked, this, &MainWindow::onOrgRowClicked);
    connect(ui->twItems, &QTableWidget::cellClicked, this, &MainWindow::onItemRowClicked);
}

MainWindow::~MainWindow()
{
    if(dbconn.isOpen())
        dbconn.close();
    delete ui;
}

void MainWindow::dbconnect()
{
    if (!dbconn.isOpen()) {
        ui->teResult->append("SQL drivers:");
        ui->teResult->append(QSqlDatabase::drivers().join(","));

        dbconn = QSqlDatabase::addDatabase("QPSQL");
        dbconn.setDatabaseName("lab1");
        dbconn.setHostName("localhost");
        dbconn.setUserName("postgres");
        dbconn.setPassword("1201");
        dbconn.setPort(5433);

        if (dbconn.open())
            ui->teResult->append("Connection opened successfully.");
        else
        {
            ui->teResult->append("Connection failed:");
            ui->teResult->append(dbconn.lastError().text());
        }
    } else {
        ui->teResult->append("Connection is already open.");
    }
}

void MainWindow::selectAllProducts()
{
    QSqlQuery query("SELECT * FROM products order by product_id");

    // Установим количество колонок и заголовки
    QStringList headers = {"ID", "Name", "Price", "In Stock", "Reorder", "Description"};
    ui->twOrg->setColumnCount(headers.size());
    ui->twOrg->setHorizontalHeaderLabels(headers);
    ui->twOrg->setRowCount(0); // очистка

    int row = 0;
    while (query.next()) {
        ui->twOrg->insertRow(row);
        for (int col = 0; col < headers.size(); ++col) {
            QString value = query.value(col).toString();
            ui->twOrg->setItem(row, col, new QTableWidgetItem(value));
        }
        row++;
    }

    ui->teResult->append("Selected all products.");
}


void MainWindow::addProduct()
{
    QString name = ui->leProductName->text();
    QString price = ui->lePrice->text();
    QString inStock = ui->leInStock->text();
    QString reorder = ui->leReorder->text();
    QString description = ui->teDescription->toPlainText();

    QSqlQuery query;
    query.prepare("INSERT INTO products (product_name, product_price, in_stock, re_order, description) "
                  "VALUES (:name, :price, :in_stock, :reorder, :description)");
    query.bindValue(":name", name);
    query.bindValue(":price", price);
    query.bindValue(":in_stock", inStock);
    query.bindValue(":reorder", (reorder.toLower() == "true" || reorder == "1"));
    query.bindValue(":description", description);

    if (query.exec()) {
        ui->teResult->append("Product added successfully.");
        selectAllProducts();
    } else {
        ui->teResult->append("Failed to add product: " + query.lastError().text());
    }
}

void MainWindow::editProduct()
{
    int row = ui->twOrg->currentRow();
    if (row < 0) {
        ui->teResult->append("No product selected to edit.");
        return;
    }

    QString id = ui->twOrg->item(row, 0)->text();
    QString name = ui->leProductName->text();
    QString price = ui->lePrice->text();
    QString inStock = ui->leInStock->text();
    QString reorder = ui->leReorder->text();
    QString description = ui->teDescription->toPlainText();

    QSqlQuery query;
    query.prepare("UPDATE products SET product_name = :name, product_price = :price, "
                  "in_stock = :in_stock, re_order = :reorder, description = :description "
                  "WHERE product_id = :id");
    query.bindValue(":name", name);
    query.bindValue(":price", price);
    query.bindValue(":in_stock", inStock);
    query.bindValue(":reorder", (reorder.toLower() == "true" || reorder == "1"));
    query.bindValue(":description", description);
    query.bindValue(":id", id);

    if (query.exec()) {
        ui->teResult->append("Product updated successfully.");
        selectAllProducts();
    } else {
        ui->teResult->append("Failed to update product: " + query.lastError().text());
    }
}


void MainWindow::deleteProduct()
{
    int row = ui->twOrg->currentRow();
    if (row < 0) {
        ui->teResult->append("No product selected to delete.");
        return;
    }

    QString id = ui->twOrg->item(row, 0)->text();  // <-- колонка 0 = product_id


    QSqlQuery query;
    query.prepare("DELETE FROM products WHERE product_id = :id");
    query.bindValue(":id", id);

    if (query.exec()) {
        ui->teResult->append("Product deleted successfully.");
        selectAllProducts();
    } else {
        ui->teResult->append("Failed to delete product: " + query.lastError().text());
    }
}

void MainWindow::addItem()
{
    QString productID = ui->leProductID->text();
    QString orderID = ui->leOrderID->text();
    QString quantity = ui->leQuantity->text();

    // Получаем следующий item_id
    QSqlQuery getItemID;
    getItemID.prepare("SELECT COALESCE(MAX(item_id), 0) + 1 FROM items");

    if (!getItemID.exec()) {
        ui->teResult->append("Error exec getItemID: " + getItemID.lastError().text());
        return;
    }

    if (!getItemID.next()) {
        ui->teResult->append("Error: no result from getItemID.");
        return;
    }

    QString itemID = getItemID.value(0).toString();


    // Получить цену товара из products
    QSqlQuery priceQuery;
    priceQuery.prepare("SELECT product_price::numeric FROM products WHERE product_id = :pid");
    priceQuery.bindValue(":pid", productID);
    if (!priceQuery.exec() || !priceQuery.next()) {
        ui->teResult->append("Error: could not retrieve product price.");
        return;
    }

    double price = priceQuery.value(0).toDouble();
    double total = price * quantity.toInt();

    QSqlQuery query;
    query.prepare("INSERT INTO items (item_id, order_id, product_id, quantity, total) "
                  "VALUES (:item_id, :order_id, :product_id, :quantity, :total)");
    query.bindValue(":item_id", itemID);
    query.bindValue(":order_id", orderID);
    query.bindValue(":product_id", productID);
    query.bindValue(":quantity", quantity);
    query.bindValue(":total", total);

    if (query.exec()) {
        ui->teResult->append("Item added to order.");
        selectItemsByProduct();
    } else {
        ui->teResult->append("Failed to add item: " + query.lastError().text());
    }
}



void MainWindow::editItem()
{
    int row = ui->twItems->currentRow();
    if (row < 0) {
        ui->teResult->append("No item selected to edit.");
        return;
    }

    QString itemID = ui->twItems->item(row, 0)->text();           // item_id
    QString orderID = ui->leOrderID->text();                      // новое order_id
    QString productID = ui->leProductID->text();                  // новое product_id
    QString quantity = ui->leQuantity->text();                    // новое quantity

    // Получаем текущую цену товара
    QSqlQuery priceQuery;
    priceQuery.prepare("SELECT product_price::numeric FROM products WHERE product_id = :pid");
    priceQuery.bindValue(":pid", productID);
    if (!priceQuery.exec() || !priceQuery.next()) {
        ui->teResult->append("Error: could not retrieve product price.");
        return;
    }

    double price = priceQuery.value(0).toDouble();
    double total = price * quantity.toInt();

    // Обновляем запись
    QSqlQuery query;
    query.prepare("UPDATE items SET order_id = :order_id, product_id = :product_id, "
                  "quantity = :quantity, total = :total WHERE item_id = :item_id");
    query.bindValue(":order_id", orderID);
    query.bindValue(":product_id", productID);
    query.bindValue(":quantity", quantity);
    query.bindValue(":total", total);
    query.bindValue(":item_id", itemID);

    if (query.exec()) {
        ui->teResult->append("Item updated successfully.");
        selectItemsByProduct(); // обновим таблицу
    } else {
        ui->teResult->append("Failed to update item: " + query.lastError().text());
    }
}


void MainWindow::deleteItem()
{
    int row = ui->twItems->currentRow();
    if (row < 0) {
        ui->teResult->append("No item selected to delete.");
        return;
    }

    QString itemID = ui->twItems->item(row, 0)->text();

    QSqlQuery query;
    query.prepare("DELETE FROM items WHERE item_id = :item_id");
    query.bindValue(":item_id", itemID);

    if (query.exec()) {
        ui->teResult->append("Item deleted.");
        selectItemsByProduct();
    } else {
        ui->teResult->append("Failed to delete item: " + query.lastError().text());
    }
}


void MainWindow::selectItemsByProduct()
{
    QString productID = ui->leProductID->text();

    QSqlQuery query;
    query.prepare("SELECT item_id, order_id, product_id, quantity, total FROM items WHERE product_id = :pid");
    query.bindValue(":pid", productID);

    if (!query.exec()) {
        ui->teResult->append("Failed to retrieve items: " + query.lastError().text());
        return;
    }

    ui->twItems->setRowCount(0);
    ui->twItems->setColumnCount(5);
    ui->twItems->setHorizontalHeaderLabels({"Item ID", "Order ID", "Product ID", "Quantity", "Total"});

    int row = 0;
    while (query.next()) {
        ui->twItems->insertRow(row);
        for (int col = 0; col < 5; ++col) {
            ui->twItems->setItem(row, col, new QTableWidgetItem(query.value(col).toString()));
        }
        row++;
    }

    ui->teResult->append("Items selected for product " + productID + ".");
}

void MainWindow::onOrgRowClicked(int row, int column)
{
    Q_UNUSED(column);

    ui->leProductName->setText(ui->twOrg->item(row, 1)->text());
    ui->lePrice->setText(ui->twOrg->item(row, 2)->text());
    ui->leInStock->setText(ui->twOrg->item(row, 3)->text());
    ui->leReorder->setText(ui->twOrg->item(row, 4)->text());
    ui->teDescription->setText(ui->twOrg->item(row, 5)->text());
}


void MainWindow::onItemRowClicked(int row, int column)
{
    Q_UNUSED(column);

    ui->leOrderID->setText(ui->twItems->item(row, 1)->text());
    ui->leProductID->setText(ui->twItems->item(row, 2)->text());
    ui->leQuantity->setText(ui->twItems->item(row, 3)->text());
}

