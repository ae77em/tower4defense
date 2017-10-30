#ifndef TP4_ACCESS_TOWERDEFENSE_MODELCOLUMNS_H
#define TP4_ACCESS_TOWERDEFENSE_MODELCOLUMNS_H


#include <gtkmm.h>

class ComboColumns : public Gtk::TreeModel::ColumnRecord{
private:
    Gtk::TreeModelColumn<int> id;
    Gtk::TreeModelColumn<Glib::ustring> value;

public:
    ComboColumns();
    virtual ~ComboColumns();

    Gtk::TreeModelColumn<int> &getId();

    Gtk::TreeModelColumn<Glib::ustring> &getValue();
};


#endif //TP4_ACCESS_TOWERDEFENSE_MODELCOLUMNS_H
