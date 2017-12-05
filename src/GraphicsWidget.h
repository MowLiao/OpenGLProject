#ifndef __GL_POLYGON_WIDGET_H__
#define __GL_POLYGON_WIDGET_H__ 1

#include <QGLWidget>
#include <GL/glut.h>

class GraphicsWidget: public QGLWidget
	{ // 

	Q_OBJECT

	public:
		GraphicsWidget(QWidget *parent);
		~GraphicsWidget();

	public slots:
		void updateXZAngle(int i);
		void updateYAngle(int i);
        void updatePos();
        void updateZoom(int i);

	protected:
	// called when OpenGL context is set up
	void initializeGL();
	// called every time the widget is resized
	void resizeGL(int w, int h);
	// called every time the widget needs painting
	void paintGL();

	private:

	/*
	void head();
	void torso();
	void shoulders();
	void upper_arm_left();
	void lower_arm_left();

	GLUquadricObj* phead;
	GLUquadricObj* ptorso;
	GLUquadricObj* pshoulders;
	GLUquadricObj* pupperleft;
	GLUquadricObj* plowerleft;
	*/

	void changeMaterial(struct materialStruct*);
	void squarePlane();
	void cylinderPlane();

	// Texture methods and variables
	void getPPMTexture(char*, QImage&, GLubyte*&);
	QImage marcimage;
	GLubyte* marctexture;
	QImage earthimage;
	GLubyte* earthtexture;
	QImage groundimage;
	GLubyte* groundtexture;


	// Caterpillar methods and variables
	void caterpillar();
	void cathead();
	void catsegment(float);
	void cattail(int);
	float catpos;
	float catup;
	float movementtick;
	float segrotate;
	float segseparate;
	float segwidth;
	float segthickness;
	int segtotal;
	int catcircle;
	GLUquadricObj* catsegments;
	GLUquadricObj* catdisks;
	GLUquadricObj* catheadobj;

	// Tree methods and variables
	void tree(int);
	void branch(int, float, float, float, float, float);
	void leaves();
	int branchangle;

	// Earth
	void earth(float);
	float earthpos;
	float earthypos;


	float zoom;
	float xcamera;
	float zcamera;
	float ycamera;
	//int larmangle;
	//int rarmangle;



	}; // class GLPolygonWidget
	
#endif
