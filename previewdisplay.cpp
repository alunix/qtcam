#include <QPainter>
#include <QImage>
#include "previewdisplay.h"
#include <opencv2/core/core_c.h>

#define BUF_SIZE (640 * 480 * 3)

PreviewDisplay::PreviewDisplay(QWidget *parent) :
	QWidget(parent),
	frame_source(0),
	swap(0)
{
}

PreviewDisplay::~PreviewDisplay()
{
	if (swap) {
		cvReleaseMat(&swap);
	}
}

void * PreviewDisplay::set_frame_property(int h, int w, int type)
{
	if (swap) {
		cvReleaseMat(&swap);
	}
	swap = cvCreateMat(h, w, type);
	return (void *)swap;
}

void PreviewDisplay::paintEvent(QPaintEvent *)
{
	if (!frame_source) {
		return;
	}
	//qDebug("w: %d, h: %d, ws: %d\n", frame_source->width, frame_source->height, frame_source->widthStep);

	cvCvtColor(frame_source, swap, CV_BGR2RGB);

	QPainter painter(this);
	QImage image((const uchar *)swap->data.ptr,
				 swap->width,
				 swap->height,
				 QImage::Format_RGB888);

	image = image.scaled(640, 480, Qt::KeepAspectRatio);
	painter.drawImage(0, 0, image);
}
