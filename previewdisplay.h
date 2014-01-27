#ifndef PREVIEWDISPLAY_H
#define PREVIEWDISPLAY_H

#include <QWidget>
#include <opencv2/highgui/highgui_c.h>
#include <opencv2/imgproc/imgproc_c.h>
#include "logutil.h"

class PreviewDisplay : public QWidget
{
	Q_OBJECT
public:
	explicit PreviewDisplay(QWidget *parent = 0);
	virtual ~PreviewDisplay();
	// Call it everytime the frame size and type are changed.
	void *set_frame_property(int h, int w, int type);
	void set_frame_source(const IplImage *fs) { frame_source = fs; }
	CvMat *get_frame_onscreen() { return rgb_image; }
	void paintEvent(QPaintEvent *);
	void rotate(int angle);
	void scale_x(qreal xs);
	void scale_y(qreal ys);
	void zoom_image(qreal z);

private:
	void drawCoordinates(QPainter &painter);

	const IplImage *frame_source;
	CvMat *rgb_image;
	int rotate_angle;
	qreal x_scale;
	qreal y_scale;
	qreal zoom;
};

#endif // PREVIEWDISPLAY_H
