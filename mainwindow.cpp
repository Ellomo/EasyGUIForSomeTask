#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::currentTaskValueChanged(Task * inputTask , unsigned int newValue) // изменяем статистику о задаче когда приходит сигнал
{
    if (this->removing_flag){return;} // если сейчас процес удаления, то не выполняем функцию

    ui->task_tableWidget->setSortingEnabled(false); // эта конструкция используется во во всех методах она отклячает сортировку в таблице
    // зачем это надо объяснено в коментарии к стрктуре TaskAndRow
    // мы можем поменять информацию не о нужной нам задаче если не выключим сортировку

    //находим индексы
    unsigned int indexTaskInList = findIndexTaskInList(inputTask);
    unsigned int indexRowTask = findRowTaskInTableWidget(indexTaskInList);

    // находим процентаж и обновляем его в ячейке таблицы
    ui->task_tableWidget->item( indexRowTask , 0 )->setText(QString::number(newValue / (FINISHD_VALUE / 100)));
    QProgressBar * progressTaskBar = (QProgressBar *) ui->task_tableWidget->cellWidget(indexRowTask , 0 );
    progressTaskBar->setValue(newValue / (FINISHD_VALUE / 100));

    // по завершению метода включаем сортровку таблицы
    ui->task_tableWidget->setSortingEnabled(true);
}

void MainWindow::currentTaskStatusChanged(Task *inputTask, bool newStatus) // меняем статус
// анологично предыдущему методу только обновляем уже статус о завершении
{

    while (this->removing_flag){;} // если операция удаления активна то ждем

    ui->task_tableWidget->setSortingEnabled(false);

    unsigned int indexTaskInList = findIndexTaskInList(inputTask);

    unsigned int indexRowTask = findRowTaskInTableWidget(indexTaskInList);


    if (newStatus == 1){ // если задача завершилась
        //то

        ui->task_tableWidget->item( indexRowTask , 1 )->setText("Завершена"); // обновляем запись в талице
        if (inputTask->getTaskType() == proc ){  // а если задача с процентным типом статистики то обновляем процент бар
            // это нужно потому что из метода изменения статистики (предыдущий метод в коде)
            // был просто выход при выполнении операции удаления
            // а в этом методе мы именно ждали
            ui->task_tableWidget->item( indexRowTask , 0 )->setText(QString::number(100));
            QProgressBar * progressTaskBar = (QProgressBar *) ui->task_tableWidget->cellWidget(indexRowTask , 0 );
            progressTaskBar->setValue(100);

        }
    } else {
        // иначе это означает, что задача только, что была создана
        ui->task_tableWidget->item( indexRowTask , 1 )->setText("В процессе");
    }

    ui->task_tableWidget->setSortingEnabled(true);

}

void MainWindow::on_addTaskProc_button_clicked() // добавляем задачу с процентным типом статистики
{

    ui->task_tableWidget->setSortingEnabled(false);

    TaskAndRow newTask; // новый экземпляр структуры
    newTask.task = new Task(proc, FINISHD_VALUE, MIN_DELAY_TIME_FOR_ITERATION, MAX_DELAY_TIME_FOR_ITERATION); // новая задача

    ui->task_tableWidget->insertRow(ui->task_tableWidget->rowCount()); // новая строка
    for (unsigned int i = 0; i < ui->task_tableWidget->horizontalHeader()->count(); i++ ) { // новые элементы в ячейки строки
        ui->task_tableWidget->setItem(ui->task_tableWidget->rowCount()-1 , i , new QTableWidgetItem);
    }

    ui->task_tableWidget->setCellWidget(ui->task_tableWidget->rowCount()-1 , 0 , new QProgressBar(0)); // новый процент бар в первую ячейку строки

    newTask.row = ui->task_tableWidget->item( ui->task_tableWidget->rowCount()-1 , 0 ); // меняем в экземпляре структуры указатель на строку таблицы (точнее на первую ячейку строки)

    listTask.append(newTask); // добавляем в массв задач экземпляр структуры

    connect( // конект на изменение текущего значения задачи
             newTask.task ,
             &Task::currentTaskValueChanged,
             this,
             &MainWindow::currentTaskValueChanged
             );

    connect( // конект на изменение статуса задачи
             newTask.task ,
             &Task::currentTaskStatusChanged,
             this,
             &MainWindow::currentTaskStatusChanged
             );

    listTask.last().task->start(); // поскольку задача унаследована от патокаа ее необходимо запустить

    ui->task_tableWidget->setSortingEnabled(true);

}

