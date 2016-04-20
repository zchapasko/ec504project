#include "gui.h"
#include "to_be_defined.h"

gui::gui(QWidget *parent) : QWidget(parent)
{
	/***** Initialize Data Structures *****/
	load_save_info();
	load_playlist_info();

	/***** Generate GUI *****/
	generate_main_page();
	generate_help_page();
	generate_add_page();

	main_page = new QWidget(this);
	main_page->setLayout(main_layout);
	help_page = new QWidget(this);
	help_page->setLayout(help_layout);
	add_page = new QWidget(this);
	add_page->setLayout(add_layout);

	window = new QStackedWidget(this);
	window->addWidget(main_page);
	window->addWidget(help_page);
	window->addWidget(add_page);

	window_layout = new QVBoxLayout;
	window_layout->addWidget(window);
	setLayout(window_layout);
}

void gui::load_playlist_info()
{
	list_of_playlists = new PlaylistList("day00.txt", songs_ds);
}

void gui::load_save_info()
{
	string line;
	int index = 0;

	//build song array line by line
	ifstream songFile("song_list.txt");
	while(getline(songFile, line))
	{
		//make line into stream (format: index \t name \t artist)
		stringstream songStream(line);
		
		string junk; //index can be predicted so not necessary
		string name;
		string artist;

		getline(songStream, junk,'\t');
		getline(songStream, name,'\t');
		getline(songStream, artist,'\t');

		songs_ds[index].name = name;
		songs_ds[index].artist = artist;
		songs_ds[index].popularity = 0;
		songs_ds[index].index = index;
		for(int i = 0; i < NUM_POPULAR_PLAYLISTS; i++)
		{
			songs_ds[index].topPlaylists[i] = NULL;
		}
		
		index++;
	}

	//build popularity
	index = 0;
	ifstream playlistFile("all_playlists.txt");
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

		playlists_ds[index].popularity = popularity;
		//for each song, increase popularity by playlist popularity
		int songIndex;
		while (songsStream >> songIndex)
		{
			song_obj *songPointer = &songs_ds[songIndex];
			songPointer->insertTopPlaylist(&playlists_ds[index]);
			playlists_ds[index].songs.push_back(songPointer);
			songs_ds[songIndex].popularity += popularity;
		}	
		index++;
	}

	//build trie
	trie = trie_obj();
	for (int i = 0; i < NUM_SONGS; i++)
	{
		trie.insertTrieNode(&songs_ds[i]);	
	}
}

