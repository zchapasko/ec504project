#ifndef GUI_H
#define GUI_H

#include <QWidget>
#include <QtGui>
#include <QLabel>
#include <QPushButton>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <list>
#include <string>
#include <sstream>

#include "playlist_item.h"

class gui : public QWidget
{
	Q_OBJECT

public:
	gui(QWidget *parent = 0);

private slots:
	void help_button_handler();
	void search_button_handler();
	void add_playlist_button_handler();
	void generate_main_page();
	void generate_help_page();
	void generate_add_page();
	void help_back_button_handler();
	void update_suggestions(const QString &);
	void update_playlists();
	void update_songs();
	void print_song_info();
	void add_search_song_button_handler();
	void add_load_file_button_handler();
	void add_save_playlist_button_handler();
	void add_back_playlist_button_handler();
	void add_remove_selected_song_button_handler();
	void add_song_button_handler();
	void add_update_suggestions(const QString &);
	void add_remove_all_song_button_handler();

private:
	QStackedWidget* window;
	QVBoxLayout *window_layout;
	QWidget* main_page;
	QWidget* help_page;
	QWidget* add_page;

	// Main Layout
	QGridLayout *main_layout;

	QPushButton *help_button;
	QPushButton *search_button;
	QPushButton *add_playlist_button;

	QLabel *playlist_label;
	QLabel *songs_label;
	QLabel *current_label;
	QLabel *current_song_label;

	QCompleter *completer;
	QLineEdit *search_bar;
	QStringListModel *word_model;

	QListWidget *playslists;
	QListWidget *songs;

	// Help Layout
	QGridLayout *help_layout;

	QLabel *help_text;
	QPushButton *help_back_button;

	// Add Playlist Layout
	QGridLayout *add_layout;
	
	QLabel *add_playlist_by_song_name_label;
	QLabel *add_playlist_by_file_name_label;
	QLabel *add_selected_song_label;
	QLabel *add_current_playlist;
	QLabel *add_popularity_label;

	QPushButton *add_search_song_button;
	QPushButton *add_load_file_button;
	QPushButton *add_save_playlist_button;
	QPushButton *add_back_playlist_button;
	QPushButton *add_remove_selected_song_button;
	QPushButton *add_remove_all_song_button;
	QPushButton *add_song_button;

	QCompleter *add_completer;
	QLineEdit *add_search_song;
	QStringListModel *add_word_model;
	QLineEdit *add_load_file;
	QLineEdit *add_popularity;

	QListWidget *add_selected_songs_list;
};

#endif