QT += qml quick charts

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Refer to the documentation for the
# deprecated API to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        derivative.cpp \
        integrator.cpp \
        lowpassfilter.cpp \
        main.cpp \
        pid_controller.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/lib/ -lControllerBackend
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/lib/ -lControllerBackend
else:unix: LIBS += -L$$PWD/lib/ -lControllerBackend


INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    derivative.h \
    integrator.h \
    lowpassfilter.h \
    pid_controller.h
