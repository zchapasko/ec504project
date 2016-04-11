#include "Trie.h"

trie_node* trie_obj::makeNode()
{
    trie_node *node = new trie_node;
 	
    node->finish = NULL;
    for(int i = 0; i < ASCII_SIZE; i++)
    {
        node->children[i] = NULL;
    }
	for(int i = 0; i < NUM_POPULAR_SONGS; i++)
	{
		node->topSongs[i] = NULL;
	}
 
    return node;
}

trie_obj::trie_obj()
{
    this->root = makeNode();
    this->count = 0;
}

void trie_obj::insertTrieNode(song_obj *song)
{  
	this->count++;
    trie_node *node = this->root;
  
	string name = song->name;
	int length = name.length();
    for(int level = 0; level < length; level++)
    {
        if(!node->children[name[level]])
        {
            node->children[name[level]] = makeNode();
        }
		node->insertTopSong(song);
        node = node->children[name[level]];
    }
 
    node->finish = song;
}

void trie_node::insertTopSong(song_obj *song)
{
	song_obj *temp = NULL;
	for(int i = 0; i < NUM_POPULAR_SONGS; i++)
	{
		//if top song doesn't exist
		if(!(this->topSongs[i]))
		{
			this->topSongs[i] = song;
			break;
		}
		else if(this->topSongs[i]->popularity < song->popularity)
		{
			temp = this->topSongs[i];
			this->topSongs[i] = song;
			song = temp;
		}
	}
}