void MainWindow::on_addSomeTaskProc_button_clicked()// создание нескольких задач с процентным типом статистики
{
    for (unsigned int i = 0 ; i < ui->countTask->text().toUInt(); i++){ // в цикле создаем несколько задач
        this->on_addTaskProc_button_clicked();
    }
}

void MainWindow::on_addTaskBool_button_clicked() // создание задач с беспроцентным типом статистики <- именно такой вариант требовался по условию
// аналогично методу с процентным типом статистики
{
    ui->task_tableWidget->setSortingEnabled(false);

    TaskAndRow newTask;
    newTask.task = new Task(noProc, FINISHD_VALUE, MIN_DELAY_TIME_FOR_ITERATION, MAX_DELAY_TIME_FOR_ITERATION); // только в конструктор передается другой тип

    ui->task_tableWidget->insertRow(ui->task_tableWidget->rowCount());
    for (unsigned int i = 0; i < ui->task_tableWidget->horizontalHeader()->count(); i++ ) {
        ui->task_tableWidget->setItem(ui->task_tableWidget->rowCount()-1 , i , new QTableWidgetItem);
    }

    ui->task_tableWidget->item( ui->task_tableWidget->rowCount()-1 , 0 )->setText("-");

    newTask.row = ui->task_tableWidget->item( ui->task_tableWidget->rowCount()-1 , 0 );

    listTask.append(newTask);

    // и нет конекта на обновление прогрес бара

    connect(
                newTask.task ,
                &Task::currentTaskStatusChanged,
                this,
                &MainWindow::currentTaskStatusChanged
                );

    listTask.last().task->start();

    ui->task_tableWidget->setSortingEnabled(true);

}

void MainWindow::on_addSomeTaskBool_button_clicked() // создание нескольких задач с беспроцентной статистикой
{
    for (unsigned int i = 0 ; i < ui->countTask->text().toUInt(); i++){
        this->on_addTaskBool_button_clicked();
    }
}

void MainWindow::on_removeComleteTasks_button_clicked() // удаление завершенных задач
{

    if (this->removing_flag){return;} // если уже идет удаление, то выходим из метода

    ui->task_tableWidget->setSortingEnabled(false);

    this->removing_flag = true; // ставим флаг о том, что идет удаление

    for ( int i = ui->task_tableWidget->rowCount() - 1 ; i > -1 ; i--) { // смотрим строки в которых задачи определены как завершенные

        if (ui->task_tableWidget->item(i,1)->text()=="Завершена"){ // находим такие строки

            unsigned int indexTaskInList = findIndexTaskInList(i); // находим по номеру строки индекс задачи в массиве
            ui->task_tableWidget->removeRow(i); // удаляем строку

            if (listTask[indexTaskInList].task->getTaskType() == proc){ // если задача была с процентным типом статистики, то
                disconnect( // дисконект на изменение текущего значения задачи
                            listTask[indexTaskInList].task ,
                            &Task::currentTaskValueChanged,
                            this,
                            &MainWindow::currentTaskValueChanged
                            );
            }

            disconnect( // дисконект на изменение статуса задачи
                        listTask[indexTaskInList].task ,
                        &Task::currentTaskStatusChanged,
                        this,
                        &MainWindow::currentTaskStatusChanged
                        );

            delete listTask[indexTaskInList].task;// удаление задачи
            // удаление listTask[indexTaskInList].row не нужно так как мы удалили всю строку ранее
            listTask.remove(indexTaskInList); // удаление экземпляра массива
        }
    }

    this->removing_flag = false; // меняем флаг, удаление закончено

    ui->task_tableWidget->setSortingEnabled(true);

}


// поиски индексов реализованы обычными циклами
unsigned int MainWindow::findIndexTaskInList(Task *inputTask)
{
    ui->task_tableWidget->setSortingEnabled(false);
    for (unsigned int i = 0 ; i < listTask.count() ; i++){
        if (listTask[i].task == inputTask){
            return i;
        }
    }
}

unsigned int MainWindow::findIndexTaskInList(unsigned int indexRowTask)
{
    ui->task_tableWidget->setSortingEnabled(false);
    for (unsigned int i = 0 ; i < listTask.count() ; i++){
        if (listTask[i].row == ui->task_tableWidget->item(indexRowTask,0)){
            return i;
        }
    }
}

unsigned int MainWindow::findRowTaskInTableWidget(unsigned int indexTaskInList)
{
    ui->task_tableWidget->setSortingEnabled(false);
    for (unsigned int i = 0 ; i < ui->task_tableWidget->rowCount() ; i++){
        if (listTask[indexTaskInList].row == ui->task_tableWidget->item( i , 0 )){
            return i;
        }
    }
}

