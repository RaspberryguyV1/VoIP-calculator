#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "../Backend/BackendVoIP.h" 
#include <QLineEdit> // Wymagane, by dostać się do ukrytego pola

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // ==========================================
    // 1. NAPRAWA WIZUALNA: Legalne obejście "protected"
    // ==========================================
    // Znajdujemy wbudowane pole tekstowe wewnątrz SpinBoxa i wymuszamy na nim rozmiar
    QLineEdit *spinBoxLineEdit = ui->ptimeSpinBox->findChild<QLineEdit*>();
    if (spinBoxLineEdit) {
        spinBoxLineEdit->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        spinBoxLineEdit->setMinimumWidth(60); // <-- To bezwzględnie wymusza wyrysowanie tekstu!
    }

    // Tekst + przypisany do niego kodek
    ui->codecComboBox->addItem("G.711 (64 kb/s)",              static_cast<int>(g711));
    ui->codecComboBox->addItem("G.722 (64 kb/s)",              static_cast<int>(g722));
    ui->codecComboBox->addItem("Opus (32 kb/s)",               static_cast<int>(opus));
    ui->codecComboBox->addItem("iLBC (15,2 kb/s)",             static_cast<int>(ilbc));
    ui->codecComboBox->addItem("G.729 (8 kb/s)",               static_cast<int>(g729));
    ui->codecComboBox->addItem("G.728 (16 kb/s)",              static_cast<int>(g728));
    ui->codecComboBox->addItem("G.723.1 MPC-MLQ (6,3 kb/s)",   static_cast<int>(g7231MPCMLQ));
    ui->codecComboBox->addItem("G.723.1 ACELP (5,3 kb/s)",     static_cast<int>(g7231ACELP));

    ui->controlSumCheckBox->setEnabled(false);
    ui->controlSumCheckBox->setChecked(false);

    connect(ui->compressedCheckBox, &QCheckBox::toggled, this, [this](bool checked) {
        ui->controlSumCheckBox->setEnabled(checked);
        if (!checked) {
            ui->controlSumCheckBox->setChecked(false);
        }
    });

    // ==========================================
    // 2. ŻELAZNA WALIDACJA "W LOCIE"
    // ==========================================
    connect(ui->ptimeSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, [this](int value) {
        CodecType wybranyKodek = static_cast<CodecType>(ui->codecComboBox->currentData().toInt());
        int poprawnyPtime = validatePtime(wybranyKodek, value);
        
        if (value != poprawnyPtime) {
            ui->ptimeSpinBox->blockSignals(true); 
            ui->ptimeSpinBox->setValue(poprawnyPtime);
            ui->ptimeSpinBox->blockSignals(false);
        }
    });

    // ==========================================
    // 3. DOSTOSOWANIE KROKU DO KODEKA
    // ==========================================
    connect(ui->codecComboBox, &QComboBox::currentIndexChanged, this, [this]() {
        CodecType wybranyKodek = static_cast<CodecType>(ui->codecComboBox->currentData().toInt());
        
        if (wybranyKodek == g7231MPCMLQ || wybranyKodek == g7231ACELP) {
            ui->ptimeSpinBox->setSingleStep(30);
            ui->ptimeSpinBox->setValue(30);
        } else {
            ui->ptimeSpinBox->setSingleStep(10);
            ui->ptimeSpinBox->setValue(20);
        }
    });

    // ==========================================
    // 4. PODŁĄCZENIE TWOICH STRZAŁEK
    // ==========================================
    ui->ptimeUp->disconnect();
    ui->ptimeDown->disconnect();
    
    connect(ui->ptimeUp, &QToolButton::clicked, ui->ptimeSpinBox, &QSpinBox::stepUp);
    connect(ui->ptimeDown, &QToolButton::clicked, ui->ptimeSpinBox, &QSpinBox::stepDown);

    // Wymuszenie załadowania ustawień startowych
    emit ui->codecComboBox->currentIndexChanged(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_calculateButton_clicked()
{
    CodecType wybranyKodek = static_cast<CodecType>(ui->codecComboBox->currentData().toInt());
    int wpisanyPtime = ui->ptimeSpinBox->value();
    
    int poprawnyPtime = validatePtime(wybranyKodek, wpisanyPtime);
    ui->ptimeSpinBox->setValue(poprawnyPtime);

    bool isCompressed = ui->compressedCheckBox->isChecked();
    bool hasControlSum = ui->controlSumCheckBox->isChecked();
    bool isVlan = ui->vlanCheckBox->isChecked();
    bool isVad = ui->vadCheckBox->isChecked();

    float totalBandwidth = calculateBandwidth(wybranyKodek, poprawnyPtime, isCompressed, hasControlSum, isVlan, isVad);

    QString wynik = QString::number(totalBandwidth, 'f', 2).replace('.', ',');
    ui->wynikLabel->setText(wynik + " kbps");
}