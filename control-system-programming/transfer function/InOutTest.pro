#-------------------------------------------------
#
# Project created by QtCreator 2018-01-29T10:01:13
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = integratorSimpson
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    blocks/continuous/continuos.cpp \
    blocks/discrete_continuous/discrete_continuous.cpp \
    blocks/discrete_plant/discrete_plant.cpp \
    blocks/integrator/integrator.cpp \
    blocks/plant/plant.cpp \
        main.cpp \
        widget.cpp \
    qcustomplot.cpp \
    blocks/gain/gain.cpp

HEADERS += \
    blocks/continuous/continuos.h \
    blocks/discrete_continuous/discrete_continuous.h \
    blocks/discrete_plant/discrete_plant.h \
    blocks/integrator/integrator.h \
    blocks/plant/plant.h \
        widget.h \
    main.h \
    qcustomplot.h \
    blocks/gain/gain.h

FORMS += \
        widget.ui
