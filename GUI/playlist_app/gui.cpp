#include "gui.h"

// Constructor to create a playlist gui
gui::gui(QWidget *parent) : QWidget(parent)
{
	/***** Initialize Data Structures *****/
	load_song_info();
	load_playlist_info();
	build_trie();

	/***** Set Theme ******/
	text.setColor(QPalette::WindowText, Qt::white);
	win.setColor(QPalette::Window, QColor::fromRgb(84, 163, 0, 255));
	this->setPalette(win);

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

// Loads playlists into a playlistlist object
void gui::load_playlist_info()
{
	// Creates the list of playlists with day00.txt as a starting point
	list_of_playlists = new PlaylistList("day00.txt", songs_ds);
}

// Load songs into the song array
void gui::load_song_info()
{
	string line;
	int index = 0;

	// Build song array line by line
	ifstream songFile("song_list.txt");
	while(getline(songFile, line))
	{
		// Make line into stream (format: index \t name \t artist)
		stringstream songStream(line);
		
		string junk; //index can be predicted so not necessary
		string name;
		string artist;

		// Extract data
		getline(songStream, junk,'\t');
		getline(songStream, name,'\t');
		getline(songStream, artist,'\t');

		// Update song entry
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
}

// Build the Trie data structure for searching and playlist suggestions
void gui::build_trie()
{
	// Create trie and add songs
	trie = new trie_obj();
	for (int i = 0; i < NUM_SONGS; i++)
	{
		trie->insertTrieNode(&songs_ds[i]);	
	}
}

// Initialize widgets and add them to the main page
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

// Initialize widgets and add them to the help page
void gui::generate_help_page()
{
	// Initialize Help Text Label
	help_text = new QLabel(this);
	help_text->setFrameStyle(QFrame::NoFrame);
	help_text->setText("I'm here to help you");
	help_text->setPalette(text);

	// Initialize Search Button
	help_back_button = new QPushButton("Back", this);
	connect(help_back_button, SIGNAL(clicked()), this, SLOT(help_back_button_handler()));

	// Initialize Layout and Add Widgets
	help_layout = new QGridLayout;
	help_layout->addWidget(help_text, 0, 0, 1, -1);
	help_layout->addWidget(help_back_button, 10, 0, 1, -1);
}

// Initialize widgets and add them to the add playlist page
void gui::generate_add_page()
{
	// Initialize add playlist via songs Label
	add_playlist_by_song_name_label = new QLabel(this);
	add_playlist_by_song_name_label->setFrameStyle(QFrame::NoFrame);
	add_playlist_by_song_name_label->setText("Create a playlist by selecting songs: ");
	add_playlist_by_song_name_label->setPalette(text);

	// Initialize add playlist via file name label
	add_playlist_by_file_name_label = new QLabel(this);
	add_playlist_by_file_name_label->setFrameStyle(QFrame::NoFrame);
	add_playlist_by_file_name_label->setText("Load many playlists by file name: ");
	add_playlist_by_file_name_label->setPalette(text);

	// Initialize add playlist via file name label
	add_selected_song_label = new QLabel(this);
	add_selected_song_label->setFrameStyle(QFrame::NoFrame);
	add_selected_song_label->setText("-- No song selected --");
	add_selected_song_label->setPalette(text);

	// Initialize add playlist via file name label
	add_current_playlist = new QLabel(this);
	add_current_playlist->setFrameStyle(QFrame::NoFrame);
	add_current_playlist->setText("Current Playlist: ");
	add_current_playlist->setPalette(text);

	// Initialize add popularity label
	add_popularity_label = new QLabel(this);
	add_popularity_label->setFrameStyle(QFrame::NoFrame);
	add_popularity_label->setText("Popularity: ");
	add_popularity_label->setPalette(text);

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
	add_layout->addWidget(add_selected_song_label, 2, 0, 1, 4);
	add_layout->addWidget(add_current_playlist, 4, 0, 1, -1);
	add_layout->addWidget(add_popularity_label, 7, 0, 1, 2);

	// Add Buttons to the Layout
	add_layout->addWidget(add_song_button, 2, 4, 1, 2);
	add_layout->addWidget(add_search_song_button, 1, 4, 1, 2);
	add_layout->addWidget(add_load_file_button, 12, 4, 1, 2);
	add_layout->addWidget(add_remove_selected_song_button, 6, 3, 1, 3);
	add_layout->addWidget(add_save_playlist_button, 9, 3, 1, 3);
	add_layout->addWidget(add_back_playlist_button, 9, 0, 1, 3);
	add_layout->addWidget(add_remove_all_song_button, 6, 0, 1, 3);

	// Add Line Edits & List
	add_layout->addWidget(add_search_song, 1, 0, 1, 4);
	add_layout->addWidget(add_load_file, 12, 0, 1, 4);
	add_layout->addWidget(add_selected_songs_list, 5, 0, 1, -1);
	add_layout->addWidget(add_popularity, 7, 2, 1, 4);
}

// Button handler for help page button on the main page
void gui::help_button_handler()
{
	// Switch to help page
	window->setCurrentIndex(1);
}

// Search button handler for the search button on the main page
void gui::search_button_handler()
{
	// Clear both playlist and song list widgets and update the playlist list widget
	playslists->clear();
	songs->clear();
	update_playlists();
}

// Button handler for add playlist button on the main page
void gui::add_playlist_button_handler()
{
	// Switch to add page
	window->setCurrentIndex(2);
}

// Button handler for back button on the help page
void gui::help_back_button_handler()
{
	// Switch back to the main page
	window->setCurrentIndex(0);
}

// Suggestions handler, called when text changes in main page's search bar's search field
void gui::update_suggestions(const QString &)
{
	// Get new suggestions based on entered text and update the associated completer
	QStringList word_list;
	std::list<std::string> items;
	trie->get_new_suggestions((search_bar->text()).toStdString(), &items);
	for(std::list<std::string>::iterator it = items.begin(); it != items.end(); it++)
	{
		word_list.append(QString::fromStdString(*it));
	}
	word_model->setStringList(word_list);
}

// Suggestions handler, called when text changes in add page's search bar's search field
void gui::add_update_suggestions(const QString &)
{
	// Get new suggestions based on entered text and update the associated completer
	QStringList word_list;
	std::list<std::string> items;
	trie->get_new_suggestions((add_search_song->text()).toStdString(), &items);
	for(std::list<std::string>::iterator it = items.begin(); it != items.end(); it++)
	{
		word_list.append(QString::fromStdString(*it));
	}
	add_word_model->setStringList(word_list);
}

// Update the playlist list widget with new playlists
void gui::update_playlists()
{
	int row = 0;
	std::list<int> items;

	// Search for song based on search entry 
	displayed_song = trie->verify_is_song(search_bar->text().toStdString());

	// Check if the song was found
	if(displayed_song != NULL)
	{	
		// If the song was found, update the playlist list widget with that song's top eight playlists
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

		// Update the playlist labels and state variables
		string temp_label = "Playlists for \"" + displayed_song->name + "\":";
		playlist_label->setText(QString::fromStdString(temp_label));
		top_eight_displayed = false;
	}
	else
	{
		// If the song was not found, get the top eight playlists
		top_eight = list_of_playlists->get_top_eight(0);

		// Display the top eight playlists in the widget
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
		// Update playlist labels and state variables
		playlist_label->setText("Top 8 Playlists:");
		top_eight_displayed = true;
	}
}

// Update songs in the songs list widget
void gui::update_songs()
{
	// Clear the widget to prepare for new entries
	songs->clear();
	int row = 0;

	// Get the current selected playlist index
	int index = playslists->currentRow();

	// Update songs from the proper playlist vector
	// - Vector can either be the top eight playlists vector or a playlist given by the searched song
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

// Print song info when a song is selected in the song list widget
void gui::print_song_info()
{
	// Get a pointer to the song object
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

	// Display the songs name and artist
	string new_text = temp_song->name + " - by - " + temp_song->artist;
	current_song_label->setText(QString::fromStdString(new_text));
}

// Button handler for searching for songs on the add page
void gui::add_search_song_button_handler()
{
	// Get the search entry
	QString song = add_search_song->text();

	// Check to see if that song is valid, if so, get a pointer to the song object
	add_displayed_song = trie->verify_is_song(song.toStdString());

	// If the search entry is 0 characters, say no song selected
	if(song.length() == 0)
		add_selected_song_label->setText("-- No song selected --");
	// If a valid song object pointer was returned, show its name and artist
	else if(add_displayed_song != NULL)
	{
		string temp = add_displayed_song->name + " - by - " + add_displayed_song->artist;
		add_selected_song_label->setText(QString::fromStdString(temp));
	}
	// If the searched name is not valid, say so
	else
		add_selected_song_label->setText("-- Not a Valid Song --");
}

// Button handler for adding a song to the added songs list
void gui::add_song_button_handler()
{
	// Check to ensure the song is valid
	if (add_selected_song_label->text() == "-- No song selected --" || add_selected_song_label->text() == "-- Not a Valid Song --")
	{
		// If song is not valid, say so
		add_selected_song_label->setText("-- Not a Valid Song --");
	}
	// Check to ensure the added song points to something
	else if (add_displayed_song != NULL)
	{
		// If there is a song to be added, add it to the vector of added songs and the list widget
		QListWidgetItem *next_item = new QListWidgetItem();
		next_item->setText(add_selected_song_label->text());
		add_selected_songs_list->insertItem(0, next_item);
		add_songs.push_back(add_displayed_song);
	}
}

// Button handler for removing selected songs from the added songs list
void gui::add_remove_selected_song_button_handler()
{
	if(add_songs.size() > 0)
	{
		// Get index of selected song
		int index = add_selected_songs_list->currentRow();

		// Remove from back of add_songs list
		// - list widget inserts at front, add_songs vector inserts from back
		add_songs.erase(add_songs.begin()+add_songs.size()-1-index);

		// Remove object from the list widget
		delete add_selected_songs_list->takeItem(index);
	}
}

// Button handler for removing all songs from the playlist list
void gui::add_remove_all_song_button_handler()
{
	// Check to see if there are any songs in the playlist list
	if(add_songs.size() != 0 )
	{
		// If there are songs in the list, ask user if they are sure they want to remove them all
		QMessageBox messageBox;
		messageBox.setWindowTitle("Warning");
	    messageBox.setText("Are you sure you want to remove all songs?");
	    messageBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
	    messageBox.setDefaultButton(QMessageBox::No);
	   	int ret = messageBox.exec();
	   	switch(ret) {
	   		// If they want to remove all songs, do so
	   		case QMessageBox::Yes:
	   			add_selected_songs_list->clear();
				add_songs.clear();
	   			break;
	   		// If they want don't want to remove all songs, do nothing
	   		case QMessageBox::No:
				break;
	   	}
	}
	// If no songs in the list, do nothing
}

// Button handler for going back to main page from add page without saving
void gui::add_back_playlist_button_handler()
{
	// Check to see if any changes have been made to popularity or songs list
	if(add_songs.size() != 0 || (add_popularity->text() != "0" && add_popularity->text() != ""))
	{
		// If changes have been made, ask user if they really want to go back without saving
		QMessageBox messageBox;
		messageBox.setWindowTitle("Warning");
	    messageBox.setText("Unsaved Content");
	    messageBox.setInformativeText("Do you want to save your changes?");
	    messageBox.setStandardButtons(QMessageBox::Save | QMessageBox::No | QMessageBox::Cancel);
	    messageBox.setDefaultButton(QMessageBox::Save);
	   	int ret = messageBox.exec();
	   	switch(ret) {
	   		// If they want to save, try to save the playlist
	   		case QMessageBox::Save:
	   			add_save_playlist_button_handler();
	   			break;
	   		// If they want don't want to save, reset add page and return to main page
	   		case QMessageBox::No:
	   			add_songs.clear();
				add_displayed_song = NULL;
				add_selected_songs_list->clear();
				add_popularity->setText("0");
				add_search_song->setText("");
				add_load_file->setText("");
				add_selected_song_label->setText("-- No song selected --");
				window->setCurrentIndex(0);
				break;
			// If they choose cancel, do nothing and return to add page
			case QMessageBox::Cancel:
				break;
	   	}
    }
    else
    {
    	// If no popularity entered and no songs added, reset add page and return to main page
		add_songs.clear();
		add_displayed_song = NULL;
		add_selected_songs_list->clear();
		add_popularity->setText("0");
		add_search_song->setText("");
		add_load_file->setText("");
		add_selected_song_label->setText("-- No song selected --");
		window->setCurrentIndex(0);
	}
}

// Button handler for saving manually added playlists
void gui::add_save_playlist_button_handler()
{
	// Try getting popularity as an integer
	int popularity;
	popularity = atoi(add_popularity->text().toStdString().c_str());
	std::stringstream out;
	out << popularity;

	// Verify that popularity is an integer and its greater than 0
	if (add_popularity->text().length() == (int)out.str().length() && popularity >= 0)
	{
		// Create a temporary playlist object
		playlist_obj temp;
		temp.songs = add_songs;
		temp.popularity = popularity;

		// Try saving the playlist
		if (list_of_playlists->insert_playlist(temp))
		{
			// If successful, reset add page and return to main page
			add_songs.clear();
			add_displayed_song = NULL;
			add_selected_songs_list->clear();
			add_popularity->setText("0");
			add_search_song->setText("");
			add_load_file->setText("");
			add_selected_song_label->setText("-- No song selected --");
			window->setCurrentIndex(0);
			
			// Rebuild the trie to update all preprocessed values
			// - Four suggested songs
			// - Eight most popular playlists
			delete trie;
			build_trie();

			// Update the playlist widget to reflect changes
			playslists->clear();
			update_playlists();
		}
		else
		{
			// If not successful with saving playlist, show error message
			QMessageBox messageBox;
			messageBox.setWindowTitle("Error");
    		messageBox.setText("Error Adding Playlist. Playlist Not Added");
   			messageBox.exec();
		}
	}
	else
	{
		// If popularity entry is invalid, show error message
		QMessageBox messageBox;
		messageBox.setWindowTitle("Error");
    	messageBox.setText("Please Enter A Valid Popularity (Integer Greater Than or Equal to 0)");
   		messageBox.exec();
	}
}

// Button handler for loading playlists from a file
void gui::add_load_file_button_handler()
{
	// Get filename from LineEdit
	string file_name = add_load_file->text().toStdString();

	// Attempt to add playlists in the file
	if(list_of_playlists->insert_playlists(file_name))
	{
		// If successful, reset add page and return to main page
		add_songs.clear();
		add_displayed_song = NULL;
		add_selected_songs_list->clear();
		add_popularity->setText("0");
		add_search_song->setText("");
		add_load_file->setText("");
		add_selected_song_label->setText("-- No song selected --");
		window->setCurrentIndex(0);

		// Rebuild the trie to update all preprocessed values
		// - Four suggested songs
		// - Eight most popular playlists
		delete trie;
		build_trie();

		// Update the playlist widget to reflect changes
		playslists->clear();
		update_playlists();
	}
	else
	{
		// If not successful, show error message
		QMessageBox messageBox;
		messageBox.setWindowTitle("Error");
    	messageBox.setText("One Or More Playlists Not Added. Check File Name and Contents.");
   		messageBox.exec();
	}
}

// Destructer, called when closing the window
gui::~gui()
{
	// Clear all list widgets (delete their contents)
	playslists->clear();
	songs->clear();
	add_selected_songs_list->clear();

	// Delete all data structures
	delete list_of_playlists;
	delete trie;

	// Delete buttons & labels & list widgets & lineedits
	delete help_button;
	delete search_button;
	delete add_playlist_button;
	delete playlist_label;
	delete songs_label;
	delete current_label;
	delete current_song_label;
	delete word_model;
	delete completer;
	delete search_bar;
	delete playslists;
	delete songs;
	delete help_text;
	delete help_back_button;
	delete add_playlist_by_song_name_label;
	delete add_playlist_by_file_name_label;
	delete add_selected_song_label;
	delete add_current_playlist;
	delete add_popularity_label;
	delete add_song_button;
	delete add_search_song_button;
	delete add_load_file_button;
	delete add_remove_selected_song_button;
	delete add_save_playlist_button;
	delete add_back_playlist_button;
	delete add_remove_all_song_button;
	delete add_word_model;
	delete add_completer;
	delete add_search_song;
	delete add_load_file;
	delete add_popularity;
	delete add_selected_songs_list;

	// Delete layouts
	delete help_layout;
	delete add_layout;
	delete main_layout;

	// Delete the window and it's pages
	delete main_page;
	delete help_page;
	delete add_page;
	delete window_layout;
	delete window;
}


