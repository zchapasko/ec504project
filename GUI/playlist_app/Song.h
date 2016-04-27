#ifndef SONG_H
#define SONG_H

#include <string>
#include <vector>

using namespace std;

#define NUM_POPULAR_PLAYLISTS (8)

class song_obj;
struct playlist_obj
{
	int popularity;
	vector<song_obj*> songs;
};

struct playlist_obj;

class song_obj 
{
public:
	string name;
	string artist;
	int popularity;
	int index;
	playlist_obj *topPlaylists[NUM_POPULAR_PLAYLISTS];

	void insertTopPlaylist(playlist_obj *playlist);
	void deleteTopPlaylist(playlist_obj *playlist);
};

#endif