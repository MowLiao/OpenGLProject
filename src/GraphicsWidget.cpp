#include <GL/glu.h>
#include <QGLWidget>
#include <QSlider>
#include <cmath>
#include <iostream>
#include "GraphicsWidget.h"


// Setting up material properties
typedef struct materialStruct 
                                {
                                    GLfloat ambient[4];
                                    GLfloat diffuse[4];
                                    GLfloat specular[4];
                                    GLfloat shininess;
                                } materialStruct;


static materialStruct brassMaterials = 
                                        {
                                            { 0.33, 0.22, 0.03, 1.0},
                                            { 0.78, 0.57, 0.11, 1.0},
                                            { 0.99, 0.91, 0.81, 1.0},
                                          27.8 
                                        };

static materialStruct whiteShinyMaterials = 
                                        {
                                            { 1.0, 1.0, 1.0, 1.0},
                                            { 1.0, 1.0, 1.0, 1.0},
                                            { 1.0, 1.0, 1.0, 1.0},
                                            100.0 
                                        };

// constructor
GraphicsWidget::GraphicsWidget(QWidget *parent)
                                : QGLWidget(parent),
                                angle(0),
                                larmangle(0),
                                rarmangle(90)
{      
    /*phead      = gluNewQuadric();
    pshoulders = gluNewQuadric();
    ptorso     = gluNewQuadric();
    pupperleft = gluNewQuadric();
    plowerleft = gluNewQuadric();
    */
    psegment = gluNewQuadric();

    segrotate = 8.0;
    segseparate = 0.2;
    squiggles = 2;

}

// called when OpenGL context is set up
void GraphicsWidget::initializeGL()
{
    GLfloat r = float(166)/255.0;
    GLfloat g = float(206)/255.0;
    GLfloat b = float(241)/255.0; 
	// set the widget background colour
	glClearColor(r, g, b, 0.0);
}

GraphicsWidget::~GraphicsWidget()
{
    /*
    gluDeleteQuadric(phead);
    gluDeleteQuadric(pshoulders);
    gluDeleteQuadric(ptorso);
    gluDeleteQuadric(pupperleft);
    gluDeleteQuadric(plowerleft);
    */
    gluDeleteQuadric(psegment);
}

// called every time the widget is resized
void GraphicsWidget::resizeGL(int w, int h)
{
    // set the viewport to the entire widget
    glViewport(0, 0, w, h);
    GLfloat light_pos[] = {1., 0.5, 1., 0.};

    glEnable(GL_LIGHTING); // enable lighting in general
    glEnable(GL_LIGHT0);   // each light source must also be enabled

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    materialStruct* p_front = &brassMaterials;
    
    glMaterialfv(GL_FRONT, GL_AMBIENT,    p_front->ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,    p_front->diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,   p_front->specular);
    glMaterialf(GL_FRONT, GL_SHININESS,   p_front->shininess);
    
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    glLightf (GL_LIGHT0, GL_SPOT_CUTOFF,150.);

        
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-8.0, 8.0, -8.0, 8.0, -8.0, 8.0);

}

void GraphicsWidget::segment(int segno)
{
    float maxsize = 0.9;
    //float startsize = maxsize - segno*0.005;
    //float endsize = maxsize - (segno+1)*0.005;
    gluCylinder(psegment, maxsize, maxsize, 0.1, 25, 5);
}

void GraphicsWidget::plane()
{

    // Here we give every face the same normal
    GLfloat normals[][3] = { {0., 1. ,0.}, {0., -1., 0.}, {1., 0., 1.}, {0., 0., -1.} };;
    materialStruct* p_front = &brassMaterials;
    
    glMaterialfv(GL_FRONT, GL_AMBIENT,    p_front->ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,    p_front->diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,   p_front->specular);
    glMaterialf(GL_FRONT, GL_SHININESS,   p_front->shininess);

    glNormal3fv(normals[0]);
    glBegin(GL_POLYGON);
    glVertex3f( -10.0, 0.0,  10.0);
    glVertex3f( -10.0, 0.0, -10.0);
    glVertex3f( 10.0,  0.0, -10.0);
    glVertex3f( 10.0,  0.0,  10.0);
    glEnd();
}

/*
void GraphicsWidget::head(){
  gluSphere(phead,1.0,20,20);
}
*/

// called every time the widget needs painting
void GraphicsWidget::paintGL()
{
	// clear the widget
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glShadeModel(GL_SMOOTH);

	// You must set the matrix mode to model view directly before enabling the depth test
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_DEPTH_TEST); // comment out depth test to observe the result

	// global rotation angle
    glRotatef((double)angle,0.,1.,0.);

	materialStruct* p_front = &whiteShinyMaterials;
	
    glMaterialfv(GL_FRONT, GL_AMBIENT,    p_front->ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE,    p_front->diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR,   p_front->specular);
	glMaterialf(GL_FRONT, GL_SHININESS,   p_front->shininess);
	
    int segno = 1;

    glPushMatrix();
        this->plane();
        segno++;

        glTranslatef(0.0, 1.0, 0.0);

        this->segment(segno);

        for (int j=0; j < squiggles; j++)
        {
            for (int i=0; i < int(90/int(segrotate)); i++)
            {
                glTranslatef(0.0, 0.0, segseparate);
                glRotatef(segrotate, 0.0, 1.0 ,0.0);
                segno++;
                this->segment(segno);
            }
            for (int i=0; i < int(180/int(segrotate)); i++)
            {
                glTranslatef(0.0, 0.0, segseparate);
                glRotatef(segrotate, 0.0, -1.0 ,0.0);
                segno++;
                this->segment(segno);
            }
        }
    glPopMatrix();

	glLoadIdentity();
    gluLookAt(1.,1.,1., 0.0,0.0,0.0, 0.0,1.0,0.0);
	
	// flush to screen
	glFlush();	

}


void GraphicsWidget::updateAngle(int i)
{
  angle = i;
  this->repaint();
}

void GraphicsWidget::updateArmAngle(int i)
{
  // find out about the object that sent
  larmangle = i;
  rarmangle = 90-i;
  this->repaint();
}
