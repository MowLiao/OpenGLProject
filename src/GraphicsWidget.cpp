#include <GL/glu.h>
#include <QGLWidget>
#include <QSlider>
#include <math.h>
#include <iostream>
#include "GraphicsWidget.h"

#define PI 3.141592653589793
#define GOLDEN_RATIO 1.61803398875


// Setting up material properties
typedef struct materialStruct 
                                {
                                    GLfloat ambient[4];
                                    GLfloat diffuse[4];
                                    GLfloat specular[4];
                                    GLfloat shininess;
                                } materialStruct;


static materialStruct planeMaterial = 
                                        {
                                            { 1.0, 1.0, 1.0, 1.0},
                                            { 1.0, 1.0, 1.0, 1.0},
                                            { 1.0, 1.0, 1.0, 1.0},
                                            100.0 
                                        };

static materialStruct middleMaterial = 
                                        {
                                            { 0.2, 0.4, 0.1, 1.0},
                                            { 0.4, 0.5, 0.2, 1.0},
                                            { 0.02, 0.15, 0.08, 0.5},
                                            89
                                        };


static materialStruct legMaterial = 
                                        {
                                            { 0.2, 0.4, 0.1, 1.0},
                                            { 0.9, 0.9, 0.6, 1.0},
                                            { 0.02, 0.15, 0.08, 0.5},
                                            89
                                        };


static materialStruct catMaterial = 
                                        {
                                            { 0.2, 0.4, 0.1, 1.0},
                                            { 0.7, 0.7, 0.5, 1.0},
                                            { 0.02, 0.15, 0.08, 0.5},
                                            89
                                        };


static materialStruct trunkMaterial = 
                                        {
                                            { 0.1, 0.1, 0.1, 1.0},
                                            { 0.8, 0.75, 0.7, 1.0},
                                            { 0.02, 0.02, 0.02, 0.5},
                                            89
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
    catup = 0.5;

    movementtick = 0.0;

    segthickness = 0.4;
    segrotate = 8.0;
    segseparate = 1.0;
    segwidth = 1.0;
    segtotal = int(catcircle)/int(segrotate);

    branchiterate = 5;
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
    
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    glLightf (GL_LIGHT0, GL_SPOT_CUTOFF,150.);

        
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float scale = 15.0;
    glOrtho(-scale, scale, -scale, scale, -100, 100);

}

