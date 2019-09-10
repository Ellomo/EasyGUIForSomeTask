#include "task.h"

Task::Task(QThread *parent) : QThread(parent)
{

}

Task::Task(
        typeTask type,
        unsigned int finishdTask_value,

        unsigned int minDelayTime,
        unsigned int maxDelayTime
        )
{
    // переприсвоим полученные значения
    this->type = type;
    this->finishdTask_value = finishdTask_value;

    if (maxDelayTime > minDelayTime){ // простая проверка не перепутал ли пользователь порядок аргументов
        this->minDelayTime = minDelayTime;
        this->maxDelayTime = maxDelayTime;
    }else {
        this->minDelayTime = maxDelayTime;
        this->maxDelayTime = minDelayTime;
    }

}

unsigned int Task::getFinishdTaskValue()
{
    return this->finishdTask_value;
}

typeTask Task::getTaskType()
{
    return this->type;
}

bool Task::isComlete()
{
    return this->complete;
}

void Task::run()
{

    currentTaskStatusChanged(this, this->complete); // посылаем сигнал о том, что статус изменен

    // от текущего значения до конечного выполняем задачу
    for (; this->currentTask_value < this->finishdTask_value; this->currentTask_value++) {

        currentTaskValueChanged(this, this->currentTask_value); // посылаем сигнал о изменении текущего значения

        bodyIterationTask(this->minDelayTime, this->maxDelayTime); // выполним одну итерацию

    }

    currentTaskValueChanged(this, this->currentTask_value); // посылаем сигнал о изменении текущего значения
    // еще раз, т.к. последний сигнал был со значением finishdTask_value-1

    this->complete = true; // меняем флаг о том, что здача завершена

    currentTaskStatusChanged(this, this->complete); // посылаем сигнал о том, что статус изменен


}

void Task::bodyIterationTask(unsigned int minDelayTime, unsigned int maxDelayTime)
{
    QTest::qSleep( // искуственная задержка
                   qrand() % ((maxDelayTime + 1) - minDelayTime) + minDelayTime // время в милисикундах, генерируемое от minDelayTime до maxDelayTime
                   );
}
