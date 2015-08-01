#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0);

protected:
	void closeEvent(QCloseEvent *e);
};

#endif // MAINWINDOW_H
