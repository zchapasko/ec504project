#include "PlaylistList.h"

PlaylistList::PlaylistList(){}

PlaylistList::PlaylistList(string filename){
    // set min popularity high so that it can be updated as nodes are added
    min_popularity = 100;

    // place dummy node into list so that insert performs correctly
    //song_obj temp();
    //songs_ds;

    //PlaylistList list_test("day00.txt");

    playlist_obj temp;
    temp.popularity = 0;
    temp.songs.push_back(&songs_ds[0]);
    ll.insert(ll.begin(),temp);

    // add real playlists here
    //insert_playlists(filename);
}

bool PlaylistList::insert_playlist(list_node node_to_add){
    if(ll.size() >= MAX_PLAYLISTS && int(node_to_add.popularity) < min_popularity){
        return false;
    }

    list<list_node>::iterator it;

    for(it = ll.begin(); it != ll.end(); ++it){
        if(int(it -> popularity) <= int(node_to_add.popularity)){
            ll.insert(it, node_to_add);
            it = --(ll.end());
            min_popularity = int(it -> popularity);
            return true;
        }
    }

    cout << "insert into linked list fell through all cases, error" << endl;
    return false;
}

bool PlaylistList::delete_playlists(){
    if(ll.size() <= MAX_PLAYLISTS){
        return false;
    }

    int nodes_to_delete = ll.size() - MAX_PLAYLISTS;
    while(nodes_to_delete > 0){
        --nodes_to_delete;
        ll.erase(--ll.end());
    }

    min_popularity = int((--ll.end()) -> popularity);
    return true;
}

void PlaylistList::insert_playlists(string filename){
    // file reading variables
    list_node temp;
    size_t first, second;
    string line;
    ifstream playlist_file(filename);

    // test printing variables
    // vector<int>::iterator it;

    if (playlist_file.is_open()){
        while (getline(playlist_file, line)){
            first = 0;
            second = line.find(" ");
            while(second!=string::npos){
                (temp.playlist).push_back(stoi(line.substr(first, second - first), nullptr, 10)); // add song
                first = second + 1;
                second = line.find(" ", first);
            }
            first++;
            temp.popularity = stoi(line.substr(first), nullptr, 10); // set popularity

            // test printing here
            // for(it = temp.playlist.begin(); it != temp.playlist.end(); ++it){
            //     cout << *it << " ";
            // }
            // cout << "   " << int(temp.popularity) << endl;

            insert_playlist(temp);  // insert playlist
            temp.playlist.clear();  // clear song list to prepare for overwrite
        }
        playlist_file.close();
        delete_playlists();
    } else {
        cout << "Error reading file!" << endl;
    }
}

int PlaylistList::get_min_pop(){
    return min_popularity;
}

int PlaylistList::get_size(){
    return int(ll.size());
}

vector<list_node> PlaylistList::get_top_eight(bool print){
    int ii;
    list<list_node>::iterator it = ll.begin();
    vector<list_node> top_eight;

    for(ii = 0; ii < 8; ii++, it++){
        top_eight.push_back(*it);
    }

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

    return top_eight;
}

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
