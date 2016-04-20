/****************************************************************************
** Meta object code from reading C++ file 'gui.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "gui.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'gui.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_gui[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      21,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
       5,    4,    4,    4, 0x08,
      27,    4,    4,    4, 0x08,
      51,    4,    4,    4, 0x08,
      81,    4,    4,    4, 0x08,
     102,    4,    4,    4, 0x08,
     123,    4,    4,    4, 0x08,
     143,    4,    4,    4, 0x08,
     170,    4,    4,    4, 0x08,
     198,    4,    4,    4, 0x08,
     217,    4,    4,    4, 0x08,
     232,    4,    4,    4, 0x08,
     250,    4,    4,    4, 0x08,
     283,    4,    4,    4, 0x08,
     314,    4,    4,    4, 0x08,
     349,    4,    4,    4, 0x08,
     384,    4,    4,    4, 0x08,
     426,    4,    4,    4, 0x08,
     452,    4,    4,    4, 0x08,
     484,    4,    4,    4, 0x08,
     521,    4,    4,    4, 0x08,
     538,    4,    4,    4, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_gui[] = {
    "gui\0\0help_button_handler()\0"
    "search_button_handler()\0"
    "add_playlist_button_handler()\0"
    "generate_main_page()\0generate_help_page()\0"
    "generate_add_page()\0help_back_button_handler()\0"
    "update_suggestions(QString)\0"
    "update_playlists()\0update_songs()\0"
    "print_song_info()\0add_search_song_button_handler()\0"
    "add_load_file_button_handler()\0"
    "add_save_playlist_button_handler()\0"
    "add_back_playlist_button_handler()\0"
    "add_remove_selected_song_button_handler()\0"
    "add_song_button_handler()\0"
    "add_update_suggestions(QString)\0"
    "add_remove_all_song_button_handler()\0"
    "load_save_info()\0load_playlist_info()\0"
};

void gui::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        gui *_t = static_cast<gui *>(_o);
        switch (_id) {
        case 0: _t->help_button_handler(); break;
        case 1: _t->search_button_handler(); break;
        case 2: _t->add_playlist_button_handler(); break;
        case 3: _t->generate_main_page(); break;
        case 4: _t->generate_help_page(); break;
        case 5: _t->generate_add_page(); break;
        case 6: _t->help_back_button_handler(); break;
        case 7: _t->update_suggestions((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 8: _t->update_playlists(); break;
        case 9: _t->update_songs(); break;
        case 10: _t->print_song_info(); break;
        case 11: _t->add_search_song_button_handler(); break;
        case 12: _t->add_load_file_button_handler(); break;
        case 13: _t->add_save_playlist_button_handler(); break;
        case 14: _t->add_back_playlist_button_handler(); break;
        case 15: _t->add_remove_selected_song_button_handler(); break;
        case 16: _t->add_song_button_handler(); break;
        case 17: _t->add_update_suggestions((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 18: _t->add_remove_all_song_button_handler(); break;
        case 19: _t->load_save_info(); break;
        case 20: _t->load_playlist_info(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData gui::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject gui::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_gui,
      qt_meta_data_gui, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &gui::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *gui::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *gui::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_gui))
        return static_cast<void*>(const_cast< gui*>(this));
    return QWidget::qt_metacast(_clname);
}

int gui::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 21)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 21;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
