#include <QtTest/QtTest>
#include "loggermodel.h"
#include <QNetworkAccessManager>
QNetworkAccessManager* globalNam = nullptr;

class TestLoggerModel : public QObject {
    Q_OBJECT

private slots:
    void initTestCase();
    void testAddLogEntry();
    void testRowCount();
    void testLogData();
    void cleanupTestCase();

private:
    LoggerModel *model;
};

void TestLoggerModel::initTestCase() {
    model = new LoggerModel();
}

void TestLoggerModel::testAddLogEntry() {
    // Create a single log entry
    LoggerData entry;
    entry.type = "H1";
    entry.id = 1;
    entry.location = "Cold Storage";
    entry.temperature = 25.0;
    entry.humidity = 50.0;
    entry.timestamp = QDateTime::currentDateTime();

    QList<LoggerData> list;
    list.append(entry);

    // Use the current API to set data
    model->setDataList(list);

    // Verify row count
    QCOMPARE(model->rowCount(), 1);

    // Verify data
    QModelIndex idx = model->index(0, 0);
    QCOMPARE(model->data(idx, LoggerModel::TypeRole).toString(), QString("H1"));
    QCOMPARE(model->data(idx, LoggerModel::IDRole).toDouble(), 1.0);
    QCOMPARE(model->data(idx, LoggerModel::LocationRole).toString(), QString("Cold Storage"));
    QCOMPARE(model->data(idx, LoggerModel::TemperatureRole).toDouble(), 25.0);
    QCOMPARE(model->data(idx, LoggerModel::HumidityRole).toDouble(), 50.0);
}

void TestLoggerModel::testRowCount() {
    // Add another entry
    LoggerData entry;
    entry.type = "H2";
    entry.id = 2;
    entry.location = "Freezer";
    entry.temperature = 30.0;
    entry.humidity = 40.0;
    entry.timestamp = QDateTime::currentDateTime();

    QList<LoggerData> list = model->dataList();
    list.append(entry);

    model->setDataList(list);

    QCOMPARE(model->rowCount(), 2);
}

void TestLoggerModel::testLogData()
{
    int lastRow = model->rowCount() - 1;
    QModelIndex idx = model->index(lastRow, 0);

    QVariantMap data = model->get(lastRow);
    QVERIFY(data.contains("type"));
    QVERIFY(data.contains("id"));
    QVERIFY(data.contains("location"));
    QVERIFY(data.contains("temperature"));
    QVERIFY(data.contains("humidity"));
}

void TestLoggerModel::cleanupTestCase() {
    delete model;
}

QTEST_MAIN(TestLoggerModel)
#include "tst_loggermodel.moc"
