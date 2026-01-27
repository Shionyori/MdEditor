// src/main.cpp
#include "MainWindow.h"
#include <QApplication>
#include <QtWebEngineQuick/QtWebEngineQuick>

int main(int argc, char* argv[])
{
    QCoreApplication::setAttribute(Qt::AA_UseSoftwareOpenGL); // 强制 Qt 使用软件 OpenGL 实现
    QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts);

    QApplication app(argc, argv);

    MainWindow w;
    w.setWindowTitle("MdEditor");
    w.resize(1200, 900);
    w.show();

    return app.exec();
}