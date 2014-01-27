#include <QPainter>
#include <QImage>
#include <QStaticText>
#include "previewdisplay.h"
#include <opencv2/core/core_c.h>

#define BUF_SIZE (640 * 480 * 3)

PreviewDisplay::PreviewDisplay(QWidget *parent) :
	QWidget(parent),
	frame_source(0),
	rgb_image(0),
	rotate_angle(0),
	x_scale(1),
	y_scale(1),
	zoom(1)
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

	const int w = width();
	const int h = height();
	const int wz = w * zoom;
	const int hz = h * zoom;
	const int radius = (w > h ? h : w) / 2 - 50;
	QColor transparent(0, 0, 0, 0);
	QBrush brush(transparent);
	QPainter painter(this);
	QImage image((const uchar *)rgb_image->data.ptr,
				 rgb_image->width,
				 rgb_image->height,
				 QImage::Format_RGB888);

	image = image.scaled(wz, hz, Qt::KeepAspectRatio);
	painter.translate(w / 2, h / 2);
	painter.fillRect(-w / 2, -h / 2, w, h, Qt::black);
	painter.setCompositionMode(QPainter::CompositionMode_DestinationIn);
	painter.setBrush(brush);
	painter.drawEllipse(-radius, -radius, radius * 2, radius * 2);
	painter.setCompositionMode(QPainter::CompositionMode_DestinationOver);
	drawCoordinates(painter);
	painter.scale(x_scale, y_scale);
	painter.rotate(rotate_angle);
	painter.drawImage(-wz / 2, -hz / 2, image);
	painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
}

void PreviewDisplay::drawCoordinates(QPainter &painter)
{
	const int w = width();
	const int half_w = w / 2;
	const int h = height();
	const int half_h = h / 2;
	const int ratio = 10;
	const int range_y = half_h / ratio;
	const int range_x = half_w / ratio;
	const int len = 5;
	const int len5 = len + 5;

	// draw coordinates
	painter.setPen(Qt::green);
	// We draw X, Y axes in a transformed coordinate system.
	painter.drawLine(0, half_h, 0, -half_h); // Y-axis
	painter.drawLine(-half_w, 0, half_w, 0); // X-axis
	painter.drawStaticText(-10, -half_h + 10, QStaticText(QString("y")));
	painter.drawStaticText(half_w - 10, 10, QStaticText(QString("x")));

	for (int i = range_y; i > 0; i--) {
		if ((i % 5) == 0) {
			painter.drawLine(0, -i * ratio, len5, -i * ratio);
			painter.drawStaticText(len5, -i * ratio, QStaticText(QString::number(i)));
		} else {
			painter.drawLine(0, -i * ratio, len, -i * ratio);
		}
	}

	for (int i = range_y; i > 0; i--) {
		if ((i % 5) == 0) {
			painter.drawLine(0, i * ratio, len5, i * ratio);
			painter.drawStaticText(len5, i * ratio, QStaticText(QString::number(-i)));
		} else {
			painter.drawLine(0, i * ratio, len, i * ratio);
		}
	}

	for (int i = range_x; i > 0; i--) {
		if ((i % 5) == 0) {
			painter.drawLine(i * ratio, 0, i * ratio, -len5);
			painter.drawStaticText(ratio * i, 0, QStaticText(QString::number(i)));
		} else {
			painter.drawLine(i * ratio, 0, i * ratio, -len);
		}
	}

	for (int i = range_x; i > 0; i--) {
		if ((i % 5) == 0) {
			painter.drawLine(-i * ratio, 0, -i * ratio, -len5);
			painter.drawStaticText(ratio * (-i), 0, QStaticText(QString::number(-i)));
		} else {
			painter.drawLine(-i * ratio, 0, -i * ratio, -len);
		}
	}

	painter.drawStaticText(5, 0, QStaticText("0"));
}

void PreviewDisplay::rotate(int angle)
{
	rotate_angle = angle;
}
void PreviewDisplay::scale_x(qreal xs)
{
	x_scale = xs;
}

void PreviewDisplay::scale_y(qreal ys)
{
	y_scale = ys;
}

void PreviewDisplay::zoom_image(qreal z)
{
	zoom = z;
}
