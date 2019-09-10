#ifndef TASK_H
#define TASK_H

#include <QThread>

#include <QtTest/QTest> // так как в условии указано, что задачи должны выполнять что-то
// подключенна библиотека для тестирования, будем использовать искуственную задержку
// необходимо не тольк оее заинклюдить, но и в в файле .pro  указать "QT += testlib"

enum typeTask { // перечисления для типа заадчи
    proc, // задача которая для вывода процентной статистики
    noProc // задача которая без процентной статистики
};


class Task : public QThread // наследуем от нити в QT
{
    Q_OBJECT
public:
    explicit Task(QThread *parent = nullptr);

    Task(// конструктор

         // переменные которые относятся к статистике задачи
         typeTask type, // тип задачи
         unsigned int finishdTask_value, // конечное значение


         // переменные которые относятся к телу задачи
         unsigned int minDelayTime,
         unsigned int maxDelayTime
         );

    unsigned int getFinishdTaskValue(); // возврящает максимальное число итераций в задаче
    // в коде не используется, была добавлена приотладке
    typeTask getTaskType(); // возвращает тип задачи, с поцентами или без статистика ведется
    bool isComlete(); // завершена ли задача
    // в коде не используется, была добавлена приотладке

signals:

    void currentTaskValueChanged(Task * task, unsigned int newValue); // сигнал об изменении текущего значения

    void currentTaskStatusChanged(Task * task, bool newStatus); // сигнал об изменении статуса

public slots:

private:

    unsigned int finishdTask_value = 0; // значение при котором задача считается выполненной
    unsigned int currentTask_value = 0; // текущее значение задачи
    // количество пакетов, итераций цикла, распакованных файлов и т.д.

    bool complete = false; // флаг завершения задачи
    typeTask type = noProc; // тип задачи


    //---- далее все что относится к основнму телу задачи -----------------------//

    void bodyIterationTask ( // основная функция итерации задачи, какое-то вычисление
                             unsigned int minDelayTime,
                             unsigned int maxDelayTime
                             );

    unsigned int minDelayTime; // минимальное время задержки при итерации
    unsigned int maxDelayTime; // максимальное время задержки при итерации
    // так как вместо сложного математического расчета
    // будет использована искуственная задержка
    // то размер искуственной задержки будем брать от minDelayTime до maxDelayTime

    //----------------------------------------------------------------------------

protected:

    void run() override; // переопределим функцию для запуска задачи

};

#endif // TASK_H
