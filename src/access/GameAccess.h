#ifndef TP4_TOWERDEFENSE_GAMEACCESS_H
#define TP4_TOWERDEFENSE_GAMEACCESS_H

#include <gtkmm.h>
#include "AccessGameHandler.h"
#include "../common/Thread.h"

class GameAccess : public Thread {
private:
    Gtk::Window *pWindow;
    std::vector<GameWindowHandler*> gameWindowHandlers;
    Gtk::ComboBoxText *pcmbMapas = nullptr;
    Gtk::Button *pbtnJugar = nullptr;
    Gtk::Button *pBtnCrearPartida = nullptr;

public:

    void run();

    void setMaps(const std::vector<std::string> &maps);

    void on_btnJugar_clicked(Glib::ustring s);

    void on_btnCrearPartida_clicked(Glib::ustring s);

    void on_cmbMapas_changed();

    bool isCmbMapsLoaded();
};


#endif //TP4_TOWERDEFENSE_GAMEACCESS_H
