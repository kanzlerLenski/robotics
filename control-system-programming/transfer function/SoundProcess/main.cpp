#include "soundprocess.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SoundProcess w;
    w.show();
    return a.exec();
}
