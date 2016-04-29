#ifndef TRIE_H
#define TRIE_H

#include "Song.h"
#include <list>
#include <iostream>
#include <QDebug>
#include <QString>

#define ASCII_SIZE (128)
#define NUM_POPULAR_SONGS (4)

class trie_node
{
public:
	song_obj *finish; //not NULL if this node is the last node in a song name
	trie_node *children[ASCII_SIZE]; //pointers to other nodes
	song_obj *topSongs[NUM_POPULAR_SONGS]; //pointers to songs in song array

	void insertTopSong(song_obj *song); //create list of top songs for node
	~trie_node();
};

class trie_obj
{

public:
	trie_node *root;
	int count;

	trie_obj();
	~trie_obj();
	trie_node* makeNode(); //add new node to trie

	void insertTrieNode(song_obj *song); //add song name to trie
	void get_new_suggestions(std::string prefix, std::list<std::string> *items); //get top songs at node
	song_obj* verify_is_song(std::string song); //check if song name exists in trie

};

#endif
