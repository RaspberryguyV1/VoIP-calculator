#include "mainwindow.h"
#include "ui_mainwindow.h" // Ten plik wygeneruje się sam podczas kompilacji

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_calculateButton_clicked()
{
    // Tutaj później dodasz pobieranie danych i wywołanie calculateBandwidth
}