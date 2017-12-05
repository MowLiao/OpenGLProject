#include <GL/glu.h>
#include <QGLWidget>
#include <QSlider>
#include <math.h>
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include "GraphicsWidget.h"
#include <QString>
#include <stdlib.h>

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
                                            { 0.2, 0.4, 0.1, 1.0},
                                            { 0.3, 0.3 , 0.2, 1.0},
                                            { 0.02, 0.15, 0.08, 0.5},
                                            89
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
                                            { 0.9, 0.95, 0.4, 1.0},
                                            { 0.02, 0.15, 0.08, 0.5},
                                            89
                                        };


static materialStruct trunkMaterial = 
                                        {
                                            { 99./255., 86./255., 63./255., 1.0},
                                            { 145./255., 114./255., 62./255., 0.7},
                                            { 145./255., 129./255., 93./255., 0.2},
                                            1
                                        };

static materialStruct leafMaterial = 
                                        {
                                            { 89./255., 137./255., 82./255., 1.0},
                                            { 58./255., 86./255., 53./255., 0.7},
                                            { 23./255., 124./255., 7./255., 0.2},
                                            40
                                        };

static materialStruct baseMaterial = 
                                        {
                                            { 1., 1,, 1,, 0.5},
                                            { 1., 1., 1,, 1.0},
                                            { 1., 1., 1., 0.4},
                                            60 
                                        };

static materialStruct orbMaterial = 
                                        {
                                            { 0.2, 0.0, 0.2, 0.5},
                                            { 0.1, 0.0, 0.1, 1.0},
                                            { 0.15, 0.0, 0.15, 0.4},
                                            60 
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
    ycamera = 10;
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

    branchangle = 90;

    earthpos = 0.0;
    zoom = 35.0;

    this->getPPMTexture("tex/marc.ppm", marcimage, marctexture);
    this->getPPMTexture("tex/earth.ppm", earthimage, earthtexture);
    this->getPPMTexture("tex/ground.ppm", groundimage, groundtexture);

}


// called when OpenGL context is set up
void GraphicsWidget::initializeGL()
{
    GLfloat r = float(166)/255.0;
    GLfloat g = float(206)/255.0;
    GLfloat b = float(241)/255.0; 
	// set the widget background colour
	glClearColor(r, g, b, 0.0);
    glEnable(GL_NORMALIZE);
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
    float scale = 35;
    glOrtho(-w/scale, w/scale, -h/scale, h/scale, -100, 100);
    //gluPerspective(45.0, (float)w / (float)h, 1, 200.0);
}

void GraphicsWidget::getPPMTexture(char* filename, QImage &image, GLubyte* &texture)
{

    image = QImage(QString(filename), "PPM");
    int width = image.width();
    int height = image.height();
    std::cout << "importing: " << filename << "\n";
    std::cout << width << " " << height << "\n";
    texture = new GLubyte[width * height * 3];
    unsigned int incr=0;

    for (int i=0; i < width; i++)
    {
        for (int j=0; j< height; j++)
        {
            incr = 3 * j * height + 3 * i;
            QRgb color = image.pixel(i, height-1-j); 
            texture[incr] = qRed(color);
            texture[incr +1] = qGreen(color);
            texture[incr+2] = qBlue(color);
        }
    }
}