void GraphicsWidget::squarePlane()
{
    // Here we give every face the same normal
    GLfloat normals[][3] = { {0., 1. ,0.}, {0., -1., 0.}, {1., 0., 1.}, {0., 0., -1.} };;
    changeMaterial(&planeMaterial);

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

void GraphicsWidget::changeMaterial(materialStruct* materialName)
{
    materialStruct* p_front = materialName;
    
    glMaterialfv(GL_FRONT, GL_AMBIENT,    p_front->ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,    p_front->diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,   p_front->specular);
    glMaterialf(GL_FRONT, GL_SHININESS,   p_front->shininess);
}

void GraphicsWidget::branch(    int iterate,
                                float treeheight, 
                                float rotate, 
                                float point1, 
                                float point2, 
                                float point3    )
{
    iterate = iterate-1;
    treeheight = treeheight / GOLDEN_RATIO;
    rotate = rotate / GOLDEN_RATIO;
    point1 = point1 / GOLDEN_RATIO;
    point2 = point2 / GOLDEN_RATIO;
    point3 = point3 / GOLDEN_RATIO;

    glTranslatef(0.0, treeheight, 0.0);

    for (int i=0; i<3; i++)
    {
        glRotatef(rotate, 1.0, 0.0, 0.0);

        glBegin(GL_POLYGON);
            glVertex3f( 0.0,  treeheight, 0.0);
            glVertex3f( -point1, 0.0, point3);
            glVertex3f( point1, 0.0, point3);

            glVertex3f( 0.0,  treeheight, 0.0);
            glVertex3f( point1, 0.0, point3);
            glVertex3f( 0.0, 0.0, point2);

            glVertex3f( 0.0,  treeheight, 0.0);
            glVertex3f( 0.0, 0.0, point2);
            glVertex3f( -point1, 0.0, point3);
        glEnd();
        if (iterate > 0)
        {
            this -> branch(iterate-1, treeheight, rotate, point1, point2, point3);
        }

        glRotatef(-rotate, 1.0, 0.0, 0.0);
        glRotatef(120, 0.0, 1.0, 0.0);
    }

    glTranslatef(0.0, -treeheight, 0.0);
}

void GraphicsWidget::tree(int iterate)
{
    // Draw in counter clockwise
    //  base:
    //  (-0.50, -0.28)
    //  ( 0.50, -0.28)
    //  ( 0.00,  0.56)
    float treeheight = 12.0;
    float rotate = 90;
    float point1 = 1;
    float point2 = 1.12;
    float point3 = -0.56;

    changeMaterial(&trunkMaterial);
    glBegin(GL_POLYGON);
        glVertex3f( 0.0,  treeheight, 0.0);
        glVertex3f( -point1, 0.0, point3);
        glVertex3f( point1, 0.0, point3);

        glVertex3f( 0.0,  treeheight, 0.0);
        glVertex3f( point1, 0.0, point3);
        glVertex3f( 0.0, 0.0, point2);

        glVertex3f( 0.0,  treeheight, 0.0);
        glVertex3f( 0.0, 0.0, point2);
        glVertex3f( -point1, 0.0, point3);
    glEnd();

    this -> branch(iterate, treeheight, rotate, point1, point2, point3);

}

void GraphicsWidget::catsegment(float movevar)
{
    //float startsize = maxsize - segno*0.005;
    //float endsize = maxsize - (segno+1)*0.005;

    GLUquadricObj* catsegments = gluNewQuadric();
    GLUquadricObj* catdisks = gluNewQuadric();
    float legmov = (cos(movevar)+1)/2;

    // Create outer ring
    changeMaterial(&catMaterial);
    gluCylinder(catsegments, segwidth, segwidth, segthickness, 25, 5);
    // Create disks on either side
    gluDisk(catdisks, 0, segwidth, 25, 10);
    glTranslatef(0.0, 0.0, segthickness);
    gluDisk(catdisks, 0, segwidth, 25, 10);

    // Create smaller center part
    changeMaterial(&middleMaterial);
    gluCylinder(catsegments, segwidth*0.8, segwidth*0.8, segthickness*1.5, 25, 5);
    // De-translate
    glTranslatef(0.0, 0.0, -segthickness);

    changeMaterial(&legMaterial);

    // Legs
    float xtranslate = segwidth * 0.6;
    float legrot = 10.0*legmov;
    float legup = 0.2*legmov;

    glRotatef(90,1.0,0.0,0.0);
    glTranslatef(xtranslate, segthickness/2, -legup);
        glRotatef(legrot,0.0,1.0,0.0);
            gluCylinder(catsegments, segthickness*0.4, segthickness*0.3, segwidth, 10, 5);
        glRotatef(-legrot,0.0,1.0,0.0);
    glTranslatef(-2 * xtranslate, 0.0, 0.0);
        glRotatef(-legrot,0.0,1.0,0.0);
            gluCylinder(catsegments, segthickness*0.4, segthickness*0.3, segwidth, 10, 5);
        glRotatef(legrot,0.0,1.0,0.0);
    glTranslatef(xtranslate, -segthickness/2, legup);

    // Back to normal
    glRotatef(-90,1.0,0.0,0.0);
}

void GraphicsWidget::cattail(int i)
{
    GLUquadricObj* catsegments = gluNewQuadric();
    GLUquadricObj* catdisks = gluNewQuadric();

    // Outer ring and disks
    changeMaterial(&catMaterial);
    gluCylinder(catsegments, segwidth-((i+1)*0.15), segwidth-(i*0.15), segthickness, 25, 5);

    gluDisk(catdisks, 0, segwidth-((i+1)*0.15), 25, 10);

    glTranslatef(0.0, 0.0, segthickness);
    gluDisk(catdisks, 0, segwidth-(i*0.15), 25, 10);

    // Create centre bit
    changeMaterial(&middleMaterial);
    gluCylinder(catsegments, (segwidth-((i+1)*0.15))*0.8, (segwidth-(i*0.15))*0.8, segthickness*1.5, 25, 5);
    // De-translate
    glTranslatef(0.0, 0.0, -segthickness);
}

void GraphicsWidget::cathead()
{
    GLUquadricObj* catheadobj = gluNewQuadric();
    changeMaterial(&middleMaterial);
    gluSphere(catheadobj, segwidth, 30, 30);
}

void GraphicsWidget::caterpillar()
{
    // Start drawing caterpillar
    int segno = 0;
    float movementsquidge = 0.0;
    float movementup = 0.0;
    float movevar = 0.0;

    // Start drawing at edge of caterpillar circle path
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
        movevar = ((float) segno + movementtick)/1.5;
        movementup = (cos(movevar)+1)/2;
        movementsquidge = 0.25 * sin( movevar) + 0.75;
        glTranslatef(0.0, (segwidth/5) + (movementup*catup) , (movementsquidge*segseparate));
        glRotatef(movementsquidge*segrotate, 0.0, 1.0 ,0.0);
        this->cattail(j);
        glTranslatef(0.0, -movementup*catup, 0.0);
    }
    
    // Body
    for (int i=4; i < segtotal+4; i++)
    {
        segno++;
        movevar = ((float) segno + movementtick)/1.5;
        movementsquidge = 0.25 * sin(movevar) + 0.75;
        movementup = (cos(movevar)+1)/2;

        glTranslatef(0.0, movementup*catup, (movementsquidge*segseparate));
        glRotatef(movementsquidge*segrotate, 0.0, 1.0 ,0.0);
        this->catsegment(movevar);
        // Reset y axis
        glTranslatef(0.0, -movementup*catup, 0.0);
    }
    
    // Head   
    segno++;
    movevar = ((float) segno + movementtick)/1.5;
    movementsquidge = 0.25 * sin(movevar) + 0.75;
    movementup = (cos(movevar)+1)/2;

    glTranslatef(0.0, movementup*catup, (movementsquidge*segseparate));
    glRotatef(movementsquidge*segrotate, 0.0, 1.0 ,0.0);
    this->cathead();
    glTranslatef(0.0, -movementup*catup, 0.0);
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

	
    // Draw normal plane
    changeMaterial(&planeMaterial);
    // this->squarePlane();
    this -> cylinderPlane();

    glPushMatrix();
    this -> caterpillar();
    glPopMatrix();

    glPushMatrix();
    this -> tree(7);
    glPopMatrix();

	glLoadIdentity();
    gluLookAt(  xcamera,    ycamera,    zcamera, 
                0.0,        2.0,        0.0, 
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
    ycamera = (float)i/5;
    this->repaint();
}

void GraphicsWidget::updateCatPos()
{
    catpos = fmod(catpos + 0.05, 360.0);
    movementtick+=0.07;
    this->repaint();
}

void GraphicsWidget::updateBranches(int i)
{
    branchiterate = i;
    this->repaint();
}
