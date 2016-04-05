#ifndef PLAYLIST_ITEM_H
#define PLAYLIST_ITEM_H

#include <QWidget>
#include <QtGui>
#include <string>

class playlist_item : public QListWidgetItem
{

public:
	playlist_item(int node);
	int get_playlist_name();

private:
	int playlist;
};

#endif