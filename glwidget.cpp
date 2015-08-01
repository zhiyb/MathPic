#include "glwidget.h"
#include <QDebug>
#include <QOpenGLFramebufferObject>
#include <QFileDialog>

//#define FSH	"stackheap.fsh"
#define FSH	"mandelbrot.fsh"
//#define FSH	"lines.fsh"
//#define FSH	"light.fsh"
//#define FSH	"alg.fsh"

#define SAVESZ	2400

GLWidget::GLWidget(QWidget *parent) : QOpenGLWidget(parent)
{
	data.vertex.push_back(QVector2D(1.0, 1.0));
	data.vertex.push_back(QVector2D(1.0, -1.0));
	data.vertex.push_back(QVector2D(-1.0, -1.0));
	data.vertex.push_back(QVector2D(-1.0, 1.0));
	data.zoom = 0;
	data.moveX = 0;
	data.moveY = 0;
	data.saving = false;

	QSurfaceFormat fmt = format();
	fmt.setSamples(0);
	setFormat(fmt);
	QSurfaceFormat::setDefaultFormat(fmt);
	setFormat(fmt);

	setFocusPolicy(Qt::StrongFocus);
	setAutoFillBackground(false);
}

void GLWidget::initializeGL()
{
	initializeOpenGLFunctions();
	qDebug() << format();

	//glEnable(GL_MULTISAMPLE);

	qDebug("Loading vertex shader...");
	GLuint vsh = loadShaderFile(GL_VERTEX_SHADER, ":/data/shaders/vertex.vsh");
	qDebug("Loading fragment shader: " FSH "...");
	GLuint fsh = loadShaderFile(GL_FRAGMENT_SHADER, ":/data/shaders/" FSH);
	if (vsh == 0 || fsh == 0)
		return;
	data.program = glCreateProgram();
	glAttachShader(data.program, vsh);
	glAttachShader(data.program, fsh);
	//glBindFragDataLocation(data.program, 0, "fragColor");

	qDebug() << "Linking program...";
	glLinkProgram(data.program);

	int logLength;
	glGetProgramiv(data.program, GL_INFO_LOG_LENGTH, &logLength);
	char log[logLength];
	glGetProgramInfoLog(data.program, logLength, &logLength, log);
	qWarning(log);

	data.loc.vertex = glGetAttribLocation(data.program, "vertex");
	data.loc.projection = glGetUniformLocation(data.program, "projection");
	data.loc.zoom = glGetUniformLocation(data.program, "zoom");
	data.loc.move = glGetUniformLocation(data.program, "move");
	glEnableVertexAttribArray(data.loc.vertex);
	glUseProgram(data.program);

	data.fbo = new QOpenGLFramebufferObject(SAVESZ, SAVESZ);
}

void GLWidget::resizeGL(int w, int h)
{
	glViewport(0, 0, w, h);
	float asp = (float)h / (float)w;
	data.projection.setToIdentity();
	data.projection.ortho(-1, 1, -asp, asp, -1, 1);
}

void GLWidget::render()
{
	glUniformMatrix4fv(data.loc.projection, 1, GL_FALSE, data.projection.constData());
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	glFinish();
}

void GLWidget::paintGL()
{
	glUniform1f(data.loc.zoom, data.zoom);
	glUniform2f(data.loc.move, data.moveX, data.moveY);
	glVertexAttribPointer(data.loc.vertex, 2, GL_FLOAT, GL_FALSE, 0, data.vertex.constData());

	render();
	if (!data.saving)
		return;
	data.saving = false;

	data.fbo->bind();
	resizeGL(SAVESZ, SAVESZ);
	render();
	data.fbo->release();
	data.img = data.fbo->toImage();
	resizeGL(width(), height());

	QLabel *l = new QLabel;
	l->setPixmap(QPixmap::fromImage(data.img));
	l->show();

	QString file = QFileDialog::getSaveFileName(this, "Save to...", QString(), "PNG file (*.png)");
	if (file.isEmpty())
		return;
	data.img.save(file, "png");
}

void GLWidget::mousePressEvent(QMouseEvent *e)
{
	data.prevPos = e->pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent *e)
{
	QPointF p = e->pos() - data.prevPos;
	data.moveX += 1024.f * -p.x() * 2.f / pow(1.1, data.zoom) / (float)width();
	data.moveY += 1024.f * p.y() * 2.f / pow(1.1, data.zoom) / (float)width();
	data.prevPos = e->pos();
	update();
}

void GLWidget::keyPressEvent(QKeyEvent *e)
{
	switch (e->key()) {
	case 's':
	case 'S':
		qDebug() << "Save...";
		data.saving = true;
		update();
		e->accept();
		break;
	};
}

void GLWidget::wheelEvent(QWheelEvent *e)
{
	float zoom = (float)e->angleDelta().y() / 120.;
	data.zoom += zoom;
	qDebug() << "Zooming level: " << pow(1.1, data.zoom);
	update();
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

	int logLength;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
	char log[logLength];
	glGetShaderInfoLog(shader, logLength, &logLength, log);
	qWarning(log);
	if (status == GL_TRUE)
		return shader;
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
