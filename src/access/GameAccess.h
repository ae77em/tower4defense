#ifndef TP4_TOWERDEFENSE_GAMEACCESS_H
#define TP4_TOWERDEFENSE_GAMEACCESS_H

static const char *const STR_NONE = "-none-";

#include <gtkmm.h>
#include "AccessGameHandler.h"
#include "../common/Thread.h"
#include "ComboColumns.h"
#include "../common/Socket.h"

class GameAccess : public Thread {
private:
    Gtk::Window *pWindow;
    std::vector<GameWindowHandler*> gameWindowHandlers;
    Gtk::Button *pBtnCrearPartida = nullptr;
    Gtk::Button *pbtnJugar = nullptr;
    Gtk::ComboBoxText *cmbMapsText = nullptr;
    Gtk::Entry *entryMatchName = nullptr;
    Gtk::ComboBoxText *cmbMatchesText = nullptr;

    const Socket &client;
    const std::string &host;
    const uint16_t &port;

public:
    GameAccess(const Socket &client, const std::string &host, const uint16_t &port);
    virtual ~GameAccess();

    void run();

    void addMapsToCombo(const std::vector<std::string> &maps);

    void addMatchesToCombo(const std::vector<std::string> &matches);

    void addMatchToCombo(const std::string &mapName, const std::string &matchName);

    void on_btnJugar_clicked();

    void on_btnCrearPartida_clicked();

    void on_cmbMapas_changed();

    bool isCmbMapsLoaded();

    void addMap(std::string map);

    void updateHeader();

    void dropMap(std::string map);

    void initComboMaps(Glib::RefPtr<Gtk::Builder> &refBuilder);

    void initButtonPlay(Glib::RefPtr<Gtk::Builder> &refBuilder);

    void initButtonCreateMatch(Glib::RefPtr<Gtk::Builder> &refBuilder);

    void initComboMatches(Glib::RefPtr<Gtk::Builder> &refBuilder);

    void on_cmbMatches_changed();

    void initEntryMatchName(Glib::RefPtr<Gtk::Builder> &refBuilder);

    void on_entryMatchName_changed();

private:
    void setCreateMatchButtonEnableStatus();

    bool mustCreateMatchBeEnabled();
};


#endif //TP4_TOWERDEFENSE_GAMEACCESS_H
