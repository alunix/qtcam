#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QImage>
#include <opencv2/highgui/highgui_c.h>
#include "logutil.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	virtual ~MainWindow();
	virtual bool init_ok();

protected slots:
	void update_preview();

private:
	Ui::MainWindow *ui;
	QTimer *preview_timer; // for freshing preview.
	CvCapture *capture;
};

#endif // MAINWINDOW_H
