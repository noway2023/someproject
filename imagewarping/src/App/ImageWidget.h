#pragma once
#include <QWidget>
#include <QVector>

QT_BEGIN_NAMESPACE
class QImage;
class QPainter;
QT_END_NAMESPACE

enum wraptype {
	defalut = 0,
	idw = 1,
	rbf = 2,
};






class WrapPoint {
public:
	WrapPoint() {};
	~WrapPoint() {};
	void set_start(QPoint);
	void set_end(QPoint);
	QPoint get_start();
	QPoint get_end();
protected:
	QPoint start;
	QPoint end;
};

class ImageWidget :
	public QWidget
{
	Q_OBJECT

public:
	ImageWidget(void);
	~ImageWidget(void);
	
	wraptype type;
	void IDW(QRect rect);
	void RBF(QRect rect);
	QRgb interplate(QImage*, double i, double j);

	bool is_end;

protected:
	void paintEvent(QPaintEvent *paintevent);
	void mousePressEvent(QMouseEvent* event);
	void mouseMoveEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent* event);
	void mouseDoubleClickEvent(QMouseEvent* event);

public slots:
	// File IO
	void Open();												// Open an image file, support ".bmp, .png, .jpg" format
	void Save();												// Save image to current file
	void SaveAs();												// Save image to another file

	// Image processing
	void Invert();												// Invert pixel value in image
	void Mirror(bool horizontal=false, bool vertical=true);		// Mirror image vertically or horizontally
	void TurnGray();											// Turn image to gray-scale map
	void Restore();												// Restore image to origin
	void Reback();
	
	void SetIDW();
	void SetRBF();
	void SetClean();

private:
	QImage		*ptr_image_;				// image 
	QImage		*ptr_image_backup_;
	QImage      * ptr_image_copy;
	QVector<QPoint> originlist, changedlist;
	WrapPoint* wrappoint;
	QPoint pointstart;
	QPoint pointend;
	int listnum;
};

