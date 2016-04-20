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
        if(!node->children[(int)name[level]])
        {
            node->children[(int)name[level]] = makeNode();
        }
		node->insertTopSong(song);
        node = node->children[(int)name[level]];
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

void trie_obj::get_new_suggestions(std::string prefix, std::list<std::string> *items)
{
	trie_node *node = this->root;
	int ii;

	for(ii = 0; ii < (int)prefix.length(); ii++)
	{
		if(NULL != node->children[(int)prefix[ii]])
		{
			node = node->children[(int)prefix[ii]];
		}
		else break;
	}
	if(ii == (int)prefix.length())
	{
		for(int jj = 0; jj<NUM_POPULAR_SONGS; jj++)
		{
			if(node->topSongs[jj] != NULL)
			{	
				items->push_back(node->topSongs[jj]->name);
			}
			else break;
		}
	}
}

/* Check to see whether a song is valid or not.
 */
song_obj* trie_obj::verify_is_song(std::string song)
{
 	trie_node *node = this->root;
	int ii;

	for(ii = 0; ii < (int)song.length(); ii++)
	{
		if(NULL != node->children[(int)song[ii]])
		{
			node = node->children[(int)song[ii]];
		}
		else break;
	}
	if(ii == (int)song.length())
	{
		return node->finish;
	}
	return NULL;
}

trie_obj::~trie_obj()
{
	delete root; 
}

trie_node::~trie_node()
{
	for(int i = 0; i < ASCII_SIZE; i++)
    {
        delete children[i];
    }
}