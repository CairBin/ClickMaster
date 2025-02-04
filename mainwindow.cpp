#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QRegularExpression>
#include <QMessageBox>
#include <QLineEdit>
#include <windows.h> // click mouse button
#include <QDebug>

/**
 * Verify str is a positive integer
 * @param str {const String&}  verified string
 * @return true if it is a positive integer; otherwise false
*/
bool isPositiveInteger(const QString& str) {
    QRegularExpression regex(R"(^0*[1-9]\d*$)");
    QRegularExpressionMatch match = regex.match(str);
    return match.hasMatch();
}

const QString MainWindow::DEFAULT_SHORTKEY_STR = "Ctrl+f6";

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // setting MainWindow UI
    ui->intervalEdit->setText("100");
    this->setFixedSize(300 ,472);
    this->setWindowTitle("ClickMaster");
    ui->startButton->setFocus();

    // binding shotcut key
    this->keyEdit = new CustomKeySequenceEdit(ui->groupBox_2);
    this->keyEdit->setGeometry(20, 50, 201, 41);
    this->shortcut = new QShortcut(QKeySequence::fromString(DEFAULT_SHORTKEY_STR), this);
    this->keyEdit->setKeySequence(QKeySequence::fromString(DEFAULT_SHORTKEY_STR));
    connect(this->keyEdit, &CustomKeySequenceEdit::editingFinished, this, &MainWindow::onShortcutChanged);
    connect(shortcut, &QShortcut::activated, this, &MainWindow::onStartButtonClicked);

    // setting mouse button selector and action selector
    ui->mouseButtonSelector->addItem("Left");
    ui->mouseButtonSelector->addItem("Middle");
    ui->mouseButtonSelector->addItem("Right");

    ui->mouseActionSelector->addItem("Single Click");
    ui->mouseActionSelector->addItem("Double Click");

    // Clicking interval
    this->mouseClickingTimer = new QTimer(this);
    this->mouseClickingTimer->setInterval(100); //default 100 ms
    connect(mouseClickingTimer, &QTimer::timeout, this, &MainWindow::onMouseClickingTimeout);

    // startButton
    connect(ui->startButton, &QPushButton::clicked, this, &MainWindow::onStartButtonClicked);
}

MainWindow::~MainWindow()
{
    if(this->shortcut){
        delete shortcut;
    }
    delete ui;
}

void MainWindow::onStartButtonClicked(){
    if(this->isRunning){
        // on timer running
        this->isRunning = false;
        ui->startButton->setText("Start");
        ui->intervalEdit->setEnabled(true);
        this->keyEdit->setEnabled(true);
        ui->mouseActionSelector->setEnabled(true);
        ui->mouseButtonSelector->setEnabled(true);
        // then stop timer
        mouseClickingTimer->stop();
        return;
    }

    // on timer not running
    QString msg;
    if(!this->checkParams(msg)){
        QMessageBox::critical(nullptr, "Error", msg);
        return;
    }
    ui->startButton->setText("Stop");
    ui->intervalEdit->setEnabled(false);
    this->keyEdit->setEnabled(false);
    ui->mouseActionSelector->setEnabled(false);
    ui->mouseButtonSelector->setEnabled(false);
    // setting interval and starting timer
    mouseClickingTimer->setInterval(ui->intervalEdit->text().toInt());
    this->isRunning = true; // don't forget changing state!!!
    mouseClickingTimer->start();

}

bool MainWindow::checkParams(QString& message) const {
    if(!isPositiveInteger(this->ui->intervalEdit->text())){
        message = "Interval must be positive integer!";
        return false;
    }

    return true;
}

void MainWindow::onMouseClickingTimeout(){
    /*
     * Pls refer to windows.h (Windows system)
     * Qt only supports handling mouse events in qt window
     * and you need to send other areas mouse event via local system interface.
     */
    POINT pos;
    GetCursorPos(&pos);

    QString currentActionText = ui->mouseActionSelector->currentText();
    QString currentButtonText = ui->mouseButtonSelector->currentText();

    qDebug() << "mouse action: " << currentActionText;
    qDebug() << "mouse button:" << currentButtonText;

    auto down = MOUSEEVENTF_LEFTDOWN;
    auto up = MOUSEEVENTF_LEFTUP;
    if(currentButtonText == "Middle"){
        down = MOUSEEVENTF_MIDDLEDOWN;
        up = MOUSEEVENTF_MIDDLEUP;
    }else if(currentButtonText == "Right"){
        down = MOUSEEVENTF_RIGHTDOWN;
        up = MOUSEEVENTF_RIGHTUP;
    }

    mouse_event(down, pos.x, pos.y, 0, 0);
    mouse_event(up, pos.x, pos.y, 0, 0);

    // if type is "Double Click", wait 100 ms and click again
    if(currentActionText == "Double Click"){
        Sleep(100);
        mouse_event(down, pos.x, pos.y, 0, 0);
        mouse_event(up, pos.x, pos.y, 0, 0);
    }

}

void MainWindow::onShortcutChanged(){
    QKeySequence seq = this->keyEdit->keySequence();
    qDebug() << "Get shotcut key sequence: " << seq;
    if(seq.isEmpty()){
        // if user inputs empty string
        seq = QKeySequence::fromString(DEFAULT_SHORTKEY_STR);
        this->keyEdit->setKeySequence(seq);
        qDebug() << "Set default sequence: " << seq;
    }

    if (shortcut){
        disconnect(shortcut, &QShortcut::activated, this, &MainWindow::onStartButtonClicked);
        delete shortcut;
        shortcut = nullptr;
    }

    this->shortcut = new QShortcut(seq, this);
    connect(shortcut, &QShortcut::activated, this, &MainWindow::onStartButtonClicked);
//    this->keyEdit->setKeySequence(seq);
}
