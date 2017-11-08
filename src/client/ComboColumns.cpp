#include "ComboColumns.h"

ComboColumns::ComboColumns() {
    add(id);
    add(value);
}

ComboColumns::~ComboColumns() {}

Gtk::TreeModelColumn<int>& ComboColumns::getId(){
    return id;
}

Gtk::TreeModelColumn<Glib::ustring>& ComboColumns::getValue(){
    return value;
}