#include "qc_simulatorqt.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QC_simulatorQt w;
	w.show();
	return a.exec();
}
