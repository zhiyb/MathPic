#include "glwidget.h"
#include "save.h"
#include <QDebug>
#include <QOpenGLFramebufferObject>
#include <QFileDialog>
#include <QDialog>
#include <QVBoxLayout>

#define RESPFX	":/data/shaders/"

const char *GLWidget::fileList[] = {
	RESPFX "mandelbrot.fsh",
	RESPFX "mandelbrot2.fsh",
	RESPFX "julia.fsh",
	RESPFX "spiral.fsh",
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
	saveDialog = 0;

	QSurfaceFormat fmt = format();
	fmt.setSamples(0);
	fmt.setVersion(4, 0);
	fmt.setOption(QSurfaceFormat::DeprecatedFunctions);
	fmt.setProfile(QSurfaceFormat::CompatibilityProfile);
	fmt.setDepthBufferSize(0);
	fmt.setStencilBufferSize(0);
	fmt.setAlphaBufferSize(0);
	fmt.setSamples(0);
	setFormat(fmt);
	QSurfaceFormat::setDefaultFormat(fmt);
	setFormat(fmt);

	setFocusPolicy(Qt::StrongFocus);
	setAutoFillBackground(false);

	saveDialog = new Save;
	saveDialog->setAttribute(Qt::WA_DeleteOnClose, false);
	saveDialog->close();
	connect(saveDialog, SIGNAL(startRender()), this, SLOT(startRender()));
}

void GLWidget::initializeGL()
{
	initializeOpenGLFunctions();
	qDebug() << format();

	glDisable(GL_MULTISAMPLE);
	glDisable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_STENCIL_TEST);
	glDisable(GL_SCISSOR_TEST);

	data.program = glCreateProgram();
	loadShaders(fileList[data.currentFile]);
	updateTitle();
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

void GLWidget::selectRegion(QRect rect, QSize size)
{
	float w = size.width(), h = size.height(), asp = h / w;
	data.projection.setToIdentity();
	data.projection.ortho(-1. + 2. * rect.left() / w,
			      -1. + 2. * rect.right() / w,
			      -asp + 2. * asp * rect.top() / h,
			      -asp + 2. * asp * rect.bottom() / h, -1., 1.);
}

void GLWidget::save()
{
	saveDialog->show();
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
	data.loc.animation = glGetUniformLocation(data.program, "animation");
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
		updateTitle();
	}
	glUniform1f(data.loc.zoom, data.zoom);
	glUniform2d(data.loc.move, data.moveX, data.moveY);
	glVertexAttribPointer(data.loc.vertex, 2, GL_FLOAT, GL_FALSE, 0, data.vertex.constData());

	if (data.loc.animation != -1) {
		glUniform1f((float)data.loc.animation, QTime::currentTime().msec() / 1000.);
		update();
	}

	render();
	if (!data.saving)
		return;
	data.saving = false;

	QOpenGLFramebufferObject fbo(data.save.blockSize);
	fbo.bind();
	resizeGL(data.save.blockSize.width(), data.save.blockSize.height());
render:
	QRect region;
	region.setLeft(data.save.position.x() * data.save.blockSize.width());
	region.setWidth(data.save.blockSize.width());
	region.setTop(data.save.position.y() * data.save.blockSize.height());
	region.setHeight(data.save.blockSize.height());
	selectRegion(region, data.save.totalSize());
	render();
	bool done = data.save.position.y() == data.save.blockCount.height() - 1 &&
			data.save.position.x() == data.save.blockCount.width() - 1;
	saveDialog->addImage(region.topLeft(), fbo.toImage(), done);
	if (!done) {
		if (data.save.position.x() != data.save.blockCount.width() - 1)
			data.save.position.setX(data.save.position.x() + 1);
		else {
			data.save.position.setX(0);
			data.save.position.setY(data.save.position.y() + 1);
		}
		goto render;
	}
	fbo.release();
	resizeGL(width(), height());
}

void GLWidget::mousePressEvent(QMouseEvent *e)
{
	data.prevPos = e->pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent *e)
{
	QPointF p = e->pos() - data.prevPos;
	data.moveX += -p.x() * 2.f / pow(1.1, data.zoom) / (double)width();
	data.moveY += p.y() * 2.f / pow(1.1, data.zoom) / (double)width();
	data.prevPos = e->pos();
	updateTitle();
	update();
}

void GLWidget::keyPressEvent(QKeyEvent *e)
{
	const float moveTh = 10;
	switch (e->key()) {
	case 'r':	// Refresh
	case 'R':
		break;
	case 'q':	// Quit
	case 'Q':
	case Qt::Key_Escape:
		qApp->quit();
		return;
	case 's':	// Render & save
	case 'S':
		e->accept();
		saveDialog->show();
		saveDialog->raise();
		return;
	case '<':	// Previous shader
	case ',':
		e->accept();
		qDebug() << "Previous...";
		if (data.nextFile > 0) {
			data.nextFile--;
			update();
		}
		return;
	case '>':	// Next shader
	case '.':
		e->accept();
		qDebug() << "Next...";
		if (fileList[data.nextFile + 1] != 0) {
			data.nextFile++;
			update();
		}
		return;
	case 'o':	// Open shader
	case 'O':
		e->accept();
		data.filePath = QFileDialog::getOpenFileName(this, "Open shader file...", QString(),
							     "Fragment shader (*.fsh)");
		if (data.filePath.isEmpty())
			break;
		data.nextFile = -1;
		data.currentFile = 0;	// Set as different value for forced loading
		return;
	case Qt::Key_Up:
		data.moveY += -moveTh * 2.f / pow(1.1, data.zoom) / (double)width();
		break;
	case Qt::Key_Down:
		data.moveY += moveTh * 2.f / pow(1.1, data.zoom) / (double)width();
		break;
	case Qt::Key_Left:
		data.moveX += moveTh * 2.f / pow(1.1, data.zoom) / (double)width();
		break;
	case Qt::Key_Right:
		data.moveX += -moveTh * 2.f / pow(1.1, data.zoom) / (double)width();
		break;
	case '+':	// Zoom in
	case '=':
		data.zoom += 1.;
		break;
	case '-':	// Zoom out
	case '_':
		data.zoom -= 1.;
		break;
	};
	updateTitle();
	update();
}

void GLWidget::startRender()
{
	data.save.blockSize = QSize(saveDialog->spBlockSize[0]->value(), saveDialog->spBlockSize[1]->value());
	data.save.blockCount = QSize(saveDialog->spBlockCount[0]->value(), saveDialog->spBlockCount[1]->value());
	data.save.position = QPoint(0, 0);
	data.saving = true;
	update();
}

void GLWidget::updateTitle()
{
	QString file = data.currentFile == -1 ? data.filePath : fileList[data.currentFile];
	emit titleUpdate(tr("MathPic <(%1, %2) * %3> - %4")
			 .arg(data.moveX / 1024.).arg(data.moveY / 1024.).arg(pow(1.1, data.zoom)).arg(file));
}

void GLWidget::wheelEvent(QWheelEvent *e)
{
	float zoom = (float)e->angleDelta().y() / 120.;
	data.zoom += zoom;
	updateTitle();
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


QSize GLWidget::data_t::save_t::totalSize()
{
	return QSize(blockSize.width() * blockCount.width(), blockSize.height() * blockCount.height());
}
