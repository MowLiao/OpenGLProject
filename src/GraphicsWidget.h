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
		void updateAngle(int i);
        void updateArmAngle(int i);

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

	void plane();
	void segment(int);
	GLUquadricObj* psegment;

	float segrotate;
	float segseparate;
	float squiggles;

	int angle;
	int larmangle;
	int rarmangle;

	}; // class GLPolygonWidget
	
#endif
