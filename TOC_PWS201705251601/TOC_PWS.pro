#-------------------------------------------------
#
# Project created by QtCreator 2017-05-18T19:53:45
#
#-------------------------------------------------

QT       += core gui \
            sql \
            charts \
            axcontainer



TARGET = TOC_PWS
TEMPLATE = app
# DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += main.cpp\
        TOC.cpp \
    S7/TypeTransform.cpp \
    Thread/FreshThread.cpp \
    Sql/exportdatabasetoexcel.cpp \
    Sql/SqlData.cpp \
    AxiaSetting/YAxiaSetting.cpp \
    AxiaSetting/XAxiaSetting.cpp \
    S7/snap7.cpp

HEADERS  += TOC.h \
    S7/TypeTransform.h \
    Thread/FreshThread.h \
    Sql/exportdatabasetoexcel.h \
    Sql/SqlData.h \
    AxiaSetting/YAxiaSetting.h \
    AxiaSetting/XAxiaSetting.h \
    S7/snap7.h

FORMS    += TOC.ui \
    AxiaSetting/YAxiaSetting.ui \
    AxiaSetting/XAxiaSetting.ui



RC_FILE +=icon.rc

RESOURCES += \
    Resource.qrc



LIBS += -L$$PWD/S7/ -lsnap7

INCLUDEPATH += $$PWD/S7
DEPENDPATH += $$PWD/S7


