QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ChartWidget.cpp \
    IChartAdapter.cpp \
    IDataLoader.cpp \
    JsonLoaderAdapter.cpp \
    LineChartAdapter.cpp \
    MainWindow.cpp \
    SQLiteLoaderAdapter.cpp \
    main.cpp

HEADERS += \
    ChartWidget.h \
    IChartAdapter.h \
    IDataLoader.h \
    JsonLoaderAdapter.h \
    LineChartAdapter.h \
    MainWindow.h \
    IOC_Contaner.h \
    SQLiteLoaderAdapter.h

FORMS += \
    MainWindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
