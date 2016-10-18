#ifndef WSTART_H
#define WSTART_H

#include <QWidget>
#include "ui_wstart.h"

class wStart : public QWidget
{
	Q_OBJECT

public:
	wStart(QWidget *parent = 0);
	~wStart();

private slots:
	void on_btn_Start_clicked();

private:
	void setStatusInfo(QString &str);

signals:
	void inputOver(QString, qint32, QString, QString);

private:
	Ui::wStart ui;
};

#endif // WSTART_H
