#ifndef TRIE_H
#define TRIE_H

#include "Song.h"

#define ASCII_SIZE (128)
#define NUM_POPULAR_SONGS (4)

class trie_node
{
public:
	song_obj *finish; //not NULL if this node is the last node in a song name
	trie_node *children[ASCII_SIZE]; //pointers to other nodes
	song_obj *topSongs[NUM_POPULAR_SONGS]; //pointers to songs in song array

	void insertTopSong(song_obj *song);
};
class trie_obj
{

public:
	trie_node *root;
	int count;

	trie_obj();
	trie_node* makeNode();

	void insertTrieNode(song_obj *song);

};

#endif