void gui::generate_main_page()
{
	// Initialize Help Button
	help_button = new QPushButton("?", this);
	connect(help_button, SIGNAL(clicked()), this, SLOT(help_button_handler()));
	help_button->setFixedHeight(25);
	help_button->setFixedWidth(25);

	// Initialize Search Button
	search_button = new QPushButton("Search!", this);
	connect(search_button, SIGNAL(clicked()), this, SLOT(search_button_handler()));

	// Initialize Add Playlist Button
	add_playlist_button = new QPushButton("Add", this);
	connect(add_playlist_button, SIGNAL(clicked()), this, SLOT(add_playlist_button_handler()));

	// Choose Text Color
	QPalette text;
	text.setColor(QPalette::WindowText, Qt::white);

	// Initialize Playlist Label
	playlist_label = new QLabel(this);
	playlist_label->setFrameStyle(QFrame::NoFrame);
	playlist_label->setText("Top 8 Playlists:");
	playlist_label->setPalette(text);

	// Initialize Songs Label
	songs_label = new QLabel(this);
	songs_label->setFrameStyle(QFrame::NoFrame);
	songs_label->setText("Songs:");
	songs_label->setPalette(text);

	// Initialize Current Song Title Label
	current_label = new QLabel(this);
	current_label->setFrameStyle(QFrame::NoFrame);
	current_label->setText("Now Playing:");
	current_label->setPalette(text);

	// Initialize Current Song Label
	current_song_label = new QLabel(this);
	current_song_label->setFrameStyle(QFrame::NoFrame);
	current_song_label->setText("");
	current_song_label->setPalette(text);

	// Initialize QCompleter WordList Model
	word_model = new QStringListModel();

	// Initialize QCompleter
	completer = new QCompleter(word_model);
	completer->setCompletionMode(QCompleter::PopupCompletion);
	completer->setModelSorting(QCompleter::UnsortedModel);

	// Initialize Search Bar
	search_bar = new QLineEdit("", this);
	search_bar->setReadOnly(false);
	search_bar->setAlignment(Qt::AlignLeft | Qt::AlignTop);
	search_bar->setCompleter(completer);
	search_bar->installEventFilter(this);
	connect(search_bar, SIGNAL(textEdited(const QString &)), this, SLOT(update_suggestions(const QString &)));
	
	// Initialize Playlist List
	playslists = new QListWidget(this);
	update_playlists();

	// Initialize Songs List
	songs = new QListWidget(this);
	connect(playslists, SIGNAL(itemSelectionChanged()), this, SLOT(update_songs()));
	connect(songs, SIGNAL(itemSelectionChanged()), this, SLOT(print_song_info()));

	// Initialize Layout
	main_layout = new QGridLayout;
	main_layout->setRowMinimumHeight(4,5);
	main_layout->setRowMinimumHeight(7,5);
	main_layout->setRowMinimumHeight(1,5);

	// Add Buttons
	main_layout->addWidget(help_button, 0, 0, 1, 1);
	main_layout->addWidget(search_button, 0, 2, 1, 1);
	main_layout->addWidget(add_playlist_button, 9, 2, 1, 1);

	// Add Labels
	main_layout->addWidget(playlist_label, 2, 0, 1, -1);
	main_layout->addWidget(songs_label, 5, 0, 1, -1);
	main_layout->addWidget(current_label, 8, 0, 1, -1);
	main_layout->addWidget(current_song_label, 9, 0, 1, 2);

	// Add Search Bar
	main_layout->addWidget(search_bar, 0, 1, 1, 1);

	// Add Lists
	main_layout->addWidget(playslists, 3, 0, 1, -1);
	main_layout->addWidget(songs, 6, 0, 1, -1);
}

void gui::generate_help_page()
{
	// Initialize Help Text Label
	help_text = new QLabel(this);
	help_text->setFrameStyle(QFrame::NoFrame);
	help_text->setText("I'm here to help you");

	// Initialize Search Button
	help_back_button = new QPushButton("Back", this);
	connect(help_back_button, SIGNAL(clicked()), this, SLOT(help_back_button_handler()));

	// Initialize Layout
	help_layout = new QGridLayout;

	help_layout->addWidget(help_text, 0, 0, 1, -1);
	help_layout->addWidget(help_back_button, 10, 0, 1, -1);
}

