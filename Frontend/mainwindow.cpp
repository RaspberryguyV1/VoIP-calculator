#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "../Backend/BackendVoIP.h" 

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 1. Wypełnienie listy rozwijanej dostępnymi kodekami
    ui->codecComboBox->addItems({
        "g.711", "g.722", "Opus", "iLBC", "g.729", "g.728", "g.723.1 MPC-MLQ", "g.723.1 ACELP"
    });

    // 2. Ustawienie stanu początkowego dla sumy kontrolnej (wyłączona i odznaczona)
    ui->controlSumCheckBox->setEnabled(false);
    ui->controlSumCheckBox->setChecked(false);

    // 3. Automatyczne powiązanie: gdy klikniesz kompresję, włącz/wyłącz sumę kontrolną
    // i automatycznie ją odznacz, jeśli kompresja zostaje wyłączona.
    connect(ui->compressedCheckBox, &QCheckBox::toggled, this, [this](bool checked) {
        ui->controlSumCheckBox->setEnabled(checked);
        if (!checked) {
            ui->controlSumCheckBox->setChecked(false);
        }
    });

    // 4. Nowoczesny, ciemny styl z obsługą wygaszonych checkboxów
    this->setStyleSheet(
        "QMainWindow { background-color: #1e1e1e; }"
        "QLabel { color: #d4d4d4; font-size: 14px; }"
        "QGroupBox { color: #4fc1ff; font-weight: bold; border: 1px solid #3c3c3c; margin-top: 15px; padding-top: 15px; border-radius: 5px; }"
        "QGroupBox::title { subcontrol-origin: margin; subcontrol-position: top left; left: 10px; padding: 0 5px; }"
        "QComboBox, QSpinBox { background-color: #2d2d2d; color: #ffffff; border: 1px solid #444444; padding: 6px; font-size: 14px; border-radius: 4px; }"
        "QCheckBox { color: #d4d4d4; font-size: 14px; spacing: 8px; margin: 4px 0px; }"
        "QCheckBox:disabled { color: #555555; }"
        "QPushButton { background-color: #0e639c; color: white; font-weight: bold; font-size: 14px; padding: 10px; border-radius: 5px; border: none; }"
        "QPushButton:hover { background-color: #1177bb; }"
        "QPushButton:pressed { background-color: #094771; }"
        "#wynikLabel { color: #2ecc71; font-size: 22px; font-weight: bold; margin-top: 10px; }"
    );
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_calculateButton_clicked()
{
    // 1. Zczytanie wybranego kodeka i dopasowanie do nowych nazw
    QString wybranyTekst = ui->codecComboBox->currentText();
    CodecType wybranyKodek = g711; 
    
    if (wybranyTekst == "g.711") wybranyKodek = g711;
    else if (wybranyTekst == "g.722") wybranyKodek = g722;
    else if (wybranyTekst == "Opus") wybranyKodek = opus;
    else if (wybranyTekst == "iLBC") wybranyKodek = ilbc;
    else if (wybranyTekst == "g.729") wybranyKodek = g729;
    else if (wybranyTekst == "g.728") wybranyKodek = g728;
    else if (wybranyTekst == "g.723.1 MPC-MLQ") wybranyKodek = g7231MPCMLQ;
    else if (wybranyTekst == "g.723.1 ACELP") wybranyKodek = g7231ACELP;

    // 2. Zczytanie wpisanego ptime i automatyczna weryfikacja/korekta
    int wpisanyPtime = ui->ptimeSpinBox->value();
    int poprawnyPtime = validatePtime(wybranyKodek, wpisanyPtime);

    if (wpisanyPtime != poprawnyPtime) {
        ui->ptimeSpinBox->setValue(poprawnyPtime);
    }

    // 3. Zczytanie stanu opcji (Checkboxów)
    bool isCompressed = ui->compressedCheckBox->isChecked();
    bool hasControlSum = ui->controlSumCheckBox->isChecked();
    bool isVlan = ui->vlanCheckBox->isChecked();
    bool isVad = ui->vadCheckBox->isChecked();

    // 4. Wywołanie backendu
    float totalBandwidth = calculateBandwidth(wybranyKodek, poprawnyPtime, isCompressed, hasControlSum, isVlan, isVad);

    // 5. Wyświetlenie wyniku
    ui->wynikLabel->setText("Wynik: " + QString::number(totalBandwidth, 'f', 2) + " kbps");
}