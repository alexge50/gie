//
// Created by alex on 5/18/19.
//

#include "mainwindow.h"
#include <QApplication>

#include <boost/python.hpp>
#include <iostream>

int main(int argc, char *argv[])
{
    int ret = -1;
    try
    {
        QApplication a(argc, argv);
        MainWindow w;
        w.show();

        ret = QApplication::exec();
    }
    catch(std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }


    return ret;
}
