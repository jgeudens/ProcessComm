#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

// Forward declarations
class EchoClient;
class LoggerClient;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private:
    void connectEcho();
    void connectLogger();

    Ui::MainWindow* _ui;
    EchoClient* _echoClient = nullptr;
    LoggerClient* _loggerClient = nullptr;

    QStringList _availableDataPoints;
};
#endif // MAINWINDOW_H
