#include "ImageWidget.h"
#include <QImage>
#include <QPainter>
#include <QtWidgets> 
#include <iostream>
#include <Eigen/Dense>
#include <cmath>
#include <unsupported/Eigen/CXX11/Tensor>

#define EPSILON 1.0e-10	// zero double

using namespace Eigen;
using namespace Eigen::internal;
using namespace Eigen::Architecture;
using std::cout;
using std::endl;

bool operator<(QPoint& other1, QPoint& other2) {
	return other1.x() < other2.x();
}


ImageWidget::ImageWidget(void)
{
	ptr_image_ = new QImage();
	ptr_image_backup_ = new QImage();
	ptr_image_copy = new QImage();
	type = rbf;
	is_end = false;
	wrappoint = NULL;
	listnum = 0;
}


ImageWidget::~ImageWidget(void)
{
	originlist.clear(); 
	changedlist.clear();
}

void ImageWidget::paintEvent(QPaintEvent *paintevent)
{
	QPainter painter;
	painter.begin(this);

	// Draw background
	painter.setBrush(Qt::lightGray);
	QRect back_rect(0, 0, width(), height());
	painter.drawRect(back_rect);

	// Draw image
	QRect rect = QRect( (width()-ptr_image_->width())/2, (height()-ptr_image_->height())/2, ptr_image_->width(), ptr_image_->height());
	if (is_end) {
		switch (type){
		case defalut:
			break;
		case idw:
			//*(ptr_image_) = *(ptr_image_backup_);
			IDW(rect);
			*(ptr_image_) = *(ptr_image_copy);
			break;
		case rbf:
			RBF(rect);
			*(ptr_image_) = *(ptr_image_copy);
			break;
		}
		
	}
	painter.drawImage(rect, *ptr_image_); 

	for (int i = 0; i < originlist.size(); i++) {
		QPen mypen(Qt::blue);
		mypen.setWidth(5);
		painter.setPen(mypen);
		painter.drawPoint(originlist[i]);
		painter.drawPoint(changedlist[i]);
		painter.setPen(QPen(Qt::red));
		painter.drawLine(originlist[i], changedlist[i]);
	}

	update();
	painter.end();
}

void ImageWidget::Open()
{
	// Open file
	QString fileName = QFileDialog::getOpenFileName(this, tr("Read Image"), ".", tr("Images(*.bmp *.png *.jpg)"));

	// Load file
	if (!fileName.isEmpty())
	{
		ptr_image_->load(fileName);
		*(ptr_image_backup_) = *(ptr_image_);
		*(ptr_image_copy) = *(ptr_image_);
	}

	//ptr_image_->invertPixels(QImage::InvertRgb);
	//*(ptr_image_) = ptr_image_->mirrored(true, true);
	//*(ptr_image_) = ptr_image_->rgbSwapped();
	cout<<"image size: "<<ptr_image_->width()<<' '<<ptr_image_->height()<<endl;
	update();
}

void ImageWidget::Save()
{
	SaveAs();
}

void ImageWidget::SaveAs()
{
	QString filename = QFileDialog::getSaveFileName(this, tr("Save Image"), ".", tr("Images(*.bmp *.png *.jpg)"));
	if (filename.isNull())
	{
		return;
	}	

	ptr_image_->save(filename);
}

void ImageWidget::Invert()
{
	for (int i=0; i<ptr_image_->width(); i++)
	{
		for (int j=0; j<ptr_image_->height(); j++)
		{
			QRgb color = ptr_image_->pixel(i, j);
			ptr_image_->setPixel(i, j, qRgb(255-qRed(color), 255-qGreen(color), 255-qBlue(color)) );
		}
	}

	// equivalent member function of class QImage
	// ptr_image_->invertPixels(QImage::InvertRgb);
	update();
}

