#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QTimer>
#include "loggermodel.h"
#include "proxyfiltermodel.h"

QNetworkAccessManager *globalNam = nullptr; // define global

void fetchFromRest(LoggerModel *loggerModel) {
    loggerModel->setLoading(true);
    loggerModel->setConnectionState("Connecting...");

    QNetworkRequest req(QUrl("http://127.0.0.1:8800/loggers"));
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QNetworkReply *reply = globalNam->get(req);

    QObject::connect(reply, &QNetworkReply::finished, [reply,loggerModel]() {
        loggerModel->setLoading(false);
        if (reply->error() == QNetworkReply::NoError) {
            loggerModel->setConnectionState("Connected");
            QByteArray body = reply->readAll();
            QJsonDocument doc = QJsonDocument::fromJson(body);
            if (doc.isArray()) {
                QList<LoggerData> list;
                for (auto v : doc.array()) {
                    auto o = v.toObject();
                    LoggerData ld;
                    ld.type = o["type"].toString();
                    ld.id = o["id"].toDouble();
                    ld.location = o["location"].toString();
                    ld.temperature = o["temperature"].toDouble();
                    ld.humidity = o["humidity"].toDouble();
                    ld.timestamp = QDateTime::fromString(o["timestamp"].toString(), Qt::ISODate);
                    list.append(ld);
                }
                loggerModel->setDataList(list);
            }
        } else {
            loggerModel->setConnectionState("Disconnected");
            qWarning() << "Network error:" << reply->errorString();
        }
        reply->deleteLater();
    });
};

int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    LoggerModel loggerModel;
    ProxyFilterModel proxyModel;
    proxyModel.setSourceModel(&loggerModel);
    proxyModel.setFilterCaseSensitivity(Qt::CaseInsensitive);

    engine.rootContext()->setContextProperty("loggerModel", &loggerModel);
    engine.rootContext()->setContextProperty("loggerProxy", &proxyModel);

    globalNam = new QNetworkAccessManager(&app);

    fetchFromRest(&loggerModel);

    const QUrl url(QStringLiteral("qrc:/Main.qml"));
    engine.load(url);

    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
