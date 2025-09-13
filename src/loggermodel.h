#pragma once
#include <QAbstractListModel>
#include <QDateTime>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QStandardPaths>
#include <QFile>
#include <QDir>

struct LoggerData {
    QString type;
    double id;
    QString location;
    double temperature;
    double humidity;
    QDateTime timestamp;
};
extern QNetworkAccessManager *globalNam;
class LoggerModel : public QAbstractListModel {
    Q_OBJECT
    Q_PROPERTY(bool loading READ loading NOTIFY loadingChanged)
    Q_PROPERTY(QString connectionState READ connectionState NOTIFY connectionStateChanged)
public:
    enum Roles { TypeRole = Qt::UserRole+1, IDRole, LocationRole, TemperatureRole, HumidityRole, TimestampRole };

    explicit LoggerModel(QObject* parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;
    void setDataList(const QList<LoggerData>& list);
    bool loading() const { return m_loading; }
    QString connectionState() const { return m_connectionState; }
    QList<LoggerData> dataList() const { return m_list; }
    Q_INVOKABLE QVariantMap get(int index) const;

public slots:
    void setLoading(bool l);
    void setConnectionState(const QString &s);
    void refresh();

signals:
    void loadingChanged();
    void connectionStateChanged();
    void dataChanged();


private:
    QList<LoggerData> m_list;
    bool m_loading = false;
    QString m_connectionState = "Disconnected"; // Default
};
