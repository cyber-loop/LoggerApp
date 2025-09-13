LoggerApp
=========================
This is a runnable LoggerApp demonstrating retrieval of Wi‑Fi logger data via a REST endpoint,
a QAbstractListModel (LoggerModel), a ProxyFilterModel (QSortFilterProxyModel wrapper),
and a QML UI with filterable list by type & location.

Build & Run (Ubuntu with Qt6):
1. Install Qt6 and dev packages (ensure qmake/cmake and Qt6 libraries).
2. Unzip and build:
   mkdir build && cd build
   cmake .. -DCMAKE_PREFIX_PATH=/path/to/Qt6/lib/cmake/Qt6
   make
3. Run:
   ./LoggerApp
The app starts a local fake REST server on localhost:8800 and fetches /loggers.

Files of interest:
 - src/loggermodel.h/.cpp
 - src/proxyfiltermodel.h
 - src/main.cpp  (fetches REST)
 - qml/Main.qml
 
Unit Test
1. mkdir build && cd build
2. cmake .. -DCMAKE_PREFIX_PATH=/path/to/Qt6/lib/cmake/Qt6 -DBUILD_TESTING=ON
3. make
4. ctest -V (To run all the unit tests)
5. ./tst_loggermodel ./tst_proxyfiltermodel ./tst_main to run individual tests
