#ifndef TP4_TOWERDEFENSE_GAMEACCESS_H
#define TP4_TOWERDEFENSE_GAMEACCESS_H

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
    Gtk::Button *pbtnUnirse = nullptr;
    Gtk::ComboBoxText *cmbMapsText = nullptr;
    Gtk::Entry *entryMatchName = nullptr;
    Gtk::ComboBoxText *cmbMatchesText = nullptr;
    Gtk::CheckButton *pchkAire = nullptr;
    Gtk::CheckButton *pchkAgua = nullptr;
    Gtk::CheckButton *pchkFuego = nullptr;
    Gtk::CheckButton *pchkTierra = nullptr;

    const Socket &client;
    const std::string &host;
    const uint16_t &port;
    int clientId;

public:
    GameAccess(const Socket &client, const std::string &host, const uint16_t &port);

    virtual ~GameAccess();

    /*
     * Inicia un nuevo thread corriendo el programa de acceso al juego.
     */
    void run();

    /*
     * Agrega los mapas pasados como parámetro al combo de mapas.
     * maps: vector de strings con los nombres de los mapas.
     */
    void addMapsToCombo(const std::vector<std::string> &maps);

    /*
     * Agrega una partida al combo de partidas.
     * map: mapa donde se jugará la partida.
     * match: nombre de la partida.
     */
    void addMatchToCombo(const std::string &mapName, const std::string &matchName);

    /*
     * Setea el id del cliente (socket) con el cual me estoy conectando al server.
     * Corresponde al FD del cliente en el server.
     * clientId: FD del cliente en el server.
     */
    void setClientId(int clientId);

    /*
     * Retorna el id del cliente (socket) con el cual me estoy conectando al server.
     * Corresponde al FD del cliente en el server.
     */
    int getClientId();

    /*
     * Retorna true si el clientId es igual a -1, que es el valor al construirse el objeto.
     * Se supone que si se conecta a un server exitosamente, ese valor cambia por el del FD
     * del cliente conectado en el server-
     */
    bool isNotValidClientId();

    /* Habilita los checkboxes correspondientes a lo elementos disponibles, para
     * poder ser seleccionados.
     * elements: listado de elementos disponibles.
     * */
    void setAvailableElements(const std::list<std::string> &elements);

    /* Inicializadores de los distintos inputs.
     * refBuilder: referencia al builder donde se están colocando los elementos.
     * */
    void initComboMaps(Glib::RefPtr<Gtk::Builder> &refBuilder);

    void initButtonPlay(Glib::RefPtr<Gtk::Builder> &refBuilder);

    void initButtonCreateMatch(Glib::RefPtr<Gtk::Builder> &refBuilder);

    void initComboMatches(Glib::RefPtr<Gtk::Builder> &refBuilder);

    void initEntryMatchName(Glib::RefPtr<Gtk::Builder> &refBuilder);

    /* Event handlers */
    void on_btnJugar_clicked();

    void on_btnCrearPartida_clicked();

    void on_cmbMapas_changed();

    void on_cmbMatches_changed();

    void on_chkElement_clicked();

    void on_entryMatchName_changed();

private:
    void setCreateMatchButtonEnableStatus();

    bool mustCreateMatchBeEnabled();

    void on_btnUnirse_clicked();

    void initButtonJoin(Glib::RefPtr<Gtk::Builder> &refBuilder);

    void initCheckboxElements(Glib::RefPtr<Gtk::Builder> &refPtr);

    std::vector<std::string> getSelectedElements();

    bool isSomeElementCheckActiveAndChecked();

    void handlePlayButtonsAvailability();
};


#endif //TP4_TOWERDEFENSE_GAMEACCESS_H
