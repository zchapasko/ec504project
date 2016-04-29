#include "PlaylistList.h"

PlaylistList::PlaylistList(string filename, song_obj *songs_ds){
    // set min popularity high so that it can be updated as nodes are added
    min_popularity = 100;

    // get pointer to song array for updating purposes
    this -> songs_ds = songs_ds; 

    //place dummy node into list so that insert performs correctly
    playlist_obj temp;
    temp.popularity = 0;
    ll.insert(ll.begin(),temp);

    // add real playlists here
    insert_playlists(filename);
}

bool PlaylistList::insert_playlist(playlist_obj node_to_add){
    // if the playlist doesn't meet minimum popularity cutoff just exit
    if(ll.size() >= MAX_PLAYLISTS && node_to_add.popularity < min_popularity){
        return false;
    }

    list<playlist_obj>::iterator it;

    for(it = ll.begin(); it != ll.end(); ++it){
        if(it -> popularity <= node_to_add.popularity){

            // insert playlist
            ll.insert(it, node_to_add);

            // get pointer to point to playlist that was just added
            --it;

            // update song array here
            vector<song_obj*> temp_songs = it -> songs;
            vector<song_obj*>::iterator jj;
            for(jj = temp_songs.begin(); jj != temp_songs.end(); ++jj){
                (*jj) -> insertTopPlaylist(&(*it));
                songs_ds[(*jj) -> index].popularity += it -> popularity;
            }

            // update minimum popularity
            it = --(ll.end());
            min_popularity = it -> popularity;
            return true;
        }
    }

    // should never reach here
    qDebug() << "insert into linked list fell through all cases, error" << endl;
    return false;
}

bool PlaylistList::delete_playlists(){
    // if list contains fewer than max playlists, don't delete
    if(ll.size() <= MAX_PLAYLISTS){
        return false;
    }

    list<playlist_obj>::iterator ii;
    vector<song_obj*> temp;
    vector<song_obj*>::iterator jj;

    // delete playlists one by one from end of list
    int nodes_to_delete = ll.size() - MAX_PLAYLISTS;
    while(nodes_to_delete > 0){
        --nodes_to_delete;
        ii = --(ll.end());

        // update songs_ds here
        temp = ii -> songs;
        for(jj = temp.begin(); jj != temp.end(); ++jj){
            (*jj) -> deleteTopPlaylist(&(*ii)); // new line
            songs_ds[(*jj) -> index].popularity -= ii -> popularity;
        }

        // erase element
        ll.erase(ii);
    }

    // update minimum popularity
    min_popularity = (--ll.end()) -> popularity;
    return true;
}

bool PlaylistList::insert_playlists(string filename){
    // file reading variables
    playlist_obj temp;
    size_t first, second;
    string line;
    ifstream playlist_file(filename.c_str());
    int index;

    // test printing variables
    // vector<song_obj*>::iterator it;

    if (playlist_file.is_open()){
        while (getline(playlist_file, line)){
            first = 0;
            second = line.find(" ");
            while(second!=string::npos){
                index = atoi(line.substr(first, second - first).c_str()); // get song index
                temp.songs.push_back(&songs_ds[index]); // add song
                first = second + 1;
                second = line.find(" ", first);
            }
            first++;
            temp.popularity = atoi(line.substr(first).c_str()); // add popularity

            // test printing here
            // for(it = temp.songs.begin(); it != temp.songs.end(); ++it){
            //     qDebug() << (*it) -> index << " ";
            // }
            // qDebug() << "   " << temp.popularity << endl;

            insert_playlist(temp);  // insert playlist
            temp.songs.clear();  // clear song list to prepare for temp variable overwrite
        }
        playlist_file.close();
        delete_playlists(); // call delete to clean up list if list has more than maximum allowed playlists
        return true;
    } else {
        cout << "Error reading file!" << endl;
        return false;
    }
}

int PlaylistList::get_min_pop(){
    return min_popularity;
}

int PlaylistList::get_size(){
    return int(ll.size());
}

vector<playlist_obj*> PlaylistList::get_top_eight(bool print){
    int ii;
    list<playlist_obj>::iterator it = ll.begin();
    vector<playlist_obj*> top_eight;

    // populate vector of 8 playlist objects from list
    for(ii = 0; ii < 8; ii++, it++){
        top_eight.push_back(&(*it));
    }
/*
    if(print){
        vector<list_node>::iterator ii;
        vector<int>::iterator jj;
        vector<int> temp;

        for(ii = top_eight.begin(); ii != top_eight.end(); ++ii){
            temp = ii -> playlist;
            for(jj = temp.begin(); jj != temp.end(); ++jj){
                cout << *jj << " ";
            }
            cout << "   " << int(ii -> popularity) << endl;
        }
    }
*/
    return top_eight;
}
/*
void PlaylistList::print(){
    list<list_node>::iterator ii;
    vector<int>::iterator jj;
    vector<int> temp;

    for(ii = ll.begin(); ii != ll.end(); ++ii){
        temp = ii -> playlist;
        for(jj = temp.begin(); jj != temp.end(); ++jj){
            cout << *jj << " ";
        }
        cout << "   " << int(ii -> popularity) << endl;
    }
}
*/
