#include "minidraw.h"
#include <iostream>

MiniDraw::MiniDraw(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	view_widget_ = new ViewWidget();
	pColorBar = new ColorCombox();
	pColorBar->setButtonIcon("../images/icon.png", Qt::black);
	this->resize(QSize(1800, 1200));
	Creat_Action();
	Creat_ToolBar();
	Creat_Menu();

	setCentralWidget(view_widget_);
}

void MiniDraw::Creat_Action() {
	Action_About = new QAction(tr("&About"), this);
	connect(Action_About, &QAction::triggered, this, &MiniDraw::AboutBox);

	Action_Clean = new QAction(tr("&Clean"), this);
	connect(Action_Clean, &QAction::triggered, view_widget_, &ViewWidget::setClean);

	Action_Undo = new QAction(tr("&Undo"), this);
	connect(Action_Undo, &QAction::triggered, view_widget_, &ViewWidget::setUndo);

	Action_Back = new QAction(tr("&Back"), this);
	connect(Action_Back, &QAction::triggered, view_widget_, &ViewWidget::setBack);
	Action_Line = new QAction(tr("&Line"), this);

	connect(Action_Line, SIGNAL(triggered()), view_widget_, SLOT(setLine()));

	Action_Rect = new QAction(tr("&Rect"), this);
	connect(Action_Rect, &QAction::triggered, view_widget_, &ViewWidget::setRect);

	Action_Ellipse = new QAction(tr("&Ellipse"), this);
	connect(Action_Ellipse, SIGNAL(triggered()), view_widget_, SLOT(setEllipse()));

	Action_Circle = new QAction(tr("&Circle"), this);
	connect(Action_Circle, SIGNAL(triggered()), view_widget_, SLOT(setCircle()));

	Action_Polygon = new QAction(tr("&Polygon"), this);
	connect(Action_Polygon, &QAction::triggered, view_widget_, &ViewWidget::setPolygon);

	Action_Freeline = new QAction(tr("&Freeline"), this);
	connect(Action_Freeline, &QAction::triggered, view_widget_, &ViewWidget::setFreeline);

	Action_Curve = new QAction(tr("&Curve"), this);
	connect(Action_Curve, &QAction::triggered, view_widget_, &ViewWidget::setCurve);

	Action_Penwidthshort = new QAction(tr("&Short"), this);
	connect(Action_Penwidthshort, &QAction::triggered, view_widget_, &ViewWidget::setPenwidthshort);

	Action_Penwidthmedium = new QAction(tr("&Medium"), this);
	connect(Action_Penwidthmedium, &QAction::triggered, view_widget_, &ViewWidget::setPenwidthmedium);

	Action_Penwidthlong = new QAction(tr("&Long"), this);
	connect(Action_Penwidthlong, &QAction::triggered, view_widget_, &ViewWidget::setPenwidthlong);

	Action_Selectcolor = new QAction(tr("&Long"), this);
	connect(Action_Selectcolor, &QAction::triggered, pColorBar, &ColorCombox::OnColorChanged);
	connect(Action_Selectcolor, &QAction::triggered, pColorBar, &ColorCombox::OnShowColorBoard);
	connect(Action_Selectcolor, &QAction::triggered, pColorBar, &ColorCombox::changeColor);


	connect(pColorBar, &ColorCombox::sigColorChanged, view_widget_, &ViewWidget::receiveColor);
	emit pColorBar->sigColorChanged(pColorBar->getColor());
	

}

void MiniDraw::Creat_ToolBar() {
	pToolBar = addToolBar(tr("&Main"));
	pToolBar->addAction(Action_Undo);
	pToolBar->addAction(Action_Back);
	pToolBar->addAction(Action_Clean);
	pToolBar->addAction(Action_Line);
	pToolBar->addAction(Action_Freeline);
	pToolBar->addAction(Action_Curve);
	pToolBar->addAction(Action_Rect);
	pToolBar->addAction(Action_Ellipse);
	pToolBar->addAction(Action_Circle);
	pToolBar->addAction(Action_Polygon);

	//pToolBar->addAction(Action_Penwidthshort);
	//pToolBar->addAction(Action_Penwidthmedium);
	//pToolBar->addAction(Action_Penwidthlong);
	QMenu* subToolbar = new QMenu("test", this);
	subToolbar->addAction(Action_Penwidthshort);
	subToolbar->addAction(Action_Penwidthmedium);
	subToolbar->addAction(Action_Penwidthlong);
	QToolButton* penButton = new QToolButton(this);
	penButton->setText("PenWidth");
	penButton->setPopupMode(QToolButton::InstantPopup);
	penButton->setMenu(subToolbar);
	pToolBar->addWidget(penButton);

	pToolBar->addWidget(pColorBar);
	pToolBar->addAction(Action_About);
}

void MiniDraw::Creat_Menu() {
	QMenu* menu_edit = menuBar()->addMenu(tr("edit"));
	QMenu* menu_tool = menuBar()->addMenu(tr("&Figure Tool"));
	QMenu* menu_help = menuBar()->addMenu(tr("help"));
	
	menu_edit->addAction(Action_Undo);
	menu_edit->addAction(Action_Back);
	
	menu_tool->addAction(Action_Clean);
	menu_tool->addAction(Action_Line);
	menu_tool->addAction(Action_Curve);
	menu_tool->addAction(Action_Freeline);
	menu_tool->addAction(Action_Rect);
	menu_tool->addAction(Action_Ellipse);
	menu_tool->addAction(Action_Circle);
	menu_tool->addAction(Action_Polygon);

	menu_help->addAction(Action_About);
}

void MiniDraw::AboutBox() {
	QMessageBox::about(this, tr("About"), tr("Created by noway2023"));
}


MiniDraw::~MiniDraw() {
	
}
