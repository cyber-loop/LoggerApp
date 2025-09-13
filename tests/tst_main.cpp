#include <QtTest/QtTest>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QNetworkAccessManager>

QNetworkAccessManager* globalNam = nullptr;

// Minimal test to check if main QML loads without crash
class TestMainApp : public QObject {
    Q_OBJECT

private slots:
    void testAppStartup();
};

void TestMainApp::testAppStartup() {
    int argc = 0;
    char **argv = nullptr;
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/Main.qml")));

    QVERIFY(!engine.rootObjects().isEmpty());
}

QTEST_MAIN(TestMainApp)
#include "tst_main.moc"
