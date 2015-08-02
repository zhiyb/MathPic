#include "save.h"

#define OUTSZ	1024

Save::Save(QWidget *parent) : QDialog(parent)
{
	img = 0;
	setModal(false);
	setWindowFlags(Qt::Window);

	QHBoxLayout *layout = new QHBoxLayout(this);

	QVBoxLayout *sLayout = new QVBoxLayout;
	layout->addLayout(sLayout);

	QGroupBox *gbBlock = new QGroupBox(tr("Block size"));
	sLayout->addWidget(gbBlock);
	QHBoxLayout *gbBlockLayout = new QHBoxLayout(gbBlock);
	gbBlockLayout->addWidget(spBlockSize[0] = new QSpinBox);
	gbBlockLayout->addWidget(new QLabel(tr("x")));
	gbBlockLayout->addWidget(spBlockSize[1] = new QSpinBox);
	for (int i = 0; i != 2; i++) {
		spBlockSize[i]->setMinimum(1);
		spBlockSize[i]->setMaximum(2048);
		spBlockSize[i]->setValue(1024);
	}

	QGroupBox *gbCount = new QGroupBox(tr("Block count"));
	sLayout->addWidget(gbCount);
	QHBoxLayout *gbCountLayout = new QHBoxLayout(gbCount);
	gbCountLayout->addWidget(spBlockCount[0] = new QSpinBox);
	gbCountLayout->addWidget(new QLabel(tr("x")));
	gbCountLayout->addWidget(spBlockCount[1] = new QSpinBox);
	for (int i = 0; i != 2; i++) {
		spBlockCount[i]->setMinimum(1);
		spBlockCount[i]->setMaximum(1000);
		spBlockCount[i]->setValue(10);
	}

	sLayout->addWidget(lFinalRes = new QLabel);

	QPushButton *pbRender = new QPushButton(tr("Render"));
	sLayout->addWidget(pbRender);

	QPushButton *pbSave = new QPushButton(tr("Save image"));
	sLayout->addWidget(pbSave);

	layout->addWidget(lwProgess = new QListWidget);

	QScrollArea *saOutput = new QScrollArea;
	saOutput->setBackgroundRole(QPalette::Dark);
	saOutput->setWidget(lOutput = new QLabel);
	layout->addWidget(saOutput, 4);

	for (int i = 0; i != 2; i++) {
		connect(spBlockSize[i], SIGNAL(valueChanged(int)), this, SLOT(updateRes()));
		connect(spBlockCount[i], SIGNAL(valueChanged(int)), this, SLOT(updateRes()));
	}
	connect(pbRender, SIGNAL(clicked(bool)), this, SLOT(render()));
	connect(pbSave, SIGNAL(clicked(bool)), this, SLOT(save()));
	updateRes();
}

Save::~Save()
{
	if (img)
		delete img;
}

void Save::updateRes()
{
	QSize res = finalRes();
	lFinalRes->setText(tr("Final resolution: %1 x %2").arg(res.width()).arg(res.height()));
}

void Save::render()
{
	if (img)
		delete img;
	lwProgess->clear();
	tStart = QTime::currentTime();
	lwProgess->addItem(tr("Rendering start at %1:%2:%3.")
			   .arg(tStart.hour()).arg(tStart.minute()).arg(tStart.second()));
	lwProgess->addItem(tr("Allocating memory..."));
	img = new QImage(finalRes(), QImage::Format_RGB888);
	if (img == 0)
		goto allocFailed;
	else if (img->isNull())
		goto nullImage;
	lwProgess->addItem(tr("QPixmap creation succeed, waiting..."));
	lOutput->resize(img->size());
	emit startRender();
	return;
nullImage:
	delete img;
	img = 0;
allocFailed:
	lwProgess->addItem(tr("QPixmap creation failed!"));
}

void Save::save()
{
	if (img == 0 || img->isNull())
		return;
	QString file = QFileDialog::getSaveFileName(this, "Save image to...", QString(), "PNG file (*.png)");
	if (file.isEmpty())
		return;
	QTime tStart = QTime::currentTime();
	if (!img->save(file))
		return;
	QTime tEnd = QTime::currentTime();
	lwProgess->addItem(tr("Image saved, time elapsed: %1s.").arg(tStart.secsTo(tEnd)));
	delete img;
	img = 0;
	lwProgess->addItem(tr("Image memory freed."));
}

void Save::addImage(QPoint pos, QImage img, bool done)
{
	lwProgess->addItem(tr("Image received at (%1,%2).").arg(pos.x()).arg(pos.y()));
	QPainter painter(this->img);
	painter.drawImage(pos, img);
	painter.end();
	if (done) {
		QImage outImg = this->img->scaled(OUTSZ, OUTSZ, Qt::KeepAspectRatio);
		lOutput->setPixmap(QPixmap::fromImage(outImg));
		lOutput->resize(outImg.size());
		QTime tEnd = QTime::currentTime();
		lwProgess->addItem(tr("Rendering finished at %1:%2:%3.")
				   .arg(tEnd.hour()).arg(tEnd.minute()).arg(tEnd.second()));
		lwProgess->addItem(tr("Time elapsed: %1s.").arg(tStart.secsTo(tEnd)));
	}
}

QSize Save::finalRes()
{
	return QSize(spBlockSize[X]->value() * spBlockCount[X]->value(),
		     spBlockSize[Y]->value() * spBlockCount[Y]->value());
}
