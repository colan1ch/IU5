#include "utils.h"
#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include <QMessageBox>

void exportToCSV(QAbstractItemModel *model, const QString &defaultFileName)
{
    if (!model) {
        QMessageBox::warning(nullptr, "Ошибка", "Нет данных для экспорта!");
        return;
    }

    QString filePath = QFileDialog::getSaveFileName(nullptr, "Сохранить как", defaultFileName, "CSV файлы (*.csv)");
    if (filePath.isEmpty()) return;

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(nullptr, "Ошибка", "Не удалось открыть файл для записи!");
        return;
    }

    QTextStream stream(&file);

    // Записываем заголовки столбцов
    for (int col = 0; col < model->columnCount(); ++col) {
        stream << model->headerData(col, Qt::Horizontal).toString();
        if (col < model->columnCount() - 1) stream << ",";
    }
    stream << "\n";

    // Записываем данные строк
    for (int row = 0; row < model->rowCount(); ++row) {
        for (int col = 0; col < model->columnCount(); ++col) {
            stream << model->data(model->index(row, col)).toString();
            if (col < model->columnCount() - 1) stream << ",";
        }
        stream << "\n";
    }

    file.close();
    QMessageBox::information(nullptr, "Успех", "Данные успешно экспортированы в CSV!");
}
