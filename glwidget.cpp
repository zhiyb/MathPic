#include "glwidget.h"
#include <QDebug>
#include <QOpenGLFramebufferObject>
#include <QFileDialog>

#define RESPFX	":/data/shaders/"

#define SAVESZ	2400

const char *GLWidget::fileList[] = {
	RESPFX "mandelbrot.fsh",
	RESPFX "mandelbrot2.fsh",
	RESPFX "julia.fsh",
	RESPFX "stackheap.fsh",
	RESPFX "lines.fsh",
	RESPFX "light.fsh",
	RESPFX "alg.fsh",
	0
};

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
	data.currentFile = 0;
	data.nextFile = 0;
	data.vsh = 0;
	data.fsh = 0;
	data.program = 0;

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

	data.program = glCreateProgram();
	loadShaders(fileList[data.currentFile]);
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

void GLWidget::loadShaders(QString file)
{
	if (data.fsh) {
		glDeleteShader(data.fsh);
		glDetachShader(data.program, data.fsh);
	}

	if (!data.vsh) {
		qDebug("Loading vertex shader...");
		data.vsh = loadShaderFile(GL_VERTEX_SHADER, RESPFX "vertex.vsh");
	}
	qDebug(QString("Loading fragment shader: %1...").arg(file).toLocal8Bit());
	data.fsh = loadShaderFile(GL_FRAGMENT_SHADER, file);
	if (data.vsh == 0 || data.fsh == 0)
		return;

	glAttachShader(data.program, data.vsh);
	glAttachShader(data.program, data.fsh);

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
}

void GLWidget::paintGL()
{
	if (data.currentFile != data.nextFile) {
		if (data.nextFile == -1)
			loadShaders(data.filePath);
		else
			loadShaders(fileList[data.nextFile]);
		data.currentFile = data.nextFile;
		data.zoom = 0;
		data.moveX = 0;
		data.moveY = 0;
	}
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
	case 'q':
	case 'Q':
		qApp->quit();
		break;
	case 's':
	case 'S':
		e->accept();
		qDebug() << "Save...";
		data.saving = true;
		update();
		break;
	case Qt::Key_Up:
	case Qt::Key_Left:
		e->accept();
		qDebug() << "Previous...";
		if (data.nextFile > 0) {
			data.nextFile--;
			update();
		}
		break;
	case Qt::Key_Down:
	case Qt::Key_Right:
		e->accept();
		qDebug() << "Next...";
		if (fileList[data.nextFile + 1] != 0) {
			data.nextFile++;
			update();
		}
		break;
	case 'o':
	case 'O':
		e->accept();
		data.filePath = QFileDialog::getOpenFileName(this, "Open shader file...", QString(), "Fragment shader (*.fsh)");
		if (data.filePath.isEmpty())
			break;
		data.nextFile = -1;
		data.currentFile = 0;	// Set as different value for forced loading
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

GLuint GLWidget::loadShaderFile(GLenum type, QString path)
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
