#ifndef SAVE_H
#define SAVE_H

#include <QtWidgets>

class Save : public QDialog
{
	Q_OBJECT
	friend class GLWidget;
public:
	explicit Save(QWidget *parent = 0);
	~Save();

signals:
	void startRender(void);

public slots:

private slots:
	void updateRes(void);
	void render(void);
	void addImage(QPoint pos, QImage img, bool done);

private:
	QSize finalRes(void);

	enum Dim {X = 0, Y = 1};
	QTime tStart;
	QImage *img;
	QSpinBox *spBlockSize[2], *spBlockCount[2];
	QLabel *lFinalRes, *lOutput;
	QListWidget *lwProgess;
};

#endif // SAVE_H
