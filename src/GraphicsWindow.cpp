#include "GraphicsWindow.h"
#include <iostream>

// constructor / destructor
GraphicsWindow::GraphicsWindow(QWidget *parent)
	: QWidget(parent)
	{
		// create menu bar
		menuBar = new QMenuBar(this);
		
		// create file menu
		fileMenu = menuBar->addMenu("&File");

		// create the action
		actionQuit = new QAction("&Quit", this);

		// leave signals & slots to the controller
		
		// add the item to the menu
		fileMenu->addAction(actionQuit);
		
		// create the window layout
		windowLayout = new QBoxLayout(QBoxLayout::TopToBottom, this);

		// create main widget
		graphicsWidget = new GraphicsWidget(this);
		windowLayout->addWidget(graphicsWidget);

		// create slider
		nSlider = new QSlider(Qt::Horizontal);
		nSlider->setMinimum(0);
		nSlider->setMaximum(360);
		nSlider->setValue(0);
        connect(nSlider, SIGNAL(valueChanged(int)), graphicsWidget, SLOT(updateAngle(int)));
        windowLayout->addWidget(nSlider);

        /*
		ptimer = new QTimer(this);
        connect(ptimer, SIGNAL(timeout()),  graphicsWidget, SLOT(updateAngle())); 
        ptimer->start(0);
        */

	} 

GraphicsWindow::~GraphicsWindow()
	{
	  	//delete ptimer; 
		delete nSlider;
		delete graphicsWidget;
		delete windowLayout;
		delete actionQuit;
		delete fileMenu;
		delete menuBar;
	}

// resets all the interface elements
void GraphicsWindow::ResetInterface()
	{ 
	
		// now force refresh
		graphicsWidget->update();
		update();
	} 
