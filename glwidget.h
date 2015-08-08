#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QtWidgets>
#include <QOpenGLFramebufferObject>
#include <QOpenGLFunctions_4_0_Core>
#include "save.h"

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions_4_0_Core
{
	Q_OBJECT
public:
	explicit GLWidget(QWidget *parent = 0);

signals:
	void titleUpdate(QString str);

protected:
	void initializeGL();
	void resizeGL(int w, int h);
	void paintGL();
	void wheelEvent(QWheelEvent *e);
	void mousePressEvent(QMouseEvent *e);
	void mouseMoveEvent(QMouseEvent *e);
	void keyPressEvent(QKeyEvent *e);

private slots:
	void startRender();

private:
	void updateTitle();
	void render();
	void save();
	void loadShaders(QString file);
	GLuint loadShader(GLenum type, const QByteArray& context);
	GLuint loadShaderFile(GLenum type, QString path);

	struct data_t {
		struct save_t {
			QSize blockSize, blockCount;
		} save;
		struct loc_t {
			GLint vertex, projection;
			GLint zoom, position;
			GLint dim, animation;
		} loc;
		GLuint program, fsh, vsh;
		GLfloat zoom;
		GLdouble moveX, moveY;
		QPoint prevPos;
		QVector<QVector2D> vertex;
		QMatrix4x4 projection;
		QImage img;
		bool saving;
		int currentFile, nextFile;
		QString filePath;
	} data;
	Save *saveDialog;
	static const char *fileList[];
};

#endif // GLWIDGET_H
