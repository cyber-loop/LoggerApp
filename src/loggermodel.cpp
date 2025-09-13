#include "loggermodel.h"

LoggerModel::LoggerModel(QObject* parent) : QAbstractListModel(parent) {}

int LoggerModel::rowCount(const QModelIndex &parent) const { Q_UNUSED(parent); return m_list.size(); }

QVariant LoggerModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid()) return {};
    const auto &item = m_list.at(index.row());
    switch (role) {
        case TypeRole: return item.type;
        case IDRole: return item.id;
        case LocationRole: return item.location;
        case TemperatureRole: return item.temperature;
        case HumidityRole: return item.humidity;
        case TimestampRole: return item.timestamp;
    }
    return {};
}

QHash<int, QByteArray> LoggerModel::roleNames() const {
    return {
        {TypeRole, "type"},
        {IDRole, "id"},
        {LocationRole, "location"},
        {TemperatureRole, "temperature"},
        {HumidityRole, "humidity"},
        {TimestampRole, "timestamp"}
    };
}

void LoggerModel::setDataList(const QList<LoggerData>& list) {
    beginResetModel();
    m_list = list;
    endResetModel();
    emit dataChanged();   // notify QML when new data arrives
}

void LoggerModel::setLoading(bool l) {
    if (m_loading == l) return;
    m_loading = l;
    emit loadingChanged();
}

void LoggerModel::setConnectionState(const QString &s) {
    if (m_connectionState == s) return;
    m_connectionState = s;
    emit connectionStateChanged();
}


void LoggerModel::refresh() {
    setLoading(true);
    QNetworkRequest req(QUrl("http://127.0.0.1:8800/loggers"));
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QNetworkReply *reply = globalNam->get(req);

    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        setLoading(false);

        if (reply->error() != QNetworkReply::NoError) {
            //server not reachable → Disconnected
            setConnectionState("Disconnected");
            qWarning() << "Network error:" << reply->errorString();
            reply->deleteLater();
            return;
        }

        QByteArray raw = reply->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(raw);

        QList<LoggerData> list;
        if (doc.isArray()) {
            QJsonArray arr = doc.array();
            for (const auto &v : std::as_const(arr)) {
                QJsonObject obj = v.toObject();
                LoggerData d;
                d.type = obj["type"].toString();
                d.id = obj["id"].toDouble();
                d.location = obj["location"].toString();
                d.temperature = obj["temperature"].toDouble();
                d.humidity = obj["humidity"].toDouble();
                d.timestamp = QDateTime::fromString(obj["timestamp"].toString(), Qt::ISODate);
                list.append(d);
            }
        }

        setDataList(list);
        setConnectionState("Connected"); //success
        reply->deleteLater();
    });
}

QVariantMap LoggerModel::get(int index) const {
    QVariantMap map;
    if (index < 0 || index >= m_list.size()) return map;
    const LoggerData &item = m_list.at(index);
    map["type"] = item.type;
    map["id"] = item.id;
    map["location"] = item.location;
    map["temperature"] = item.temperature;
    map["humidity"] = item.humidity;
    map["timestamp"] = item.timestamp;
    return map;
}
