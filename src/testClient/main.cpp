#include "testclient.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	testClient w;
	w.show();
	return a.exec();
}
