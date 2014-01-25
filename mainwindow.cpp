#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <opencv2/highgui/highgui_c.h>
#include <opencv2/imgproc/imgproc_c.h>

#define FPS 25
#define WIDTH 640
#define HEIGHT 480

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow),
	preview_timer(new QTimer),
	capture(0)
{
	ui->setupUi(this);

	capture = cvCreateCameraCapture(CV_CAP_ANY);
	if (!capture) {
		LOGE("cvCreateCameraCapture() returned NULL");
		return;
	}
	cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH, WIDTH);
	cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT, HEIGHT);
	ui->preview->set_frame_property(HEIGHT, WIDTH, CV_8UC3);

	connect(preview_timer, SIGNAL(timeout()), this, SLOT(update_preview()));
	preview_timer->setInterval(1000 / FPS);
	preview_timer->start();

}

bool MainWindow::init_ok()
{
	if (!capture) {
		return false;
	}
	return true;
}

MainWindow::~MainWindow()
{
	delete ui;
	delete preview_timer;

	if (capture) {
		cvReleaseCapture(&capture);
	}
}

void MainWindow::update_preview()
{
	const IplImage *f = NULL;

	f = cvQueryFrame(capture);
	if (f == NULL) {
		LOGW("cvQueryFrame() returned NULL");
		return;
	}

	ui->preview->set_frame_source(f);
	ui->preview->update();
}
