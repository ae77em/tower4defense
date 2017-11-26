#ifndef TP4_TOWERDEFENSE_GAMEACCESS_H
#define TP4_TOWERDEFENSE_GAMEACCESS_H

#include <gtkmm.h>
#include "Thread.h"
#include "Socket.h"
#include "SharedBuffer.h"
#include "GamePlayWindow.h"
#include "Notificable.h"
#include <string>
#include <list>
#include <vector>

class GameAccessWindow : public Thread, public Notificable {
private:
    std::mutex uiMutex;
    GamePlayWindow *game = nullptr;
    bool gameStarted;

    /* View */
    Gtk::Window *pWindow;
    Gtk::Button *pBtnCrearPartida = nullptr;
    Gtk::Button *pbtnJugar = nullptr;
    Gtk::Button *pbtnUnirse = nullptr;
    Gtk::Button *pbtnSalir = nullptr;
    Gtk::ComboBoxText *cmbMapsText = nullptr;
    Gtk::Entry *entryMatchName = nullptr;
    Gtk::ComboBoxText *cmbMatchesText = nullptr;
    Gtk::CheckButton *pchkAire = nullptr;
    Gtk::CheckButton *pchkAgua = nullptr;
    Gtk::CheckButton *pchkFuego = nullptr;
    Gtk::CheckButton *pchkTierra = nullptr;

    /* External modifications */
    Glib::Dispatcher dispatcher;
    std::queue<std::string> dataForUILoad;

    /* Connection */
    Socket *server;
    SharedBuffer &toSend;
    SharedBuffer &toReceive;
    SharedBuffer &other;
    int clientId;
    std::string matchName;
    std::vector<std::string> myElements;

public:
    GameAccessWindow(Socket *client,
                     SharedBuffer &toSend,
                     SharedBuffer &toReceive,
                     SharedBuffer &other);

    virtual ~GameAccessWindow();

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
     * Agrega las partidas pasados como parámetro al combo de partidas.
     * maps: vector de strings con los nombres de las partidas.
     */
    void addMatchesToCombo(const std::vector<std::string> &matches);

    /*
     * Agrega una partida al combo de partidas.
     * map: mapa donde se jugará la partida.
     * match: nombre de la partida.
     */
    void addMatchToCombo(int clientId, const std::string &matchName);

    /*
     * Setea el id del cliente (socket) con el cual me estoy conectando al
     * server.
     * Corresponde al FD del cliente en el server.
     * clientId: FD del cliente en el server.
     */
    void setClientId(int clientId);

    /*
     * Retorna el id del cliente (socket) con el cual me estoy conectando al
     * server.
     * Corresponde al FD del cliente en el server.
     */
    int getClientId();

    /* Especifica si el cliente esta conectado */
    bool isNotValidClientId();

    /* Habilita los checkboxes correspondientes a lo elementos disponibles, para
     * poder ser seleccionados.
     * */
    void setAvailableElements(const std::list<std::string> &unavailableElems);

    /* Inicializadores de los distintos inputs.
     * refBuilder: referencia al builder donde se están colocando los elementos.
     * */
    void initComboMaps(Glib::RefPtr<Gtk::Builder> &refBuilder);

    void initButtonPlay(Glib::RefPtr<Gtk::Builder> &refBuilder);

    void initButtonCreateMatch(Glib::RefPtr<Gtk::Builder> &refBuilder);

    void initComboMatches(Glib::RefPtr<Gtk::Builder> &refBuilder);

    void initEntryMatchName(Glib::RefPtr<Gtk::Builder> &refBuilder);

    /* Event handlers */
    void on_btnPlay_clicked();

    void on_btnCreateMatch_clicked();

    void on_cmbMapas_changed();

    void on_cmbMatches_changed();

    void on_chkElement_clicked();

    void on_entryMatchName_changed();

    void setAvailableElementsForJoin(
            const std::list<std::string> &unavailableElems);

    void setJoinedToMatch(int clientId, std::string matchName);

    void startMatch(std::string matchName, std::string map);

    void notify(std::string &dtl);

    void updateUIData();

private:
    void setCreateMatchButtonEnableStatus();

    bool mustCreateMatchBeEnabled();

    void on_btnJoin_clicked();

    void initButtonJoin(Glib::RefPtr<Gtk::Builder> &refBuilder);

    void initCheckboxElements(Glib::RefPtr<Gtk::Builder> &refPtr);

    std::vector<std::string> getSelectedElements();

    bool isSomeElementCheckActiveAndChecked();

    void handlePlayButtonsAvailability();

    bool hasValidValue(const std::string &match) const;

    void initDispatcher(Glib::RefPtr<Gtk::Builder> &refPtr);

    void on_loadData();

    void listen();


    void initButtonLeave(Glib::RefPtr<Gtk::Builder> &refBuilder);

    void on_btnLeave_clicked();
};

#endif //TP4_TOWERDEFENSE_GAMEACCESS_H
