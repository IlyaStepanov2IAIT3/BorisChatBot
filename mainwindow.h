#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#include <boris.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void BorisReply(QString phrase); // метод-ответ Бориса
    QString UserReply(); // метод-ответ пользователя

private slots:
    void on_lineEdit_returnPressed(); // обработчик нажатия клавиши "Enter"

private:
    Ui::MainWindow *ui;
    Boris *boris;
};
#endif // MAINWINDOW_H
