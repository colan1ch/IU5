#ifndef UTILS_H
#define UTILS_H

#include <QAbstractItemModel>
#include <QString>

void exportToCSV(QAbstractItemModel *model, const QString &defaultFileName);

#endif // UTILS_H
