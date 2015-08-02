#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QtWidgets>
#include <QOpenGLFramebufferObject>
#include <QOpenGLFunctions>
#include "save.h"

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
	Q_OBJECT
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

private slots:
	void startRender(void);

private:
	void render(void);
	void selectRegion(QRect rect, QSize size);
	void save(void);
	void loadShaders(QString file);
	GLuint loadShader(GLenum type, const QByteArray& context);
	GLuint loadShaderFile(GLenum type, QString path);

	struct data_t {
		struct save_t {
			QSize totalSize();
			QSize blockSize, blockCount;
			QPoint position;
		} save;
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
		QImage img;
		bool saving;
		int currentFile, nextFile;
		QString filePath;
	} data;
	Save *saveDialog;
	static const char *fileList[];
};

#endif // GLWIDGET_H