void gui::generate_add_page()
{
	// Initialize add playlist via songs Label
	add_playlist_by_song_name_label = new QLabel(this);
	add_playlist_by_song_name_label->setFrameStyle(QFrame::NoFrame);
	add_playlist_by_song_name_label->setText("Create a playlist by selecting songs: ");

	// Initialize add playlist via file name label
	add_playlist_by_file_name_label = new QLabel(this);
	add_playlist_by_file_name_label->setFrameStyle(QFrame::NoFrame);
	add_playlist_by_file_name_label->setText("Load many playlists by file name: ");

	// Initialize add playlist via file name label
	add_selected_song_label = new QLabel(this);
	add_selected_song_label->setFrameStyle(QFrame::NoFrame);
	add_selected_song_label->setText(" - No song selected - ");

	// Initialize add playlist via file name label
	add_current_playlist = new QLabel(this);
	add_current_playlist->setFrameStyle(QFrame::NoFrame);
	add_current_playlist->setText("Current Playlist: ");

	// Initialize add popularity label
	add_popularity_label = new QLabel(this);
	add_popularity_label->setFrameStyle(QFrame::NoFrame);
	add_popularity_label->setText("Popularity: ");

	// Initialize Search Song Button
	add_song_button = new QPushButton("Add Song", this);
	connect(add_song_button, SIGNAL(clicked()), this, SLOT(add_song_button_handler()));

	// Initialize Search Song Button
	add_search_song_button = new QPushButton("Search", this);
	connect(add_search_song_button, SIGNAL(clicked()), this, SLOT(add_search_song_button_handler()));

	// Initialize Load Playlist Button
	add_load_file_button = new QPushButton("Load Playlists", this);
	connect(add_load_file_button, SIGNAL(clicked()), this, SLOT(add_load_file_button_handler()));

	// Initialize Remove Selected Song Button
	add_remove_selected_song_button = new QPushButton("Remove Selected Song", this);
	connect(add_remove_selected_song_button, SIGNAL(clicked()), this, SLOT(add_remove_selected_song_button_handler()));

	// Initialize Back w/ Saving Button
	add_save_playlist_button = new QPushButton("Save and Go Back", this);
	connect(add_save_playlist_button, SIGNAL(clicked()), this, SLOT(add_save_playlist_button_handler()));

	// Initialize Back w/o Saving Button
	add_back_playlist_button = new QPushButton("Go Back Without Saving", this);
	connect(add_back_playlist_button, SIGNAL(clicked()), this, SLOT(add_back_playlist_button_handler()));

	// Initialize Remove All Button
	add_remove_all_song_button = new QPushButton("Remove All Songs", this);
	connect(add_remove_all_song_button, SIGNAL(clicked()), this, SLOT(add_remove_all_song_button_handler()));

	// Initialize QCompleter WordList Model
	add_word_model = new QStringListModel();

	// Initialize QCompleter
	add_completer = new QCompleter(add_word_model);
	add_completer->setCompletionMode(QCompleter::PopupCompletion);
	add_completer->setModelSorting(QCompleter::UnsortedModel);

	// Initialize Song Search Bar
	add_search_song = new QLineEdit("", this);
	add_search_song->setReadOnly(false);
	add_search_song->setAlignment(Qt::AlignLeft | Qt::AlignTop);
	add_search_song->setCompleter(add_completer);
	add_search_song->installEventFilter(this);
	connect(add_search_song, SIGNAL(textEdited(const QString &)), this, SLOT(add_update_suggestions(const QString &)));

	// Initialize Load Playlist From File Bar
	add_load_file = new QLineEdit("", this);
	add_load_file->setReadOnly(false);
	add_load_file->setAlignment(Qt::AlignRight | Qt::AlignTop);

	// Initialize Popularity Input Bar
	add_popularity = new QLineEdit("0", this);
	add_popularity->setReadOnly(false);
	add_popularity->setAlignment(Qt::AlignRight | Qt::AlignTop);

	// Initialize Added Songs List
	add_selected_songs_list = new QListWidget(this);

	// Initialize Layout
	add_layout = new QGridLayout;
	add_layout->setRowMinimumHeight(3,5);
	add_layout->setRowMinimumHeight(8,5);
	add_layout->setRowMinimumHeight(10,10);

	// Add Labels to the Layout
	add_layout->addWidget(add_playlist_by_song_name_label, 0, 0, 1, -1);
	add_layout->addWidget(add_playlist_by_file_name_label, 11, 0, 1, -1);
	add_layout->addWidget(add_selected_song_label, 2, 0, 1, 2);
	add_layout->addWidget(add_current_playlist, 4, 0, 1, -1);
	add_layout->addWidget(add_popularity_label, 7, 0, 1, 1);

	// Add Buttons to the Layout
	add_layout->addWidget(add_song_button, 2, 2, 1, 1);
	add_layout->addWidget(add_search_song_button, 1, 2, 1, 1);
	add_layout->addWidget(add_load_file_button, 12, 2, 1, 1);
	add_layout->addWidget(add_remove_selected_song_button, 6, 2, 1, 1);
	add_layout->addWidget(add_save_playlist_button, 9, 2, 1, 1);
	add_layout->addWidget(add_back_playlist_button, 9, 0, 1, 1);
	add_layout->addWidget(add_remove_all_song_button, 6, 0, 1, 1);

	// Add Line Edits & List
	add_layout->addWidget(add_search_song, 1, 0, 1, 2);
	add_layout->addWidget(add_load_file, 12, 0, 1, 2);
	add_layout->addWidget(add_selected_songs_list, 5, 0, 1, -1);
	add_layout->addWidget(add_popularity, 7, 1, 1, 2);
}

