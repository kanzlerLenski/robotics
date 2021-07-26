#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "./lib/controllerbackend.h"
#include "pid_controller.h"
#include "integrator.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication app(argc, argv);

    ControllerBackend controllerBackend(50, "Zsdnw82iuw");

    PID_controller pid_controller;

    QQmlApplicationEngine engine;

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);

    engine.rootContext()->setContextProperty("plant", &controllerBackend);

    QObject::connect(&controllerBackend, &ControllerBackend::outputBytesChanged,
                     &pid_controller, &PID_controller::get);

    QObject::connect(&pid_controller, &PID_controller::generatedReference,
                    &controllerBackend, &ControllerBackend::setReferenceSignal);


    QObject::connect(&pid_controller, &PID_controller::generatedInput,
                    &controllerBackend, &ControllerBackend::setInput);

   //    QObject::connect(&controllerBackend, &ControllerBackend::outputBytesChanged,
//                     &testController, &TestController::computeBytes);

//    QObject::connect(&testController, &TestController::computed,
//                     &controllerBackend, &ControllerBackend::setInput);

    engine.load(url);


    return app.exec();
}
