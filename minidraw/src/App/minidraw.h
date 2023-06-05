#pragma once

#include <ui_minidraw.h>
#include <viewwidget.h>
#include <QToolBar>
#include <QToolButton>
#include <QtWidgets/QMainWindow>
#include <qmessagebox.h>
#include "Colorcombox.h"

class MiniDraw : public QMainWindow {
	Q_OBJECT

public:
	MiniDraw(QWidget* parent = 0);
	~MiniDraw();

	QMenu* pMenu;
	QToolBar* pToolBar;
	QAction* Action_About;
	QAction* Action_Clean;
	QAction* Action_Undo;
	QAction* Action_Back;

	QAction* Action_Line;
	QAction* Action_Rect;
	QAction* Action_Ellipse;
	QAction* Action_Circle;
	QAction* Action_Polygon;
	QAction* Action_Freeline;
	QAction* Action_Curve;
	QAction* Action_Penwidthshort;
	QAction* Action_Penwidthmedium;
	QAction* Action_Penwidthlong;
	QAction* Action_Selectcolor;


	void Creat_Menu();
	void Creat_ToolBar();
	void Creat_Action();

	void AboutBox();

private:
	Ui::MiniDrawClass ui;
	ViewWidget* view_widget_;
	ColorCombox* pColorBar;
};
