/*
 * File:   main.cpp
 * Author: rafal
 *
 * Created on 6 marzec 2011, 13:38
 */

#include <QtGui/QApplication>
#include <MainWindow.h>
#include <AboutDialog.h>

int main(int argc, char *argv[]) {
    //initialize resources, if needed
    //Q_INIT_RESOURCE(resfile);

    QApplication app(argc, argv);
    MainWindow mainWindow;
    
    mainWindow.show();

    return app.exec();
}