void gui::help_button_handler()
{

	window->setCurrentIndex(1);
}

void gui::search_button_handler()
{
	playslists->clear();
	songs->clear();
	update_playlists();
}

void gui::add_playlist_button_handler()
{

	window->setCurrentIndex(2);
}

void gui::help_back_button_handler()
{

	window->setCurrentIndex(0);
}

void gui::update_suggestions(const QString &)
{
	QStringList word_list;
	std::list<std::string> items;
	trie.get_new_suggestions((search_bar->text()).toStdString(), &items);
	for(std::list<std::string>::iterator it = items.begin(); it != items.end(); it++)
	{
		word_list.append(QString::fromStdString(*it));
	}
	word_model->setStringList(word_list);
}

void gui::add_update_suggestions(const QString &)
{
	QStringList word_list;
	std::list<std::string> items;
	trie.get_new_suggestions((add_search_song->text()).toStdString(), &items);
	for(std::list<std::string>::iterator it = items.begin(); it != items.end(); it++)
	{
		word_list.append(QString::fromStdString(*it));
	}
	add_word_model->setStringList(word_list);
}

void gui::update_playlists()
{
	int row = 0;
	std::list<int> items;
	displayed_song = trie.verify_is_song(search_bar->text().toStdString());
	if(displayed_song != NULL)
	{	
		for(int ii = 0; ii < 8; ii++)
		{
			if(displayed_song->topPlaylists[ii] != NULL)
			{
				QListWidgetItem *next_item = new QListWidgetItem();
				std::stringstream temp;
				temp << displayed_song->topPlaylists[ii]->popularity << "    ";
				for (std::vector<song_obj*>::iterator it = displayed_song->topPlaylists[ii]->songs.begin(); it != displayed_song->topPlaylists[ii]->songs.end(); it++)
				{
					temp << (*it)->index << " ";
				}
				QString next_text = QString::fromStdString(temp.str());
				next_item->setText(next_text);
				playslists->insertItem(row++, next_item);
			}
		}
		string temp_label = "Playlists for \"" + displayed_song->name + "\":";
		playlist_label->setText(QString::fromStdString(temp_label));
		top_eight_displayed = false;
	}
	else
	{
		top_eight = list_of_playlists->get_top_eight(0);
		for(std::vector<playlist_obj*>::iterator it = top_eight.begin(); it != top_eight.end(); it++)
		{
			if(*it != NULL)
			{
				QListWidgetItem *next_item = new QListWidgetItem();
				std::stringstream temp;
				temp << (*it)->popularity << "    ";
				for (std::vector<song_obj*>::iterator it2 = (*it)->songs.begin(); it2 != (*it)->songs.end(); it2++)
				{
					temp << (*it2)->index << " ";
				}
				QString next_text = QString::fromStdString(temp.str());
				next_item->setText(next_text);
				playslists->insertItem(row++, next_item);
			}
		}
		playlist_label->setText("Top 8 Playlists:");
		top_eight_displayed = true;
	}
}

void gui::update_songs()
{
	songs->clear();
	int row = 0;
	int index = playslists->currentRow();
	if(top_eight_displayed)
	{
		for(std::vector<song_obj*>::iterator it = top_eight[index]->songs.begin(); it != top_eight[index]->songs.end(); it++)
		{
				QListWidgetItem *next_item = new QListWidgetItem();
				QString next_text = QString::fromStdString((*it)->name);
				next_item->setText(next_text);
				songs->insertItem(row++, next_item);
		}
	}
	else
	{
		if(displayed_song->topPlaylists[index] != NULL)
		{
			for(std::vector<song_obj*>::iterator it = displayed_song->topPlaylists[index]->songs.begin(); it != displayed_song->topPlaylists[index]->songs.end(); it++)
			{
				QListWidgetItem *next_item = new QListWidgetItem();
				QString next_text = QString::fromStdString((*it)->name);
				next_item->setText(next_text);
				songs->insertItem(row++, next_item);
			}
		}
	}
}

