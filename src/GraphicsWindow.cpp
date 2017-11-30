#include "GraphicsWindow.h"
#include <iostream>

// constructor / destructor
GraphicsWindow::GraphicsWindow(QWidget *parent)
	: QWidget(parent)
	{
		// create menu bar
		//menuBar = new QMenuBar(this);
		
		// create file menu
		//fileMenu = menuBar->addMenu("&File");

		// create the action
		//actionQuit = new QAction("&Quit", this);

		// leave signals & slots to the controller
		
		// add the item to the menu
		//fileMenu->addAction(actionQuit);
		
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
		ySlider->setMinimum(10);
		ySlider->setMaximum(50);
		ySlider->setValue(15);
        connect(ySlider, SIGNAL(valueChanged(int)), graphicsWidget, SLOT(updateYAngle(int)));

		// create treeslider
		//branchSlider = new QSlider(Qt::Horizontal);
		//branchSlider->setMinimum(2);
		//branchSlider->setMaximum(8);
		//branchSlider->setValue(5);
        //connect(ySlider, SIGNAL(valueChanged(int)), graphicsWidget, SLOT(updateBranches(int)));

        windowLayout->addWidget(xSlider);
        windowLayout->addWidget(ySlider);
        //windowLayout->addWidget(branchSlider);

        
		ptimer = new QTimer(this);
        connect(ptimer, SIGNAL(timeout()),  graphicsWidget, SLOT(updateCatPos())); 
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
