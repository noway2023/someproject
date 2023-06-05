#include "Curve.h"
#include "Eigen/Dense"
#include <iostream>

using namespace Eigen;
using namespace Eigen::internal;
using namespace Eigen::Architecture;


Curve::Curve() {
}

Curve::~Curve() {
}

void Curve::Draw(QPainter& painter) {
	if (controlpoints.size() <= 1) return;
	painter.setPen(pen);

	if (controlpoints.size() == 2) {
		painter.drawLine(controlpoints[0], controlpoints[1]);
		return;
	}
	
	pointlist.clear();
	int pointnum = 100;

	// //cubic hermite ssplines
	// 
	//MatrixXf A(pointnum, 4);
	//MatrixXf B1(4, 1), B2(4, 1);
	//QVector<double> tlist;
	//double t, h0, h1, h2, h3;
	//for (int i = 0; i < pointnum; i++) {
	//	t = 1.0 / (pointnum - 1) * i;
	//	tlist.push_back(t);
	//	h0 = 2 * t * t * t - 3 * t * t + 1;
	//	h1 = -2 * t * t * t + 3 * t * t;
	//	h2 = t * t * t - 2 * t * t + t;
	//	h3 = t * t * t - t * t;
	//	A(i, 0) = h0;
	//	A(i, 1) = h1;
	//	A(i, 2) = h2;
	//	A(i, 3) = h3;
	//}
	//double y0, y1, y2, m1, m2, m0, x0, x1, x2;
	//x0 = controlpoints[0].x();
	//x1 = controlpoints[2].x();
	//x2 = controlpoints[1].x();
	//y0 = controlpoints[0].y();
	//y1 = controlpoints[2].y();
	//y2 = controlpoints[1].y();
	//m0 = 0.5 * (y1 - y0) / (x1 - x0);
	//m1 = 0.5 * (y2 - y0) / (x2 - x0);
	//m2 = 0.5 * (y2 - y1) / (x2 - x1);
	//B1(0, 0) = y0;
	//B1(1, 0) = y1;
	//B1(2, 0) = m0;
	//B1(3, 0) = m1;
	//B2(0, 0) = y1;
	//B2(1, 0) = y2;
	//B2(2, 0) = m1;
	//B2(3, 0) = m2;
	//MatrixXf product1 = A * B1;
	//MatrixXf product2 = A * B2;
	//

	//for (int i = 0; i < 2* pointnum; i++) {
	//	if (i < pointnum) {
	//		pointlist.push_back(QPoint(1.0 / (pointnum - 1) * i * (x1 - x0) + x0, product1(i, 0)));
	//	}
	//	else {
	//		pointlist.push_back(QPoint(1.0 / (pointnum - 1)* (i - pointnum) * (x2 - x1) + x1, product2(i - pointnum, 0)));
	//	}
	//}


	// seconed bezier curve
	MatrixXf A(pointnum, 3);
	MatrixXf B1(3, 1), B2(3, 1);
	double t, h0, h1, h2, h3;
	for (int i = 0; i < pointnum; i++) {
		t = 1.0 / (pointnum - 1) * i;
		h0 = (1 - t) * (1 - t);
		h1 = 2 * t * (1 - t);
		h2 = t * t;
		A(i, 0) = h0;
		A(i, 1) = h1;
		A(i, 2) = h2;
	}
	double y0, y1, y2, m1, m2, m0, x0, x1, x2;
	x0 = controlpoints[0].x();
	x1 = controlpoints[2].x();
	x2 = controlpoints[1].x();
	y0 = controlpoints[0].y();
	y1 = controlpoints[2].y();
	y2 = controlpoints[1].y();
	B1(0, 0) = x0;
	B1(1, 0) = x1;
	B1(2, 0) = x2;
	B2(0, 0) = y0;
	B2(1, 0) = y1;
	B2(2, 0) = y2;
	MatrixXf product1 = A * B1;
	MatrixXf product2 = A * B2;
	for (size_t i = 0; i < pointnum; i++) {
		pointlist.push_back(QPoint(product1(i, 0), product2(i, 0)));
	}

	for (int i = 1; i < pointlist.size(); i++) {
		painter.drawLine(pointlist[i - 1], pointlist[i]);
	}

}

void Curve::add_end(QPoint s) {
	
	while (controlpoints.size() > 2) {
		controlpoints.pop_back();
	}
	controlpoints.push_back(s);
	
}

