#include "FNN.h"
#include <QtWidgets/QApplication>

#undef qtmain
int main(int argc, char *argv[])
{
	
	QApplication a(argc, argv);
	FNN w;
	w.showMaximized();
	return a.exec();
}
