#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <opencv2/highgui/highgui_c.h>
#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/core/core_c.h>
#include <stdlib.h>
#include <QDateTime>
#include <QMessageBox>

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

	connect(ui->slider_rotate, SIGNAL(valueChanged(int)), this, SLOT(rotate(int)));
	connect(ui->slider_x_scale, SIGNAL(valueChanged(int)), this, SLOT(scale_x(int)));
	connect(ui->slider_y_scale, SIGNAL(valueChanged(int)), this, SLOT(scale_y(int)));
	connect(ui->pb_take_shot, SIGNAL(clicked()), this, SLOT(take_shot()));
	connect(ui->pb_grab_preview, SIGNAL(clicked()), this, SLOT(grab_preview()));
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

static QString save_file_ok("Saved Image File");
static QString save_file_fail("Save Image File Failed");

void MainWindow::take_shot()
{
	const CvMat *frame = NULL;

	frame = ui->preview->get_frame_onscreen();
	if (!frame) {
		LOGE("Failed to get onscreen frame.");
		return;
	}

	QImage image((const uchar *)frame->data.ptr,
				 frame->width,
				 frame->height,
				 QImage::Format_RGB888);
	QString filename;
	bool status;

	filename = get_filename();
	if (filename.length() == 0) {
		QMessageBox::warning(this, save_file_fail, "Failed to get a valid file path.");
		return;
	}

	status = image.save(filename);
	if (status) {
		QMessageBox::information(this,
								 save_file_ok,
								 "Successfully saved image to " + filename + ".");
	}
}

void MainWindow::grab_preview()
{
	QString filename;
	QImage image(ui->preview->size(), QImage::Format_RGB888);
	bool status;

	filename = get_filename();
	if (filename.length() == 0) {
		QMessageBox::warning(this, save_file_fail, "Failed to get a valid file path.");
		return;
	}

	ui->preview->render(&image);
	status = image.save(filename);
	if (status) {
		QMessageBox::information(this,
								 save_file_ok,
								 "Successfully saved image to " + filename + ".");
	}
}

QString MainWindow::get_filename()
{
	char * home;
	QString datetime;
	QString home_stripped;
	QString filename;

	home = getenv("HOME");
	if (!home) {
		return QString("");
	}
	home_stripped = QString::fromAscii(home);
	if (home_stripped.endsWith('/')) {
		home_stripped.chop(1);
	}

	datetime = QDateTime::currentDateTime().toString("yyyy-MM-dd-hh-mm-ss");
	filename = home_stripped + "/qtcam-" + datetime + ".jpg";

	return filename;
}

void MainWindow::rotate(int angle)
{
	int rotate_angle = angle * 10 - 180;

	ui->label_rotate->setText(QString::number(rotate_angle));
	ui->preview->rotate(-rotate_angle);
}
void MainWindow::scale_x(int xs)
{
	qreal x_scale = xs / 10.0;

	ui->label_x_scale->setText(QString::number(x_scale));
	ui->preview->scale_x(x_scale);
}

void MainWindow::scale_y(int ys)
{
	qreal y_scale = ys / 10.0;
	ui->label_y_scale->setText(QString::number(y_scale));
	ui->preview->scale_y(y_scale);
}
