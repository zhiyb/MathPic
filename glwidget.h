#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QtWidgets>
#include <QOpenGLFunctions>

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
public:
	explicit GLWidget(QWidget *parent = 0);

protected:
	void initializeGL();
	void resizeGL(int w, int h);
	void paintGL();
	void wheelEvent(QWheelEvent *e);
	void mousePressEvent(QMouseEvent *e);
	void mouseMoveEvent(QMouseEvent *e);

private:
	GLuint loadShader(GLenum type, const QByteArray& context);
	GLuint loadShaderFile(GLenum type, const char *path);

	struct data_t {
		struct loc_t {
			GLuint vertex, zoom, move;
		} loc;
		GLuint program;
		GLfloat zoom;
		QVector2D move;
		QPoint prevPos;
		QVector<QVector2D> vertex;
	} data;
};

#endif // GLWIDGET_H
