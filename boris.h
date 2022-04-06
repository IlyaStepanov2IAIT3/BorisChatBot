#ifndef BORIS_H
#define BORIS_H
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QDir>

class Boris
{
private:
    QFile file;
    QTextStream *stream;
    QDir dir;
    QList<int> answers_num; // вспомогательный список номеров ответов
    QList<int> questions_num; // вспомогательный список номеров вопросов
    QString username;
    bool isHello = false; // флаг - было ли приветствие
    bool hasTopicChanged = false; // флаг - поменялась ли тема разговора

public:
    Boris();
    ~Boris();

    QString GreetingsReplay(QString phrase); // метод - приветствие (ответ бота на введеное польз. имя)
    QString Conversation(QString phrase); // основной метод разговора, вызывается при отправке пользователем сообщения
    QString topicChange(QString phrase); // метод, вызывающийся при запросе сменить директорию - меняет директорию
    bool isAnsRepetition(int phrase, int phr_num); // метод, проверяющий, есть ли такой ответ в файле
    bool isQueRepetition(int phrase, int phr_num); // метод, проверяющий, есть ли такой вопрос в файле
    QString randomAns(); // метод, возвращающий случайный ответ из файла
    QString randomQue(); // метод, возвращающий случайный вопрос из файла
    void addQueToFile(QString phrase); // метод, записывающий вопрос пользователя в файл
};

#endif // BORIS_H
