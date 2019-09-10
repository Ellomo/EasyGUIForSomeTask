#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define MIN_DELAY_TIME_FOR_ITERATION 20 // минимальное время итерации в задаче
#define MAX_DELAY_TIME_FOR_ITERATION 100 // максимальное время итерации в задаче
#define FINISHD_VALUE 100 // количество итераций в задаче (например количество файлов, пакетов, рачсетов)

#include <QMainWindow>
#include <QTableWidgetItem>
#include <QVector>
#include <QProgressBar>

#include "task.h"

struct TaskAndRow { // эта структура потребудется для возможности сортировки задач в TableWidget

    // дело в том, что если оставить в TableWidget возможность сортировки и провести сортировку по какому-либо параметру,
    // то индексы строк в TableWidget меняются

    /*

      пример
            имя телефон
      1     А   +7999
      2     С   +7999
      3     B   +7920

      проводим сортировку по имени

            имя телефон
      1     А   +7999
      2     B   +7920
      3     C   +7999

      индексы строк 2 и 3 поменялись

    */

    Task *task; // указатель на здачу
    QTableWidgetItem *row; // указатель на строку в таблице

};


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void currentTaskValueChanged(Task * inputTask , unsigned int newValue); // слот об изменении текущего значения задачи
    void currentTaskStatusChanged(Task * inputTask , bool newStatus); // сигнал об изменении статуса задачи

    void on_addTaskProc_button_clicked(); // слот нажатия кнопки добавления задачи с процентным типом статистики <- такой вариант я добавил

    void on_addSomeTaskProc_button_clicked(); // слот нажатия кнопки добавления задач с процентным типом статистики

    void on_addTaskBool_button_clicked(); // слот нажатия кнопки добавления задачи с безпроцентным типом статистики <- такой вариант требовался по условию

    void on_addSomeTaskBool_button_clicked(); // слот нажатия кнопки добавления задач с безпроцентным типом статистики

    void on_removeComleteTasks_button_clicked(); // удаление завершенных задач

private:

    QVector <TaskAndRow> listTask; // динамический массив задач и их строк в таблице

    bool removing_flag = false; // флаг удаляем ли мы сейчас строки

    unsigned int findIndexTaskInList(Task * inputTask); // по указателю на задачу найти ее индекс св массиве

    unsigned int findIndexTaskInList(unsigned int indexRowTask);  // по индексу строки найти задачу соответствующую ей

    unsigned int findRowTaskInTableWidget(unsigned int indexTaskInList); // по ндексу задачи найти запись таблицы =

    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
