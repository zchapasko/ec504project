#include <fstream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

#define ASCII_SIZE (128)
#define NUM_POPULAR_SONGS (4)
#define NUM_POPULAR_PLAYLISTS (8)
#define NUM_PLAYLISTS (5504)
#define NUM_SONGS (3168)


struct song_obj;
struct playlist_obj
{
	int popularity;
	vector<song_obj*> songs;
};
struct song_obj 
{
	string name;
	string artist;
	int popularity;
	int index;
	playlist_obj *topPlaylists[NUM_POPULAR_PLAYLISTS];
};
struct trie_node
{
    song_obj *finish; //not NULL if this node is the last node in a song name
    trie_node *children[ASCII_SIZE]; //pointers to other nodes
	song_obj *topSongs[NUM_POPULAR_SONGS]; //pointers to songs in song array
};
struct trie_obj
{
    trie_node *root;
    int count;
};
trie_node* makeNode(void)
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

void initialize(trie_obj *trie)
{
    trie->root = makeNode();
    trie->count = 0;
}

void insertTopSong(trie_node *node, song_obj *song)
{
	song_obj *temp = NULL;
	for(int i = 0; i < NUM_POPULAR_SONGS; i++)
	{
		//if top song doesn't exist
		if(!(node->topSongs[i]))
		{
			node->topSongs[i] = song;
			break;
		}
		else if(node->topSongs[i]->popularity < song->popularity)
		{
			temp = node->topSongs[i];
			node->topSongs[i] = song;
			song = temp;
		}
	}
}

void insertTopPlaylist(song_obj *song, playlist_obj *playlist)
{
	playlist_obj *temp = NULL;
	for(int i = 0; i < NUM_POPULAR_PLAYLISTS; i++)
	{
		//if top playlist doesn't exist
		if(!(song->topPlaylists[i]))
		{
			song->topPlaylists[i] = playlist;
			break;
		}
		else if(song->topPlaylists[i]->popularity < playlist->popularity)
		{
			temp = song->topPlaylists[i];
			song->topPlaylists[i] = playlist;
			playlist = temp;
		}
	}
}
 
void insertTrieNode(trie_obj *trie, song_obj *song)
{  
	trie->count++;
    trie_node *node = trie->root;
  
	string name = song->name;
	int length = name.length();
    for(int level = 0; level < length; level++)
    {
        if(!node->children[name[level]])
        {
            node->children[name[level]] = makeNode();
        }
		insertTopSong(node, song);
        node = node->children[name[level]];
    }
 
    node->finish = song;
}

int main() 
{  
	string line;
	song_obj songs[NUM_SONGS];
	playlist_obj playlists[NUM_PLAYLISTS];
	int index = 0;

	//build song array line by line
	ifstream songFile("C:\\Users\\Alex\\Dropbox\\EC504_Advanced_Data_Structures_Spring2016\\Project\\PlayListApp\\PlaylistApp-Datasets\\song_list.txt");
	while(getline(songFile, line))
	{
		//make line into stream (format: index \t name \t artist)
		stringstream   songStream(line);
		
		string junk; //index can be predicted so not necessary
		string name;
		string artist;

		getline(songStream, junk,'\t');
		getline(songStream, name,'\t');
		getline(songStream, artist,'\t');

		songs[index].name = name;
		songs[index].artist = artist;
		songs[index].popularity = 0;
		songs[index].index = index;
		for(int i = 0; i < NUM_POPULAR_PLAYLISTS; i++)
		{
			songs[index].topPlaylists[i] = NULL;
		}
		
		index++;
	}

	//build popularity
	index = 0;
	ifstream playlistFile("C:\\Users\\Alex\\Dropbox\\EC504_Advanced_Data_Structures_Spring2016\\Project\\PlayListApp\\PlaylistApp-Datasets\\all_playlists.txt");
	while(getline(playlistFile, line))
	{
		//make line into stream (format: space delimited song indices \t popularity)
		stringstream playlistStream(line);

		//get space delimited song indices
		string playlistSongs;
		getline(playlistStream, playlistSongs,'\t');
		
		//get playlist popularity
		int popularity;
		playlistStream >> popularity;

		//make stream from space delimited song indices
		stringstream songsStream(playlistSongs);

		playlists[index].popularity = popularity;
		//for each song, increase popularity by playlist popularity
		int songIndex;
		while (songsStream >> songIndex)
		{
			song_obj *songPointer = &songs[songIndex];
			insertTopPlaylist(songPointer, &playlists[index]);
			if (index == 1024)
			{
				int test = 0;
			}
			playlists[index].songs.push_back(songPointer);
			songs[songIndex].popularity += popularity;
		}	
		
		index++;
	}

	//build trie
	trie_obj trie;
	initialize(&trie);
	for (int i = 0; i < NUM_SONGS; i++)
	{
		insertTrieNode(&trie, &songs[i]);	
	}

	return 0;
}