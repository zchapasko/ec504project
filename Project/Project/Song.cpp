#include "Song.h"

void song_obj::insertTopPlaylist(playlist_obj *playlist)
{
	playlist_obj *temp = NULL;
	for(int i = 0; i < NUM_POPULAR_PLAYLISTS; i++)
	{
		//if top playlist doesn't exist
		if(!(this->topPlaylists[i]))
		{
			this->topPlaylists[i] = playlist;
			break;
		}
		else if(this->topPlaylists[i]->popularity < playlist->popularity)
		{
			temp = this->topPlaylists[i];
			this->topPlaylists[i] = playlist;
			playlist = temp;
		}
	}
}