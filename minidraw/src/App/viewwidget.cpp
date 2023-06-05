#include "viewwidget.h"
#include "iostream"




ViewWidget::ViewWidget(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	draw_status_ = false;
	ispoly_ = false;
	isfree = false;
	iscurve = false;
	isthird = false;
	ismedium = true;
	dclick = true;
	shape_ = NULL;
	type_ = Shape::kDefault;
	//this->setMouseTracking(true);
	penwidth = 1;
	pencolor = Qt::black;
}

ViewWidget::~ViewWidget()
{
	for (size_t i = 0; i < shape_list_.size(); i++) {
		if (shape_list_[i]) {
			delete shape_list_[i];
			shape_list_[i] = NULL;
		}
	}
	for (size_t i = 0; i < back_list_.size(); i++) {
		if (back_list_[i]) {
			delete back_list_[i];
			back_list_[i] = NULL;
		}
	}
}


void ViewWidget::setClean() {
	type_ = Shape::kClean;
	shape_ = new Clean(this);
	shape_list_.push_back(shape_);
	shape_ = NULL;
}

void ViewWidget::setUndo() {
	if (shape_list_.size()==0) return;
	Shape* back_shape = shape_list_.back();
	back_list_.push_back(back_shape);
	shape_list_.pop_back();
}

void ViewWidget::setBack() {
	if (back_list_.size() == 0) return;
	Shape* back_shape = back_list_.back();
	shape_list_.push_back(back_shape);
	back_list_.pop_back();
}


void ViewWidget::setLine()
{
	type_ = Shape::kLine;
	ispoly_ = false;
	iscurve = false;
}

void ViewWidget::setRect()
{
	type_ = Shape::kRect;
	ispoly_ = false;
	iscurve = false;
}

void ViewWidget::setEllipse()
{
	type_ = Shape::kEllipse;
	ispoly_ = false;
	iscurve = false;
}

void ViewWidget::setCircle()
{
	type_ = Shape::kCircle;
	ispoly_ = false;
	iscurve = false;
}

void ViewWidget::setPolygon()
{
	type_ = Shape::kPolygon;
	ispoly_ = true;
	iscurve = false;
}

void ViewWidget::setFreeline()
{
	type_ = Shape::kFreeline;
	ispoly_ = false;
	iscurve = false;
}

void ViewWidget::setCurve()
{
	type_ = Shape::kCurve;
	ispoly_ = false;
	
}

void ViewWidget::setPenwidthshort()
{
	penwidth = 1;
}

void ViewWidget::setPenwidthmedium()
{
	penwidth = 2;
}

void ViewWidget::setPenwidthlong()
{
	penwidth = 3;
}

void ViewWidget::receiveColor(QColor color)
{
	pencolor = color;
}

void ViewWidget::mousePressEvent(QMouseEvent* event)
{
	if (Qt::LeftButton == event->button())
	{
		switch (type_)
		{
		case Shape::kLine:
			shape_ = new Line();
			break;
		case Shape::kDefault:
			break;

		case Shape::kRect:
			shape_ = new Rect();
			break;
		case Shape::kEllipse:
			shape_ = new Elli();
			break;
		case Shape::kPolygon:
			if (dclick) {
				shape_ = new Poly();
				shapepoly = new Poly();
				shape_->set_start(event->pos());
			}
			break;
		case Shape::kFreeline:
			isfree = true;
			shape_ = new Freeline();
			break;
		case Shape::kCircle:
			shape_ = new Circle();
			break;
		case Shape::kCurve:
			if (!isthird) {
				iscurve = true;
				shape_ = new Curve();
				shape_->add_end(event->pos());
			}
			break;
		case Shape::kClean:
			break;
		
		}
		if (shape_ != NULL)
		{
			if (!ispoly_) {
				if (isthird) {
					return;
				}

				draw_status_ = true;
				start_point_ = end_point_ = event->pos();
				shape_->set_start(start_point_);
				shape_->set_end(end_point_);
				shape_->set_width(penwidth);
				shape_->set_color(pencolor);
			}
			else {
				
				dclick = false;
				draw_status_ = true;
				if (ismedium) {
					shape_->add_end(event->pos());
					shape_->set_end(event->pos());
					shape_->set_width(penwidth);
					shape_->set_color(pencolor);
				}
				shapepoly->add_end(event->pos());
				shapepoly->set_width(penwidth);
				shapepoly->set_color(pencolor);
			}
		}
	}
	update();
}

void ViewWidget::mouseMoveEvent(QMouseEvent* event)
{
	if (draw_status_ && shape_ != NULL)
	{
		if (iscurve) {
			if (!isthird) { 
				//isthird = true;
				//shape_->add_end(event->pos());
			}
			else {
				shape_->add_end(event->pos());
			}
			return;
		}
		if (!ispoly_) {
			end_point_ = event->pos();
			if (!isfree)
				shape_->set_end(end_point_);
			else shape_->add_end(end_point_);
		}
		else {
			ismedium = true;
			while (shapepoly->endpoint() != shape_->get_end()) {
				shapepoly->pop_end();
			}
			shapepoly->add_end(event->pos());
		}
	}
}

void ViewWidget::mouseDoubleClickEvent(QMouseEvent* event) {
	if (shape_ != NULL) {
		if (ispoly_) {
			ismedium = true;
			dclick = true;
			draw_status_ = false;
			shape_->add_end(shape_->get_start());
			
			shape_list_.push_back(shape_);
			shape_ = NULL;
		}
	}
}
void ViewWidget::mouseReleaseEvent(QMouseEvent* event)
{
	if (shape_ != NULL)
	{
		if (iscurve) {
			if (!isthird) {
				isthird = true;
				shape_->add_end(event->pos());
			}
			else{
				draw_status_ = false;
				shape_list_.push_back(shape_);
				shape_ = NULL;
				iscurve = false;
				isthird = false;
			}
			return;
		}
		if (!ispoly_) {
			draw_status_ = false;
			shape_list_.push_back(shape_);
			shape_ = NULL;
			if (isfree) isfree = false;
		}
		else {
			ismedium = false;
			shape_->add_end(event->pos());
			shape_->set_end(event->pos());
			shape_->set_width(penwidth);
			shape_->set_color(pencolor);
			shapepoly->add_end(event->pos());
			shapepoly->set_width(penwidth);
			shapepoly->set_color(pencolor);
		}
	}
}

void ViewWidget::paintEvent(QPaintEvent*)
{
	QPainter painter(this);
	QPen pen;

	for (int i = 0; i < shape_list_.size(); i++)
	{
		shape_list_[i]->Draw(painter);
	}

	if (shape_ != NULL) {
		if (ispoly_ && ismedium) {
			shapepoly->Draw(painter);
		}
		else{
			shape_->Draw(painter);
		}
		
	}
	update();
}