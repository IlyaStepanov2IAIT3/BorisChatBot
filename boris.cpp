#include "boris.h"

// конструктор
Boris::Boris()
    : stream(new QTextStream(&file))
{
    dir.setPath("Common"); // тема разговора по умолчанию - повседневная
}

// деструктор
Boris::~Boris()
{
    delete stream;
}

// метод - приветствие
QString Boris::GreetingsReplay(QString phrase)
{
    if (phrase.contains("не", Qt::CaseSensitivity::CaseInsensitive) ||
            phrase.contains("нет", Qt::CaseSensitivity::CaseInsensitive))
    {
        username = "Анонимус";
        return "Хорошо,  буду называть тебя Анонимус";
    }
    username = phrase; // сохранение введенного пользователем имени
    return "Приятно познакомиться,  " + phrase + "!";
}

// метод - смена темы разговора
QString Boris::topicChange(QString phrase)
{
    QString msg; // переменная-результат выполнения метода, в нее запишется сообщение на выход к пользователю

    QDir mainDir;
    QStringList stringDirs = mainDir.entryList(QDir::Dirs); // список директорий главной директории

    //для каждой директории
    foreach(QString stringDir, stringDirs)
    {
        if (stringDir != "." && stringDir != "..") // пропускаем системные директории
        {
            QDir thisDir(stringDir);
            QStringList stringFiles = thisDir.entryList(QDir::Files); // список файлов данной директории

            //для каждого файла
            foreach(QString stringFile, stringFiles)
            {
                if (stringFile == "key.txt") // если у файла имя "key.txt"
                {
                    QFile thisFile(stringDir + "/key.txt");
                    if (thisFile.open(QFile::ReadOnly)) // открываем файл
                    {
                        QTextStream temp_stream(&thisFile); // открываем поток
                        QString keyPhrase = temp_stream.readLine(); // считываем фразу в файле
                        if (keyPhrase == phrase)
                        {
                            dir = thisDir; // сохраняем эту директорию в поле
                            answers_num.clear(); // очищаем список ответов
                            questions_num.clear(); // очищаем список вопросов
                            hasTopicChanged = true; // поднимаем флаг о смене темы разговора
                            return "Да,  давай сменим тему"; // результат
                        }
                        thisFile.close(); // закрываем файл
                    }
                    else
                        msg = "Ошибка открытия ключевого файла директории " + stringDir; // результат в случае ошибки
                }
            }
        }
    }
    return msg; // если тема разговора не сменилась - возвращаем пустое сообщение
}

// основной метод-разговор
QString Boris::Conversation(QString phrase)
{
    srand(time(NULL)); // обновление генератора случайных чисел
    if (phrase == "Подбрось монетку") return rand() % 2 == 0 ? "Орёл!" : "Решка!"; // если пользователь написал "Подбрось монетку"

    // приветствие
    if (!isHello)
    {
        isHello = true;
        return GreetingsReplay(phrase);
    }
    hasTopicChanged = false;
    QString msg;
    msg = topicChange(phrase); // проверяем фразу пользователя - если она - запрос на смену темы разговора - меняем тему

    // если тема разговора не была изменена
    if (!hasTopicChanged)
    {
        if (phrase.contains("?")) // если фраза вопрос
        {
            msg = randomAns(); // случайный ответ
            addQueToFile(phrase); // запоминаем вопрос пользователя
        }
        else
        {
            msg = randomQue(); // случайный вопрос
        }
    }
    return msg; // результат
}

// проверка фразы-ответа на повторение
bool Boris::isAnsRepetition(int phrase, int phr_num)
{
    // если список ответов полон
    if (answers_num.count() == phr_num)
        answers_num.clear(); // очищаем список

    for (int i = 0; i < answers_num.count(); i++) // для каждого ответа в списке ответов
    {
        if (answers_num[i] == phrase) // если такой ответ был
            return true; // возвращаем истину
    }
    return false; // возвращаем ложь
}

// аналогично проверке фразы-ответа
bool Boris::isQueRepetition(int phrase, int phr_num)
{
    if (questions_num.count() == phr_num)
        questions_num.clear();

    for (int i = 0; i < questions_num.count(); i++)
    {
        if (questions_num[i] == phrase)
            return true;
    }
    return false;
}

// генерация случайного овтета из файла
QString Boris::randomAns()
{
    QString msg; // переменная-результат
    file.setFileName(dir.path() + "/" + "answers.txt"); // устанав. путь переменной-файла на файл с ответами
    if (file.open(QFile::ReadOnly)) // открываем файл в режиме чтения
    {
        int count = 0; // счётчик
        stream->seek(0); // устанавливаем указатель поток на начало файла
        while (!stream->atEnd()) // пока не дошли до конца файла
        {
            msg = stream->readLine(); // считываем из потока строки-ответы
            count++; // инкрементируем счётчик
        }
        if (count > 1)
        {
            int random;
            do
            {
                random = rand() % count; // генерируем случайное число
                stream->seek(0);
                for (int i = 0; i < random; i++)
                {
                    msg = stream->readLine(); // находим нужную строку-ответ
                }
            }
            while (isAnsRepetition(random, count)); // выполняем пока не нашли ответ, которого еще не было
            answers_num.append(random); // заносим ответ в список ответов
            if (random % 2 != 0) msg = username + ",  " + msg.toLower(); // каждый второй ответ бот обращается к пользователю по имени
        }
        file.close(); // закрываем файл
    }
    return msg; // результат
}

// аналогично генерации случайного ответа
QString Boris::randomQue()
{
    QString msg;
    file.setFileName(dir.path() + "/" + "questions.txt");
    if (file.open(QFile::ReadOnly))
    {
        int count = 0;
        stream->seek(0);
        while (!stream->atEnd())
        {
            msg = stream->readLine();
            count++;
        }
        if (count > 1)
        {
            int random;
            do
            {
                random = rand() % count;
                stream->seek(0);
                for (int i = 0; i < random; i++)
                {
                    msg = stream->readLine();
                }
            }
            while (isQueRepetition(random, count));
            questions_num.append(random);
            if (random % 2 != 0) msg = username + ",  " + msg.toLower();
        }
        file.close();
    }
    return msg;
}

// метод - добавление вопроса в файл
void Boris::addQueToFile(QString phrase)
{
    file.setFileName(dir.path() + "/" + "questions.txt"); // устанавливаем путь переменной-файла как файл с вопросами
    if (file.open(QFile::ReadWrite | QFile::Append)) // открываем файл в режиме чтения-записи, а также в режиме дополнения, а не перезаписи
    {
        // проверяем - есть ли такой вопрос в файле
        QString temp;
        bool isRepetition = false;
        stream->seek(0);
        while(!stream->atEnd())
        {
            temp = stream->readLine();
            if (temp == phrase)
                isRepetition = true;
        }
        if (!isRepetition) // если нет
            *stream << phrase << "\n";
        file.close(); // закрываем файл
    }
}