void ImageWidget::Mirror(bool ishorizontal, bool isvertical)
{
	QImage image_tmp(*(ptr_image_));
	int width = ptr_image_->width();
	int height = ptr_image_->height();

	if (ishorizontal)
	{
		if (isvertical)
		{
			for (int i=0; i<width; i++)
			{
				for (int j=0; j<height; j++)
				{
					ptr_image_->setPixel(i, j, image_tmp.pixel(width-1-i, height-1-j));
				}
			}
		} 
		else			//仅水平翻转			
		{
			for (int i=0; i<width; i++)
			{
				for (int j=0; j<height; j++)
				{
					ptr_image_->setPixel(i, j, image_tmp.pixel(width-1-i, j));
				}
			}
		}
		
	}
	else
	{
		if (isvertical)		//仅垂直翻转
		{
			for (int i=0; i<width; i++)
			{
				for (int j=0; j<height; j++)
				{
					ptr_image_->setPixel(i, j, image_tmp.pixel(i, height-1-j));
				}
			}
		}
	}

	// equivalent member function of class QImage
	//*(ptr_image_) = ptr_image_->mirrored(true, true);
	update();
}

void ImageWidget::TurnGray()
{
	for (int i=0; i<ptr_image_->width(); i++)
	{
		for (int j=0; j<ptr_image_->height(); j++)
		{
			QRgb color = ptr_image_->pixel(i, j);
			int gray_value = (qRed(color)+qGreen(color)+qBlue(color))/3;
			ptr_image_->setPixel(i, j, qRgb(gray_value, gray_value, gray_value) );
		}
	}

	update();
}

void ImageWidget::Restore()
{
	*(ptr_image_) = *(ptr_image_backup_);
	is_end = false;
	update();
}

void ImageWidget::Reback()
{
	*(ptr_image_) = *(ptr_image_copy);
	is_end = false;
	update();
}

void ImageWidget::SetIDW() {
	type = idw;
	is_end = false;

}

void ImageWidget::SetClean() {
	is_end = false;
	originlist.clear();
	changedlist.clear();
}

void ImageWidget::SetRBF() {
	type = rbf;
	is_end = false;
}


double distance2(QPoint a, QPoint b) {
	double dx = a.x() - b.x();
	double dy = a.y() - b.y();
	return pow(dx * dx + dy * dy, 1);
}

int distance3(QPoint a, QPoint b) {
	int dx = a.x() - b.x();
	int dy = a.y() - b.y();
	return abs(dx-dy);
}

double distance1(QPoint a, QPoint b) {
	double dx = a.x() - b.x();
	double dy = a.y() - b.y();
	return pow(dx * dx + dy * dy, 0.5);
}

int distance(int a, int b) {
	return abs(a-b);
}

