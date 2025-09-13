#pragma once
#include <QSortFilterProxyModel>

class ProxyFilterModel : public QSortFilterProxyModel {
    Q_OBJECT
public:
    explicit ProxyFilterModel(QObject* parent = nullptr) : QSortFilterProxyModel(parent) {}

    Q_INVOKABLE void setTypeFilter(const QString &type) { m_type = type; invalidateFilter(); }
    Q_INVOKABLE void setLocationFilter(const QString &loc) { m_location = loc; invalidateFilter(); }
    using QSortFilterProxyModel::QSortFilterProxyModel;

    Q_INVOKABLE void invalidateFilterQml() {
        invalidateFilter();
    }

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override {
        Q_UNUSED(source_parent);
        QModelIndex idx = sourceModel()->index(source_row, 0);
        QString type = sourceModel()->data(idx, Qt::UserRole+1).toString();
        QString location = sourceModel()->data(idx, Qt::UserRole+3).toString();
        if (!m_type.isEmpty() && m_type != type) return false;
        if (!m_location.isEmpty() && m_location != location) return false;
        return true;
    }

private:
    QString m_type;
    QString m_location;
};
