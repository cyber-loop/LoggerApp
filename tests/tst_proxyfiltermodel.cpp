#include <QtTest/QtTest>
#include "loggermodel.h"
#include "proxyfiltermodel.h"
#include <QNetworkAccessManager>
QNetworkAccessManager* globalNam = nullptr;

class TestProxyFilterModel : public QObject {
    Q_OBJECT

private slots:
    void initTestCase();
    void testFilterByType();
    void testFilterByLocation();
    void testFilterByTypeAndLocation();
    void cleanupTestCase();

private:
    LoggerModel *model;
    ProxyFilterModel *proxy;
};

void TestProxyFilterModel::initTestCase() {
    model = new LoggerModel();
    LoggerData infoEntry;
    infoEntry.type = "H1";
    infoEntry.id = 1;
    infoEntry.location = "Cold Storage";
    infoEntry.temperature = 20.0;
    infoEntry.humidity = 50.0;
    infoEntry.timestamp = QDateTime::currentDateTime();

    LoggerData errorEntry;
    errorEntry.type = "ERROR";
    errorEntry.id = 2;
    errorEntry.location = "Usman Room";
    errorEntry.temperature = 25.0;
    errorEntry.humidity = 40.0;
    errorEntry.timestamp = QDateTime::currentDateTime();

    QList<LoggerData> list;
    list.append(infoEntry);
    list.append(errorEntry);

    // Set data in LoggerModel
    model->setDataList(list);

    proxy = new ProxyFilterModel();
    proxy->setSourceModel(model);
}

void TestProxyFilterModel::testFilterByType() {
    // Set filter to only show "ERROR" logs
    proxy->setTypeFilter("ERROR");
    QCOMPARE(proxy->rowCount(), 1);

    QModelIndex idx = proxy->index(0, 0);
    QVERIFY(idx.isValid());

    // Access the message via source model using the proxy
    QModelIndex sourceIdx = proxy->mapToSource(idx);
    QVERIFY(sourceIdx.isValid());

    QVariantMap data = model->get(sourceIdx.row());
    QCOMPARE(data["type"].toString(), QString("ERROR"));
    //QCOMPARE(data["location"].toString(), QString("Cold Storage"));

    // Reset filter
    proxy->setTypeFilter("");
    QVERIFY(proxy->rowCount() >= 2);
}

void TestProxyFilterModel::testFilterByLocation() {
    proxy->setLocationFilter("Usman Room");
    QCOMPARE(proxy->rowCount(), 1);

    QModelIndex idx = proxy->index(0, 0);
    QVERIFY(idx.isValid());

    QModelIndex sourceIdx = proxy->mapToSource(idx);
    QVERIFY(sourceIdx.isValid());

    QVariantMap data = model->get(sourceIdx.row());
    QCOMPARE(data["location"].toString(), QString("Usman Room"));

    // Reset filter
    proxy->setLocationFilter("");
    QCOMPARE(proxy->rowCount(), 2);
}

void TestProxyFilterModel::testFilterByTypeAndLocation() {
    proxy->setTypeFilter("ERROR");
    proxy->setLocationFilter("Usman Room");

    QCOMPARE(proxy->rowCount(), 1);

    QModelIndex idx = proxy->index(0, 0);
    QVERIFY(idx.isValid());

    QModelIndex sourceIdx = proxy->mapToSource(idx);
    QVERIFY(sourceIdx.isValid());

    QVariantMap data = model->get(sourceIdx.row());
    QCOMPARE(data["type"].toString(), QString("ERROR"));
    QCOMPARE(data["location"].toString(), QString("Usman Room"));

    // Reset filters
    proxy->setTypeFilter("");
    proxy->setLocationFilter("");
    QCOMPARE(proxy->rowCount(), 2);
}

void TestProxyFilterModel::cleanupTestCase() {
    delete proxy;
    delete model;
}

QTEST_MAIN(TestProxyFilterModel)
#include "tst_proxyfiltermodel.moc"
