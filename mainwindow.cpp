#include "mainwindow.h"
#include "ui_mainwindow.h"

// конструктор главного окна
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) // иниц. пользовательский интерфейс
    , boris(new Boris) // иниц. бота
{
    ui->setupUi(this);
    ui->lineEdit->setStyleSheet("color: white"); // стиль границ

    // графическая структура ответа бота
    ui->textBrowser->setTextColor(QColor(12,255,249));// цвет имени
    ui->textBrowser->append("Борис (◕‿◕)"); // имя
    ui->textBrowser->setTextColor(QColor(255,255,255)); // цвет реплики (белый)
    ui->textBrowser->append("Привет, я Борис! Как Вас зовут?\n"); // реплика - приветствие
}

// деструктор
MainWindow::~MainWindow()
{
    delete ui;
    delete boris;
}

// обработчик нажатия клавиши Enter
void MainWindow::on_lineEdit_returnPressed()
{
    if (!ui->lineEdit->text().isEmpty())
    {
        QString msg = UserReply();
        BorisReply(msg);
    }
}

// ответ бота на полученную фразу пользователя
void MainWindow::BorisReply(QString phrase)
{
    // граф. структура ответа
    ui->textBrowser->setTextColor(QColor(12,255,249));
    ui->textBrowser->append("Борис (◕‿◕)");
    ui->textBrowser->setTextColor(QColor(255,255,255));
    ui->textBrowser->append(boris->Conversation(phrase) + "\n"); // вызываем главный метод - разговор, передаем в него фразу пользователя
}

// ответ пользователя
QString MainWindow::UserReply()
{
    QString msg = ui->lineEdit->text();
    ui->lineEdit->clear();

    // граф. структура реплики пользователя
    ui->textBrowser->setTextColor(QColor(255,0,204));
    ui->textBrowser->append("Вы");
    ui->textBrowser->setTextColor(QColor(255,255,255));
    ui->textBrowser->append(msg + "\n");

    return msg;
}