void ImageWidget::IDW(QRect rect) {
	int n = originlist.size();
	for (int i = 0; i < n; i++) {
		originlist[i].setX(originlist[i].x() - rect.topLeft().x());
		originlist[i].setY(originlist[i].y() - rect.topLeft().y());
		changedlist[i].setX(changedlist[i].x() - rect.topLeft().x());
		changedlist[i].setY(changedlist[i].y() - rect.topLeft().y());
	}
	int width = ptr_image_->width();
	int hidth = ptr_image_->height();
	//Tensor<double, 3> w(width, hidth, n);
	//w.setZero();
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < hidth; j++) {
			double sumw = 0.0;
			double newx = 0.0, newy = 0.0;
			double v,w;
			QRgb color;
			for (int k = 0; k < n; k++) {
				v = 1.0 / distance1(QPoint(i, j), changedlist[k]);
				sumw += v;
				if (i == changedlist[k].x() && j == changedlist[k].y()) {
					w = 1.0;
					ptr_image_copy->setPixel(changedlist[k].x(), changedlist[k].y(), 
						ptr_image_->pixel(originlist[k].x(), originlist[k].y()));
					continue;
				}
				else {
					w = v;
				}
				newx += w * (originlist[k].x() + i - changedlist[k].x());
				newy += w * (originlist[k].y() + j - changedlist[k].y());
			}
			w = 1. / sumw;
			newx = newx * w;
			newy = newy * w;


			//for (int k = 0; k < n; k++) {
				//w(i, j, k) /= sumw;
				//double dx = 0.0, dy = 0.0;
				//dx = 2.0 * (i - originlist[k].x()); //+ 1.0 * (j - originlist[k].y());
				//dy = 2.0 * (j - originlist[k].y()); // +1.0 * (i - originlist[k].x());
				//dx = distance2(QPoint(i, j), originlist[k]);
				//dy = distance2(QPoint(i, j), originlist[k]);
				//newx += w(i, j, k) * (originlist[k].x() + dx);
				//newy += w(i, j, k) * (originlist[k].y() + dy);
				//cout << w(i, j, k) <<"   " << changedlist[k].x() <<"   " << newx << endl;
			//}

			//if (int(newx) >= width  || int(newx) < 0 || int(newy) >= hidth  || int(newy) < 0) {
			//	color = qRgb(127, 127, 127);
			//}
			//else {
			//	color = ptr_image_->pixel(int(newx), int(newy));
			//}

			if (int(newx) >= width  || int(newx) < 0 || int(newy) >= hidth || int(newy) < 0) {
				color = qRgb(127, 127, 127);
			}
			else if (int(newx) == width -1|| int(newx) == 0 || int(newy) == hidth -1 || int(newy) == 0) {
				color = ptr_image_->pixel(int(newx), int(newy));
			}
			else{
				color = interplate(ptr_image_, newx, newy);
			}

			ptr_image_copy->setPixel(i, j, color);
			//cout << i << "   " << j << "   " << int(newx) << "   " << int(newy) << endl;
		}
	}

	for (int i = 0; i < n; i++) {
		originlist[i].setX(originlist[i].x() + rect.topLeft().x());
		originlist[i].setY(originlist[i].y() + rect.topLeft().y());
		changedlist[i].setX(changedlist[i].x() + rect.topLeft().x());
		changedlist[i].setY(changedlist[i].y() + rect.topLeft().y());
	}
}


double RadialFunc(double x) {
	double r = 10.;
	double u = -1.;
	return pow(x * x + r * r, u / 2);
}
void ImageWidget::RBF(QRect rect) {
	int n = originlist.size();
	for (int i = 0; i < n; i++) {
		originlist[i].setX(originlist[i].x() - rect.topLeft().x());
		originlist[i].setY(originlist[i].y() - rect.topLeft().y());
		changedlist[i].setX(changedlist[i].x() - rect.topLeft().x());
		changedlist[i].setY(changedlist[i].y() - rect.topLeft().y());
	}
	int width = ptr_image_->width();
	int hidth = ptr_image_->height();
	MatrixXf A(n + 3, n + 3), C(n + 3, 2);
	Matrix<float, Dynamic, 2> a(n, 2);
	Matrix<float, 3, 2> v;
	Matrix<float, Dynamic, 2> b(n, 2);
	Matrix<float, Dynamic, Dynamic> g(n, n);
	Matrix<float, Dynamic, 3> h(n, 3);
	Matrix<float, 3, 2> z;
	z.setZero();
	Matrix<float, Dynamic, 2> result(n + 3, 2);
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			g(i, j) = RadialFunc(distance1(changedlist[i], changedlist[j]));
		}
		b(i, 0) = originlist[i].x();
		b(i, 1) = originlist[i].y();
		h(i, 0) = changedlist[i].x();
		h(i, 1) = changedlist[i].y();
		h(i, 2) = 1;
	}
	A << g, h, h.transpose(), MatrixXf::Zero(3, 3);
	C << b, z;
	result = A.fullPivLu().solve(C);
	a = result.topRows(n).reshaped(n, 2);
	v = result.bottomRows(3).reshaped(3, 2);
	//for (int i = 0; i < 3; i++) {
	//	for (int j = 0; j < 2; j++) {
	//		cout << v(i, j) << "   ";
	//	}
	//	cout << endl;
	//}
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < hidth; j++) {
			QRgb color;
			float newx = 0.0, newy = 0.0;
			MatrixXf M(n, 1), N(3, 1), New(2, 1);
			N(0, 0) = i;
			N(1, 0) = j;
			N(2, 0) = 1;
			for (int k = 0; k < n; k++) {
				M(k,0) = RadialFunc(distance1(QPoint(i, j), changedlist[k]));
			}
			New = a.transpose() * M + v.transpose() * N;
			newx = New(0, 0);
			newy = New(1, 0);

			
			//if (int(newx) >= width  || int(newx) < 0 || int(newy) >= hidth  || int(newy) < 0) {
			//	color = qRgb(127, 127, 127);
			//}
			//else {
			//	color = ptr_image_->pixel(int(newx), int(newy));
			//}

			if (int(newx) >= width || int(newx) < 0 || int(newy) >= hidth || int(newy) < 0) {
				color = qRgb(127, 127, 127);
			}
			else if (int(newx) == width - 1 || int(newx) == 0 || int(newy) == hidth - 1 || int(newy) == 0) {
				color = ptr_image_->pixel(int(newx), int(newy));
			}
			else {
				color = interplate(ptr_image_, newx, newy);
			}
			ptr_image_copy->setPixel(i, j, color);

		
		}
	}


	for (int i = 0; i < n; i++) {
		originlist[i].setX(originlist[i].x() + rect.topLeft().x());
		originlist[i].setY(originlist[i].y() + rect.topLeft().y());
		changedlist[i].setX(changedlist[i].x() + rect.topLeft().x());
		changedlist[i].setY(changedlist[i].y() + rect.topLeft().y());
	}
}

