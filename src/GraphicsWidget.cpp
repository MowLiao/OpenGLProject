#include <GL/glu.h>
#include <QGLWidget>
#include <QSlider>
#include <math.h>
#include <iostream>
#include "GraphicsWidget.h"

#define PI 3.141592653589793


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
                                : QGLWidget(parent)
{      
    /*phead      = gluNewQuadric();
    pshoulders = gluNewQuadric();
    ptorso     = gluNewQuadric();
    pupperleft = gluNewQuadric();
    plowerleft = gluNewQuadric();
    */

    xcamera = 0.0;
    ycamera = 2.5;
    zcamera = 5.0;

    catcircle = 90;
    catpos = 0.0;
    catup = 0.4;

    movementtick = 0.0;

    segthickness = 0.4;
    segrotate = 8.0;
    segseparate = 1.0;
    segwidth = 1.0;
    segtotal = int(catcircle)/int(segrotate);
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
    //gluDeleteQuadric(catsegment);
    //gluDeleteQuadric(catdisk);
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
    float scale = 15.0;
    glOrtho(-scale, scale, -scale, scale, -scale, scale);

}

void GraphicsWidget::squarePlane()
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

void GraphicsWidget::cylinderPlane()
{
    glPushMatrix();
    glRotatef(90, 1.0, 0.0, 0.0);
    GLUquadricObj* plane = gluNewQuadric();
    gluCylinder(plane, 12.0, 12.0, 5.0, 50, 10);
    GLUquadricObj* disk = gluNewQuadric();
    gluDisk(disk, 0, 12.0, 50, 50);
    glTranslatef(0.0, 0.0, 5.0);
    gluDisk(disk, 0, 12.0, 50, 50);
    glPopMatrix();
}

void GraphicsWidget::segment(int segno)
{
    //float startsize = maxsize - segno*0.005;
    //float endsize = maxsize - (segno+1)*0.005;

    GLUquadricObj* catsegment = gluNewQuadric();
    GLUquadricObj* catdisk = gluNewQuadric();
    gluCylinder(catsegment, segwidth, segwidth, segthickness, 25, 5);

    gluDisk(catdisk, 0, segwidth, 25, 10);

    glTranslatef(0.0, 0.0, segthickness);
    gluDisk(catdisk, 0, segwidth, 25, 10);
    gluCylinder(catsegment, segwidth*0.5, segwidth*0.5, segthickness*2, 25, 5);
    // De-translate
    glTranslatef(0.0, 0.0, -segthickness);


    // Legs
    float xtranslate = segwidth * 0.8;

    glRotatef(90,1.0,0.0,0.0);
    glTranslatef(xtranslate, segthickness/2, 0.0);

    gluCylinder(catsegment, segthickness*0.4, segthickness*0.2, segwidth, 10, 5);
    glTranslatef(-2 * xtranslate, 0.0, 0.0);
    gluCylinder(catsegment, segthickness*0.4, segthickness*0.2, segwidth, 10, 5);
    glTranslatef(xtranslate, -segthickness/2, 0.0);
    // Derotate
    glRotatef(-90,1.0,0.0,0.0);

}

void GraphicsWidget::tail(int i)
{
    GLUquadricObj* catsegment = gluNewQuadric();
    GLUquadricObj* catdisk = gluNewQuadric();
    gluCylinder(catsegment, segwidth-((i+1)*0.15), segwidth-(i*0.15), segthickness, 25, 5);

    gluDisk(catdisk, 0, segwidth-((i+1)*0.15), 25, 10);

    glTranslatef(0.0, 0.0, segthickness);
    gluDisk(catdisk, 0, segwidth-(i*0.15), 25, 10);
    gluCylinder(catsegment, (segwidth-((i+1)*0.15))/2, (segwidth-(i*0.15))/2, segthickness*2, 25, 5);
    // De-translate
    glTranslatef(0.0, 0.0, -segthickness);
}

// called every time the widget needs painting
void GraphicsWidget::paintGL()
{
	// clear the widget
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glShadeModel(GL_SMOOTH);

    // global rotation angle
    //glRotatef((double)xcamera, 0.0, 1.0, 0.0);
    //glRotatef((double)ycamera, 1.0, 0.0, 0.0);

	// You must set the matrix mode to model view directly before enabling the depth test
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_DEPTH_TEST); // comment out depth test to observe the result

	materialStruct* p_front = &whiteShinyMaterials;
	
    glMaterialfv(GL_FRONT, GL_AMBIENT,    p_front->ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE,    p_front->diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR,   p_front->specular);
	glMaterialf(GL_FRONT, GL_SHININESS,   p_front->shininess);
	
    // Draw normal plane
    // this->squarePlane();
    this -> cylinderPlane();

    p_front = &brassMaterials;
    
    glMaterialfv(GL_FRONT, GL_AMBIENT,    p_front->ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,    p_front->diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,   p_front->specular);
    glMaterialf(GL_FRONT, GL_SHININESS,   p_front->shininess);

    glPushMatrix();
        // Start drawing caterpillar
        int segno = 0;
        float movementsquidge = 0.0;
        float movementup = 0.0;
        float cosconstant = 0.0;
        glTranslatef(-7.2, segwidth/5, 0.0);

        for (int r=0; r<int(catpos*10); r++)
        {
            glRotatef(segrotate/10, 0.0, 1.0 ,0.0);
            glTranslatef(0.0, 0.0, segseparate/10);
        }

        // Tail
        for (int j=4; j > 0; j=j-1)
        {
            segno++;
            cosconstant = ((float) segno + movementtick)/1.5;
            movementsquidge = 0.25 * sin( cosconstant) + 0.75;
            movementup = (cos(cosconstant)+1)/2;
            glTranslatef(0.0, (segwidth/5) + (movementup*catup) , (movementsquidge*segseparate));
            glRotatef(movementsquidge*segrotate, 0.0, 1.0 ,0.0);
            this->tail(j);
            glTranslatef(0.0, -movementup*catup, 0.0);
        }
        
        // Body
        for (int i=4; i < segtotal+4; i++)
        {
            segno++;
            cosconstant = ((float) segno + movementtick)/1.5;
            movementsquidge = 0.25 * sin(cosconstant) + 0.75;
            movementup = (cos(cosconstant)+1)/2;

            glTranslatef(0.0, movementup*catup, (movementsquidge*segseparate));
            glRotatef(movementsquidge*segrotate, 0.0, 1.0 ,0.0);
            this->segment(segno);
            glTranslatef(0.0, -movementup*catup, 0.0);
        }
        
            
        segno++;
        cosconstant = ((float) segno + movementtick)/1.5;
        movementsquidge = 0.25 * sin(cosconstant) + 0.75;
        movementup = (cos(cosconstant)+1)/2;

    glPopMatrix();

	glLoadIdentity();
    //gluLookAt(  0.0,    0.0,    1.0, 
    gluLookAt(  xcamera,    ycamera,    zcamera, 
                0.0,        0.0,        0.0, 
                0.0,        1.0,        0.0);
	
	// flush to screen
	glFlush();	

}


void GraphicsWidget::updateXZAngle(int i)
{
    float in = (float) i;
    xcamera = 5 * sin(in * PI/180);
    zcamera = 5 * cos(in * PI/180);
    this->repaint();
}


void GraphicsWidget::updateYAngle(int i)
{
    ycamera = (float)i/10;
    this->repaint();
}

void GraphicsWidget::updateCatPos()
{
    catpos = fmod(catpos + 0.05, 360.0);
    movementtick+=0.07;
    this->repaint();
}
