#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeySequence>
#include <QTimer>
#include <QShortcut>
#include "customkeysequenceedit.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QTimer* mouseClickingTimer;     // clicking timer
    bool isRunning = false;         // current state
    QShortcut* shortcut = nullptr;  // current shortcut sequence
    CustomKeySequenceEdit* keyEdit; // QKeySequenceEdit which only allow one sequence

    static const QString DEFAULT_SHORTKEY_STR;  // default shortcut key Ctrl+f6

private:
    /**
     * Verify interval and other params are valid and return error message.
     * @param message {QString&} error message
     * @return true if success; otherwise false
    */
    bool checkParams(QString& message) const;

private slots:
    /**
     * The slot function corresponding to the clicked event of the startButton.
    */
    void onStartButtonClicked();
    /**
     * The slot function corresponding to the timer.
    */
    void onMouseClickingTimeout();

    /**
     * The slot function corresponding to the keyEdit
    */
    void onShortcutChanged();
};
#endif // MAINWINDOW_H
