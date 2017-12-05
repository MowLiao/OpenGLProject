#include "GraphicsWindow.h"
#include <iostream>

// constructor / destructor
GraphicsWindow::GraphicsWindow(QWidget *parent)
	: QWidget(parent)
	{
		
		// create the window layout
		windowLayout = new QBoxLayout(QBoxLayout::TopToBottom, this);

		// create main widget
		graphicsWidget = new GraphicsWidget(this);
		windowLayout->addWidget(graphicsWidget);

		// create xslider
		xSlider = new QSlider(Qt::Horizontal);
		xSlider->setMinimum(-180);
		xSlider->setMaximum(180);
		xSlider->setValue(0);
        connect(xSlider, SIGNAL(valueChanged(int)), graphicsWidget, SLOT(updateXZAngle(int)));

		// create yslider
		ySlider = new QSlider(Qt::Horizontal);
		ySlider->setMinimum(40);
		ySlider->setMaximum(100);
		ySlider->setValue(50);
        connect(ySlider, SIGNAL(valueChanged(int)), graphicsWidget, SLOT(updateYAngle(int)));

        windowLayout->addWidget(xSlider);
        windowLayout->addWidget(ySlider);
        // windowLayout->addWidget(zoomSlider);

        
		ptimer = new QTimer(this);
        connect(ptimer, SIGNAL(timeout()),  graphicsWidget, SLOT(updatePos())); 
        ptimer->start(20);
        

	} 

GraphicsWindow::~GraphicsWindow()
	{
	  	delete ptimer; 
		delete xSlider;
		delete graphicsWidget;
		delete windowLayout;
		//delete actionQuit;
		//delete fileMenu;
		//delete menuBar;
	}

// resets all the interface elements
void GraphicsWindow::ResetInterface()
	{ 
	
		// now force refresh
		graphicsWidget->update();
		update();
	} 
