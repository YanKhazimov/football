#ifndef STATPRESENTERPROXYMODEL_H
#define STATPRESENTERPROXYMODEL_H

#include <QIdentityProxyModel>

class StatPresenterProxyModel: public QIdentityProxyModel//, public CustomExtendableModel
{
    Q_OBJECT

public:
    StatPresenterProxyModel(QObject *parent = nullptr);

    Q_INVOKABLE virtual QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;

    virtual void setSourceModel(QAbstractItemModel *source) Q_DECL_OVERRIDE;

    Q_INVOKABLE bool sortBy(int statRole);
    Q_INVOKABLE bool selectRow(int row);

signals:
    void selectedRowChanged(int selectedRow);

private:
    void sort();
};

#endif // STATPRESENTERPROXYMODEL_H
