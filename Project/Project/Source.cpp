#include <fstream>
#include <string>
#include <sstream>
#include "Trie.h"
#include "Song.h"

using namespace std;

#define NUM_PLAYLISTS (5504)
#define NUM_SONGS (3168)

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
			songPointer->insertTopPlaylist(&playlists[index]);
			playlists[index].songs.push_back(songPointer);
			songs[songIndex].popularity += popularity;
		}	
		
		index++;
	}

	//build trie
	trie_obj trie = trie_obj();
	for (int i = 0; i < NUM_SONGS; i++)
	{
		trie.insertTrieNode(&songs[i]);	
	}

	return 0;
}