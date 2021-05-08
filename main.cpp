#include <QCoreApplication>

#include "gameplay.h"

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);
    Gameplay gp;
    if (!gp.start())
    {
        return 1;
    }
    return app.exec();
}
