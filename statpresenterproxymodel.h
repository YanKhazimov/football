#ifndef STATPRESENTERPROXYMODEL_H
#define STATPRESENTERPROXYMODEL_H

#include <QIdentityProxyModel>
#include "customextendablemodel.h"

class StatPresenterProxyModel: public QIdentityProxyModel, public CustomExtendableModel
{
    Q_OBJECT

public:
    StatPresenterProxyModel(QObject *parent = nullptr);

    Q_INVOKABLE virtual QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;

    Q_INVOKABLE bool sortBy(int statRole) Q_DECL_OVERRIDE;
    Q_INVOKABLE bool selectRow(int row) Q_DECL_OVERRIDE;

private:
    void sort();
};

#endif // STATPRESENTERPROXYMODEL_H
