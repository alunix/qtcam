#include <QPainter>
#include <QImage>
#include "previewdisplay.h"
#include <opencv2/core/core_c.h>

#define BUF_SIZE (640 * 480 * 3)

PreviewDisplay::PreviewDisplay(QWidget *parent) :
	QWidget(parent),
	frame_source(0),
	rgb_image(0)
{
}

PreviewDisplay::~PreviewDisplay()
{
	if (rgb_image) {
		cvReleaseMat(&rgb_image);
	}
}

void * PreviewDisplay::set_frame_property(int h, int w, int type)
{
	if (rgb_image) {
		cvReleaseMat(&rgb_image);
		rgb_image = NULL;
	}
	if ((h < 1) || (w < 1)) {
		LOGD("Invalid height or width");
		return (void *)NULL;
	}
	rgb_image = cvCreateMat(h, w, type);

	return (void *)rgb_image;
}

void PreviewDisplay::paintEvent(QPaintEvent *)
{
	if ((!frame_source) || (!rgb_image)) {
		LOGW("frame_source is %p, rgb_image is %p", frame_source, rgb_image);
		return;
	}
	LOGD("w: %d, h: %d, ws: %d", frame_source->width, frame_source->height, frame_source->widthStep);

	cvCvtColor(frame_source, rgb_image, CV_BGR2RGB);

	QPainter painter(this);
	QImage image((const uchar *)rgb_image->data.ptr,
				 rgb_image->width,
				 rgb_image->height,
				 QImage::Format_RGB888);

	image = image.scaled(640, 480, Qt::KeepAspectRatio);
	painter.drawImage(0, 0, image);
}