QRgb ImageWidget::interplate(QImage* img, double i, double j) {
	int lowx, lowy, highx, highy;
	lowx = std::floor(i);
	highx = std::floor(i+0.5);
	lowy = std::floor(j);
	highy = std::floor(j+0.5);
	if (lowx < 0) lowx = 0;
	if (highx > img->width() - 1) highx = img->width() - 1;
	if (lowy < 0) lowy = 0;
	if (highy > img->height() - 1) highy = img->height() - 1;


	QRgb color0, color1, color2, color3;
	color0 = img->pixel(lowx, lowy);
	color1 = img->pixel(highx, lowy);
	color2 = img->pixel(lowx, highy);
	color3 = img->pixel(highx, highy);
	double w0, w1, w2, w3;
	w0 = (highx - i) * (highy - j);
	w1 = (1- highx + i) * (highy - j);
	w2 = (highx - i) * (1 - highy + j);
	w3 = (1 - highx + i) * (1 - highy + j);
	double r, b, g;
	r = w0 * qRed(color0) + w1 * qRed(color1) + w2 * qRed(color2) + w3 * qRed(color3);
	g = w0 * qGreen(color0) + w1 * qGreen(color1) + w2 * qGreen(color2) + w3 * qGreen(color3);
	b = w0 * qBlue(color0) + w1 * qBlue(color1) + w2 * qBlue(color2) + w3 * qBlue(color3);
	return QColor(int(r), int(g), int(b)).rgb();
}



void ImageWidget::mousePressEvent(QMouseEvent* event) {
	if (Qt::LeftButton == event->button()) {
		wrappoint = new WrapPoint();
		if (wrappoint != NULL ) {
			pointstart = pointend = event->pos();
			wrappoint->set_start(pointstart);
			wrappoint->set_end(pointend);
		}
	}
	update();
}

void ImageWidget::mouseMoveEvent(QMouseEvent* event) {
	if (wrappoint != NULL) {
		pointend = event->pos();
		wrappoint->set_end(pointend);
	}
}

void ImageWidget::mouseReleaseEvent(QMouseEvent* event) {
	if (!is_end) {
		originlist.push_back(pointstart);
		changedlist.push_back(pointend);
	}
	is_end = false;
}

void ImageWidget::mouseDoubleClickEvent(QMouseEvent* event) {
	originlist.pop_back();
	changedlist.pop_back();
	listnum = originlist.size();

	is_end = true;
	update();
}

void WrapPoint::set_start(QPoint s) {
	start = s;
}

void WrapPoint::set_end(QPoint e) {
	end = e;
}

QPoint WrapPoint::get_start() {
	return start;
}

QPoint WrapPoint::get_end() {
	return end;
}
