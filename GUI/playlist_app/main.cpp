
#include <QApplication>
#include "gui.h"

int main(int argc, char *argv[])
{
	// Create Application
    QApplication app(argc, argv);

    // Create Window and Set Window Size and Name
    gui window;
    window.setWindowTitle("504 Playlist App");
    window.resize(500,700);
    window.setMaximumWidth(600);
    window.setMinimumWidth(400);
    window.setMaximumHeight(800);
    window.setMinimumHeight(600);

    // Show Window
    window.show();

    return app.exec();
}