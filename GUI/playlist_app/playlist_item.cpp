#include "playlist_item.h"

playlist_item::playlist_item(int node) : QListWidgetItem()
{
	playlist = node;
}

int playlist_item::get_playlist_name()
{
	return playlist;
}