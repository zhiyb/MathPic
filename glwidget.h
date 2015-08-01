#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QtWidgets>
#include <QOpenGLFramebufferObject>
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
	void keyPressEvent(QKeyEvent *e);

private:
	//void saveImage(void);
	void render(void);
	void loadShaders(QString file);
	GLuint loadShader(GLenum type, const QByteArray& context);
	GLuint loadShaderFile(GLenum type, QString path);

	struct data_t {
		struct loc_t {
			GLuint vertex, projection;
			GLuint zoom, move;
		} loc;
		GLuint program, fsh, vsh;
		GLfloat zoom;
		GLfloat moveX, moveY;
		QPoint prevPos;
		QVector<QVector2D> vertex;
		QMatrix4x4 projection;
		QOpenGLFramebufferObject *fbo;
		QImage img;
		bool saving;
		int currentFile, nextFile;
		QString filePath;
	} data;
	static const char *fileList[];
};

#endif // GLWIDGET_H
