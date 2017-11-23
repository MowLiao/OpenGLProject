#ifndef __GL_POLYGON_WINDOW_H__
#define __GL_POLYGON_WINDOW_H__ 1

#include <QGLWidget>
#include <QMenuBar>
#include <QSlider>
#include <QBoxLayout>
#include <QTimer>
#include "GraphicsWidget.h"

class GraphicsWindow: public QWidget
	{ 
	public:  
	
	// constructor / destructor
	GraphicsWindow(QWidget *parent);
	~GraphicsWindow();

	// visual hierarchy
	// menu bar
	QMenuBar *menuBar;
		// file menu
		QMenu *fileMenu;
			// quit action
			QAction *actionQuit;

	// window layout
	QBoxLayout *windowLayout;

	// beneath that, the main widget
	GraphicsWidget *graphicsWidget;

	// and a slider for the number of vertices
	QSlider *nSlider;

	// timer for animation
	QTimer* ptimer;

	// resets all the interface elements
	void ResetInterface();
	}; 
	
#endif
