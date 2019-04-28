#include "form.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	form w;
    w.setWindowTitle("Coriolis-force");
	w.show();

	return a.exec();
}
