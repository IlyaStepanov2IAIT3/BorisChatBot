#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv); // объект - приложение
    MainWindow w; // объект - главное окно
    w.setStyleSheet("background-color:rgb(44,44,44); border-style:rgb(44,44,44)"); // темная тема
    w.show(); // отображаем окно
    return a.exec(); // выполняем приложение
}

