#include "Song.h"


//add top 8 most popular playlists for song
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
		//bubble down playlists if this playlist is more popular
		else if(this->topPlaylists[i]->popularity < playlist->popularity)
		{
			temp = this->topPlaylists[i];
			this->topPlaylists[i] = playlist;
			playlist = temp;
		}
	}
}

//if a playlist is removed from the top 1024, this removes it from the song's top 8 (if it exists)
void song_obj::deleteTopPlaylist(playlist_obj *playlist)
{
	for(int i = 0; i < NUM_POPULAR_PLAYLISTS; i++)
	{
		//if top playlist doesn't exist
		if(!(this->topPlaylists[i]))
		{
			break;
		}
		else if(this->topPlaylists[i] == playlist)
		{
			this->topPlaylists[i]=NULL;
		}
	}
}