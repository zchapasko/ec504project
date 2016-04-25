#ifndef PLAYLISTLIST_H
#define PLAYLISTLIST_H

#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <list>
#include "Song.h"
#include "gui.h"
#include "Trie.h"
#include <cstdlib>

using namespace std;

#define ASCII_SIZE (128)
#define NUM_POPULAR_SONGS (4)
#define NUM_POPULAR_PLAYLISTS (8)
#define NUM_PLAYLISTS (5504)
#define NUM_SONGS (3168)
#define MAX_PLAYLISTS (1024)

// struct list_node
// {
// 	vector<int> playlist;
// 	unsigned char popularity;
// };

class PlaylistList {
public:
	PlaylistList(string filename, song_obj *songs_ds);					// constructs list from file
	bool insert_playlist(playlist_obj node_to_add);	// inserts a single playlist
	bool insert_playlists(string filename);			// insert playlists from file
	bool delete_playlists();						// deletes enough playlists so ll.size() <= MAX_PLAYLISTS
	int get_min_pop();								// returns minimum popularity of all playlists in list
	int get_size();									// returns actual length of list object
	vector<playlist_obj*> get_top_eight(bool print);	// returns top 8 playlists; if print is true, also prints them
	void print();									// prints list contents
private:
	PlaylistList();									// constructs empty list; disallow default constructor
	list<playlist_obj> ll;							// list of playlists
	int min_popularity;								// keeps track of lowest popularity score in the list
	song_obj *songs_ds;								// pointer to song array
};

#endif