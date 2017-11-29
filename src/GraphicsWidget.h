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
        void updateCatPos();

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

	void squarePlane();
	void cylinderPlane();
	void segment(int);
	void tail(int);

	float segrotate;
	float segseparate;
	float segwidth;
	float segthickness;
	float movementtick;
	int segtotal;

	GLUquadricObj* catsegment;
	GLUquadricObj* catdisk;
	int catcircle;
	float catup;

	float xcamera;
	float zcamera;
	float ycamera;
	float catpos;
	//int larmangle;
	//int rarmangle;


	}; // class GLPolygonWidget
	
#endif
