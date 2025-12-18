#include <QApplication>
#include <QIcon>

#include "MainWindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    // Set application icon
    app.setApplicationName("ByteShrink");
    app.setApplicationVersion("1.0.0");
    app.setWindowIcon(QIcon(":/icon.png"));
    
    MainWindow w;
    w.resize(560, 420);
    w.setWindowIcon(QIcon(":/icon.png"));
    w.show();
    return app.exec();
}
