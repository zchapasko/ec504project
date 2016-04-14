#ifndef TO_BE_DEFINED_H
#define TO_BE_DEFINED_H

//Other Functions
#include <list>
#include <string>
#include <sstream>
#include <stdlib.h>

//Helper function 
std::string int_to_string(int value)
{
	std::stringstream out;
	out << value;
	return out.str();
}

/* Return a list of playlists based on song name. Playlists should be the
 * eight most popular based on that song name. If an empty string is entered
 * the function should return the eight most popular playlists overall.
 
void get_top_playlists(std::string song_name,  std::list<int> *items)
{
	srand(time(NULL));
	song_name = "hello";
	for(int ii = 0; ii<8; ii++)
	{
		items->push_back(rand() % 1026);
	}
}*/

/* Return a list of songs based on the prefix. Songs should be the four
 * most popular songs with the provided prefix. If an empty string is 
 * entered, no songs should be returned.
 
void get_new_suggestions(std::string prefix, std::list<std::string> *items)
{
	srand(time(NULL));
	for(int ii = 0; ii < 4; ii++)
	{
		std::stringstream out;
		out << rand()%100;
		items->push_back(prefix + out.str());
	}
}
*/
/* Check to see whether a song is valid or not.
 
bool verify_is_song(std::string song)
{
 	if(song.length() <= 1)
 		return false;
 	else
		return true;
}*/

/* Return a list of songs based on the selected playlist.
 
void get_songs(int playlist, std::list<int> *items)
{
	srand(time(NULL));
	for(int ii = 0; ii<rand()%20+1; ii++)
	{
		items->push_back(rand() % (playlist+100));
	}
}*/


void load_songs_from_file(std::string file_name)
{
	int k;
	if(file_name.length() > 2)
	{
	
		k = 1;
	}
	if (k == 1)
	{
		k = 2;
	}
}

void get_top_eight(std::list<playlist_obj*> *items)
{
	qDebug() << "Returning top eight!" << items->size();
}


bool save_new_playlist(std::list<song_obj*> *items, int popularity)
{
	if ((int)items->size() >= 0 && popularity >= 0)
		return true;
	else
		return false;
}

void generate_error_window(int error_number)
{
	error_number+=1;
}

#endif