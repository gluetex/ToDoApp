#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QStandardPaths>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QFile file(path);
    if(!file.open(QIODevice::ReadWrite)){
        QMessageBox::information(0, "error", file.errorString());
    }

    QTextStream in(&file);

    while(!in.atEnd()){
        QListWidgetItem* item = new QListWidgetItem(in.readLine(),ui->listWidget);
        ui->listWidget->addItem(item);
        item->setFlags(item->flags() | Qt::ItemIsEditable);
    }
    file.close();
}

MainWindow::~MainWindow()
{


    QFile file(path);
    if(!file.open(QIODevice::ReadWrite)){
        QMessageBox::information(0, "error", file.errorString());
    }

    QTextStream out(&file);

    for(int i=0; i < ui->listWidget->count(); i++){
        out<<ui->listWidget->item(i)->text()<<"\n";
    }

    file.close();

      delete ui;
}


void MainWindow::on_btnAdd_clicked()
{

      QString task = ui->txtTask->text().trimmed();
      if (!task.isEmpty()) {
        QListWidgetItem* item = new QListWidgetItem(task, ui->listWidget);
        ui->listWidget->addItem(item);
        item->setFlags(item->flags() | Qt::ItemIsEditable);
        ui->txtTask->clear();
        ui->txtTask->setFocus();
      } else {
        QMessageBox::warning(this, "Input Error", "Task cannot be empty.");
      }

}


void MainWindow::on_btnRemove_clicked()
{
     int currentRow = ui->listWidget->currentRow();

      if (currentRow >= 0) {
        QListWidgetItem* item = ui->listWidget->takeItem(currentRow);
        delete item;

        QFile file(path);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
            QMessageBox::information(0, "Error", file.errorString());
        } else {
            QTextStream out(&file);
            for (int i = 0; i < ui->listWidget->count(); i++) {
                out << ui->listWidget->item(i)->text() << "\n";
            }
            file.close();
        }
      }
}


void MainWindow::on_btnRemoveAll_clicked()
{
    ui->listWidget->clear();
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        QMessageBox::information(0, "Error", file.errorString());
    }
    file.close();

}

