#include "testclient.h"
#include "wstart.h"
#include "wtest.h"

testClient::testClient(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	wStart * widgetStart = new wStart(this);
	ui.stackedWidget->addWidget(widgetStart);

	wTest *widgetTest = new wTest(this);
	ui.stackedWidget->addWidget(widgetTest);

	ui.stackedWidget->setCurrentIndex(0);

	connect(widgetStart, SIGNAL(inputOver(QString,qint32,QString,QString)), 
		this, SLOT(startTest(QString,qint32,QString,QString)));
	connect(widgetStart, SIGNAL(inputOver(QString, qint32, QString, QString)),
		widgetTest, SLOT(startTest(QString, qint32, QString, QString)));
}

testClient::~testClient()
{
//  no need this. the widget will be delete by testClient was destroy.
// 	for (int i = 0; i < ui.stackedWidget->count(); i++)
// 	{
// 		delete ui.stackedWidget->widget(i);
// 	}	
}

void testClient::startTest(QString host, qint32 port, QString user, QString pwd)
{
	ui.stackedWidget->setCurrentIndex(1);
}