void GraphicsWidget::changeMaterial(materialStruct* materialName)
{
    materialStruct* p_front = materialName;
    
    glMaterialfv(GL_FRONT, GL_AMBIENT,    p_front->ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,    p_front->diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,   p_front->specular);
    glMaterialf(GL_FRONT, GL_SHININESS,   p_front->shininess);
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
    glEnable(GL_TEXTURE_2D);

    GLUquadric* texQuadric = gluNewQuadric();
    gluQuadricTexture(texQuadric, GL_TRUE);

    glTexImage2D(   GL_TEXTURE_2D, 
                    0, GL_RGB, 
                    this->groundimage.width(), 
                    this->groundimage.height(), 
                    0, 
                    GL_RGB, 
                    GL_UNSIGNED_BYTE, 
                    this->groundtexture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 

    glPushMatrix();
    glRotatef(270, 1.0, 0.0, 0.0);
    glTranslatef(0.0, 0.0, -5.0);
    GLUquadricObj* plane = gluNewQuadric();
    gluCylinder(plane, 12.0, 12.0, 5.0, 50, 10);
    GLUquadric* disk = gluNewQuadric();

    gluDisk(texQuadric, 0, 12.0, 50, 50);
    glTranslatef(0.0, 0.0, 5.0);
    gluDisk(texQuadric, 0, 12.0, 50, 50);
    glPopMatrix();


    glDisable(GL_TEXTURE_2D);
}

void GraphicsWidget::earth(float size)
{
    float bob = 0.25;
    float ypos = this->earthypos;
    float rotate = this->earthpos;

    glEnable(GL_TEXTURE_2D);

    GLUquadric* texQuadric = gluNewQuadric();

    gluQuadricTexture(texQuadric, GL_TRUE);
    gluQuadricNormals(texQuadric, GLU_SMOOTH);

    // // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    // // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 

    // glTexImage2D(   GL_TEXTURE_2D, 
    //                 0, GL_RGB, 
    //                 this->earthimage.width(), 
    //                 this->earthimage.height(), 
    //                 0, 
    //                 GL_RGB, 
    //                 GL_UNSIGNED_BYTE, 
    //                 this->earthtexture);
    glDisable(GL_TEXTURE_2D);

    glTranslatef(0.0, bob * sin(ypos * PI/180), 0.0);
    glRotatef(rotate, 0.0, 1.0, 0.0);
    changeMaterial(&baseMaterial);
    gluSphere(texQuadric, size, 30, 30);
    glDisable(GL_TEXTURE_2D);
    glRotatef(-rotate, 0.0, 1.0, 0.0);
    glTranslatef(0.0, bob * -sin(ypos * PI/180), 0.0);


}

void GraphicsWidget::leaves()
{
    GLfloat normals[3] =  {0,0,-0.12};

    glScalef(15,15,15);
    for (int i=0; i<5; i++)
    {
        glRotatef(65, 1.0, 0.0, 0.0);
        changeMaterial(&leafMaterial);
        glNormal3fv(normals);
        glBegin(GL_POLYGON);
            glVertex3f( 0.0,  0.0, 0.0);
            glVertex3f( -0.2, 0.1, 0.0);
            glVertex3f( -0.15, 0.4, 0.0);

            glVertex3f( 0.0, 0.6, 0.0);

            glVertex3f( 0.15, 0.4, 0.0);
            glVertex3f( 0.2, 0.1, 0.0);
            glVertex3f( 0.0,  0.0, 0.0);
        glEnd();
        glRotatef(-65, 1.0, 0.0, 0.0);
        glRotatef(72, 0.0, 1.0, 0.0);
    }
    glScalef(1.0/15.0, 1.0/15.0, 1.0/15.0);

}

void GraphicsWidget::branch(    int iterate,
                                float treeheight, 
                                float rotate,
                                float point1,
                                float point2,
                                float point3)
{
    iterate = iterate-1;
    rotate = rotate / GOLDEN_RATIO;

    GLfloat normals[3][3] =  {  {(-treeheight*point3)-(-treeheight*point3),
                                (-point1*point3)-(point1*point3),
                                (-point1*-treeheight)-(point1*-treeheight)},

                                {(-treeheight*point2)-(-treeheight*point3),
                                (point1*point2)-(0*point3),
                                (point1*-treeheight)-(0*-treeheight)},

                                {(-treeheight*point3)-(-treeheight*point2),
                                (0*point3)-(-point1*point2),
                                (0*-treeheight)-(-point1*-treeheight)}
                            };

    glScalef(1/GOLDEN_RATIO,1/GOLDEN_RATIO,1/GOLDEN_RATIO);
    glTranslatef(0.0, treeheight, 0.0);

    for (int i=0; i<3; i++)
    {
        glRotatef(rotate, 1.0, 0.0, 0.0);

        changeMaterial(&trunkMaterial);
        glNormal3fv(normals[0]);
        glBegin(GL_POLYGON);
            glVertex3f( 0.0,  treeheight, 0.0);
            glVertex3f( -1, 0.0, -0.56);
            glVertex3f( 1, 0.0, -0.56);
        glEnd();

        glNormal3fv(normals[1]);
        glBegin(GL_POLYGON);
            glVertex3f( 0.0,  treeheight, 0.0);
            glVertex3f( 1, 0.0, -0.56);
            glVertex3f( 0.0, 0.0, 1.12);
        glEnd();

        glNormal3fv(normals[2]);
        glBegin(GL_POLYGON);
            glVertex3f( 0.0,  treeheight, 0.0);
            glVertex3f( 0.0, 0.0, 1.12);
            glVertex3f( -1, 0.0, -0.56);
        glEnd();

        if (iterate > 0)
        {
            glRotatef(rotate, 0.0, 1.0, 0.0);
            this -> branch(iterate, treeheight, rotate, point1, point2, point3);
            glRotatef(rotate, 0.0, -1.0, 0.0);
        }

        if (iterate == 0)
        {
            glTranslatef(0.0, treeheight*0.8, 0.0);
            this -> leaves();
            glTranslatef(0.0, -treeheight*0.8, 0.0);
        }

        glRotatef(-rotate, 1.0, 0.0, 0.0);
        glRotatef(120, 0.0, 1.0, 0.0);
    }

    glTranslatef(0.0, -treeheight, 0.0);
    glScalef(GOLDEN_RATIO,GOLDEN_RATIO,GOLDEN_RATIO);
}

void GraphicsWidget::tree(int iterate)
{
    // Draw in counter clockwise
    //  base:
    //  (-0.50, -0.28)
    //  ( 0.50, -0.28)
    //  ( 0.00,  0.56)
    float treeheight = 12.0;
    float rotate = 120;
    float point1 = 1;
    float point2 = 1.12;
    float point3 = -0.56;

    changeMaterial(&trunkMaterial);
    GLfloat normals[3][3] =  {  {(-treeheight*point3)-(-treeheight*point3),
                                (-point1*point3)-(point1*point3),
                                (-point1*-treeheight)-(point1*-treeheight)},

                                {(-treeheight*point2)-(-treeheight*point3),
                                (point1*point2)-(0*point3),
                                (point1*-treeheight)-(0*-treeheight)},

                                {(-treeheight*point3)-(-treeheight*point2),
                                (0*point3)-(-point1*point2),
                                (0*-treeheight)-(-point1*-treeheight)}
                            };


    glNormal3fv(normals[0]);
    glBegin(GL_POLYGON);
        glVertex3f( 0.0,  treeheight, 0.0);
        glVertex3f( -point1, 0.0, point3);
        glVertex3f( point1, 0.0, point3);
    glEnd();

    glNormal3fv(normals[1]);
    glBegin(GL_POLYGON);
        glVertex3f( 0.0,  treeheight, 0.0);
        glVertex3f( point1, 0.0, point3);
        glVertex3f( 0.0, 0.0, point2);
    glEnd();

    glNormal3fv(normals[2]);
    glBegin(GL_POLYGON);
        glVertex3f( 0.0,  treeheight, 0.0);
        glVertex3f( 0.0, 0.0, point2);
        glVertex3f( -point1, 0.0, point3);
    glEnd();

    this -> branch(iterate, treeheight, rotate, point1, point2, point3);
    float newrotate = rotate/GOLDEN_RATIO;
    glRotatef(newrotate, 0.0, 1.0, 0.0);
    this -> branch(iterate-1, treeheight+1, 75, point1, point2, point3);
    newrotate = newrotate/GOLDEN_RATIO;
    glRotatef(newrotate, 0.0, 1.0, 0.0);
    this -> branch(iterate-2, treeheight+1, 60, point1, point2, point3);

    // Draw earth
    glTranslatef(0.0, treeheight + 10.0, 0.0);
    this -> earth(3);
    glTranslatef(0.0, -(treeheight + 10.0), 0.0);

}

void GraphicsWidget::catsegment(float movevar)
{
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
    glEnable(GL_TEXTURE_2D);

    GLUquadric* texQuadric = gluNewQuadric();
    gluQuadricTexture(texQuadric, GL_TRUE);

    glTexImage2D(   GL_TEXTURE_2D, 
                    0, GL_RGB, 
                    this->marcimage.width(), 
                    this->marcimage.height(), 
                    0, 
                    GL_RGB, 
                    GL_UNSIGNED_BYTE, 
                    this->marctexture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 

    glRotatef(-120, 1.0, 0.0, 0.0);
    glRotatef(-30, 0.0, 0.0, 1.0);

    changeMaterial(&catMaterial);
    gluSphere(texQuadric, segwidth, 30, 30);
    glDisable(GL_TEXTURE_2D);

    glRotatef(30, 0.0, 0.0, 1.0);
    glRotatef(120, 1.0, 0.0, 0.0);


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
    this -> tree(5);
    glPopMatrix();

    // glDrawPixels(marcimage.width(),marcimage.height(),GL_RGB, GL_UNSIGNED_BYTE, this->marctexture);

    //glDrawPixels(earthimage.height(),earthimage.width(),GL_RGB, GL_UNSIGNED_BYTE, this->earthtexture);


	glLoadIdentity();
    gluLookAt(  xcamera,    ycamera,    zcamera, 
                0.0,        8.0,        0.0, 
                0.0,        1.0,        0.0);
	
	// flush to screen
	glFlush();	

}


void GraphicsWidget::updateXZAngle(int i)
{
    float in = (float) i;
    this->xcamera = 5 * sin(in * PI/180);
    this->zcamera = 5 * cos(in * PI/180);
    this->repaint();
}


void GraphicsWidget::updateYAngle(int i)
{
    this->ycamera = (float)i/5;
    this->repaint();
}

void GraphicsWidget::updatePos()
{
    this->catpos = fmod(catpos + 0.05, 360.0);
    this->movementtick+=0.07;
    this->earthpos = fmod(earthpos - 0.1, 360.0);
    this->earthypos = fmod(earthypos + 1, 360.0);
    this->repaint();
}

void GraphicsWidget::updateZoom(int i)
{
    this->zoom = (float) i;
    std::cout << zoom << "\n";
    this->repaint();
}
