#include "glwidget.h"
#include <QDebug>

GLWidget::GLWidget(QWidget *parent) : QOpenGLWidget(parent)
{
	data.vertex.push_back(QVector2D(1.0, 1.0));
	data.vertex.push_back(QVector2D(1.0, -1.0));
	data.vertex.push_back(QVector2D(-1.0, -1.0));
	data.vertex.push_back(QVector2D(-1.0, 1.0));
	data.zoom = 0;

	QSurfaceFormat fmt = format();
	fmt.setSamples(4);
	setFormat(fmt);
	QSurfaceFormat::setDefaultFormat(fmt);
	setFormat(fmt);
}

void GLWidget::initializeGL()
{
	initializeOpenGLFunctions();
	qDebug() << format();

	glEnable(GL_MULTISAMPLE);

	qDebug("Loading vertex shader...");
	GLuint vsh = loadShaderFile(GL_VERTEX_SHADER, ":/shaders/vertex.vsh");
	qDebug("Loading fragment shader...");
	GLuint fsh = loadShaderFile(GL_FRAGMENT_SHADER, ":/shaders/stackheap.fsh");
	if (vsh == 0 || fsh == 0)
		return;
	data.program = glCreateProgram();
	glAttachShader(data.program, vsh);
	glAttachShader(data.program, fsh);
	glLinkProgram(data.program);
	glUseProgram(data.program);
	data.loc.vertex = glGetAttribLocation(data.program, "vertex");
	data.loc.zoom = glGetUniformLocation(data.program, "zoom");
	data.loc.move = glGetUniformLocation(data.program, "move");
	glEnableVertexAttribArray(data.loc.vertex);
}

void GLWidget::resizeGL(int w, int h)
{
	glViewport(0, 0, w, h);
}

void GLWidget::paintGL()
{
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glUniform1f(data.loc.zoom, data.zoom);
	glUniform2fv(data.loc.move, 1, (const GLfloat *)&data.move);
	glVertexAttribPointer(data.loc.vertex, 2, GL_FLOAT, GL_FALSE, 0, data.vertex.constData());
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

void GLWidget::mousePressEvent(QMouseEvent *e)
{
	data.prevPos = e->pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent *e)
{
	QPoint p = e->pos() - data.prevPos;
	data.move += QVector2D(-p.x(), p.y());
	update();
}

void GLWidget::wheelEvent(QWheelEvent *e)
{
	float zoom = (float)e->angleDelta().y() / 120.;
	//if (data.zoom + zoom >= 0) {
		data.zoom += zoom;
		update();
	//}
}

GLuint GLWidget::loadShader(GLenum type, const QByteArray& context)
{
	GLuint shader = glCreateShader(type);
	const char *p = context.constData();
	int length = context.length();
	glShaderSource(shader, 1, &p, &length);
	glCompileShader(shader);

	int status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_TRUE)
		return shader;

	int logLength;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
	char log[logLength];
	glGetShaderInfoLog(shader, logLength, &logLength, log);
	qWarning(log);
	glDeleteShader(shader);
	return 0;
}

GLuint GLWidget::loadShaderFile(GLenum type, const char *path)
{
	QFile f(path);
	if (!f.open(QIODevice::ReadOnly)) {
		qWarning(QString("Cannot open file %1").arg(path).toLocal8Bit());
		return 0;
	}
	QByteArray context = f.readAll();
	f.close();
	return loadShader(type, context);
}