void gui::print_song_info()
{
	int p_index = playslists->currentRow();
	int s_index = songs->currentRow();
	song_obj *temp_song;
	if(top_eight_displayed)
	{
		temp_song = top_eight[p_index]->songs[s_index];
	}
	else
	{
		temp_song = displayed_song->topPlaylists[p_index]->songs[s_index];
	}
	string new_text = temp_song->name + " - by - " + temp_song->artist;
	current_song_label->setText(QString::fromStdString(new_text));
}

void gui::add_search_song_button_handler()
{
	QString song = add_search_song->text();
	add_displayed_song = trie.verify_is_song(song.toStdString());
	if(song.length() == 0)
		add_selected_song_label->setText(" - No song selected - ");
	else if(add_displayed_song != NULL)
		{
			string temp = add_displayed_song->name + " - by - " + add_displayed_song->artist;
			add_selected_song_label->setText(QString::fromStdString(temp));
		}
	else
		add_selected_song_label->setText("-- Not a Valid Song --");
}

void gui::add_song_button_handler()
{
	if (add_displayed_song != NULL)
	{
		QListWidgetItem *next_item = new QListWidgetItem();
		next_item->setText(add_selected_song_label->text());
		add_selected_songs_list->insertItem(0, next_item);
		add_songs.push_back(add_displayed_song);
	}
}

void gui::add_remove_selected_song_button_handler()
{
	for(std::vector<song_obj*>::iterator it = add_songs.begin(); it != add_songs.end(); it++)
	{
		if((*it)->name == add_selected_songs_list->currentItem()->text().toStdString())
		{
			add_songs.erase(it);
			break;
		}
	}
	delete add_selected_songs_list->takeItem(add_selected_songs_list->row(add_selected_songs_list->currentItem()));
}

void gui::add_remove_all_song_button_handler()
{

	add_selected_songs_list->clear();
	add_songs.clear();
}

void gui::add_back_playlist_button_handler()
{
	add_songs.clear();
	add_displayed_song = NULL;
	add_selected_songs_list->clear();
	add_popularity->setText("0");
	add_search_song->setText("");
	add_load_file->setText("");
	add_selected_song_label->setText(" - No song selected - ");
	window->setCurrentIndex(0);
}

void gui::add_save_playlist_button_handler()
{
	// Try getting popularity
	int popularity;
	popularity = atoi(add_popularity->text().toStdString().c_str());
	if (add_popularity->text().length() == (int)int_to_string(popularity).length() || popularity <= 0)
	{
		playlist_obj temp;
		temp.songs = add_songs;
		temp.popularity = popularity;
		// Try saving the playlist
		if (list_of_playlists->insert_playlist(temp))
		{
			add_songs.clear();
			add_displayed_song = NULL;
			add_selected_songs_list->clear();
			add_popularity->setText("0");
			add_search_song->setText("");
			add_load_file->setText("");
			add_selected_song_label->setText(" - No song selected - ");
			window->setCurrentIndex(0);
		}
		else
		{
			generate_error_window(5);
		}
	}
	else
	{
		generate_error_window(10);
	}
}

void gui::add_load_file_button_handler()
{
	
	string file_name = add_load_file->text().toStdString();
	list_of_playlists->insert_playlists(file_name);
	add_songs.clear();
	add_displayed_song = NULL;
	add_selected_songs_list->clear();
	add_popularity->setText("0");
	add_search_song->setText("");
	add_load_file->setText("");
	add_selected_song_label->setText(" - No song selected - ");
	window->setCurrentIndex(0);
	if(top_eight_displayed)
	{
		playslists->clear();
		update_playlists();
	}

	
}