#include "GameAccessWindow.h"
#include "../common/MessageFactory.h"
#include "../common/Socket.h"
#include "../common/TextMessage.h"
#include "../common/Protocol.h"
#include "GamePlayWindow.h"
#include <gtkmm.h>
#include <iostream>

GameAccessWindow::GameAccessWindow(Socket *c, SharedBuffer &tsnd, SharedBuffer &trcv)
        : client(c), toSend(tsnd), toReceive(trcv) {
    clientId = -1;
}

GameAccessWindow::~GameAccessWindow(){}


void GameAccessWindow::run() {
    auto app = Gtk::Application::create();

    //Load the GtkBuilder file and instantiate its widgets:
    auto refBuilder = Gtk::Builder::create();

    try {
        refBuilder->add_from_file("resources/glade/game-access.glade");
    }
    catch (const Glib::FileError &ex) {
        std::cerr << "FileError: " << ex.what() << std::endl;
        return;
    }
    catch (const Glib::MarkupError &ex) {
        std::cerr << "MarkupError: " << ex.what() << std::endl;
        return;
    }
    catch (const Gtk::BuilderError &ex) {
        std::cerr << "BuilderError: " << ex.what() << std::endl;
        return;
    }

    //Get the GtkBuilder-instantiated Dialog:
    refBuilder->get_widget("mainWindow", pWindow);
    if (pWindow) {
        initButtonCreateMatch(refBuilder);
        initButtonPlay(refBuilder);
        initButtonJoin(refBuilder);
        initComboMaps(refBuilder);
        initEntryMatchName(refBuilder);
        initComboMatches(refBuilder);
        initCheckboxElements(refBuilder);

        app->run(*pWindow);
    }

    delete pWindow;
}

void GameAccessWindow::on_cmbMapas_changed(){
    setCreateMatchButtonEnableStatus();
}

void GameAccessWindow::on_entryMatchName_changed(){
    setCreateMatchButtonEnableStatus();
    std::string match = cmbMatchesText->get_active_text();

    if (hasValidValue(match)){

    }
}

bool GameAccessWindow::hasValidValue(const std::string &match) const {
    return !match.empty() && STR_NONE.compare(match) != 0;
}

void GameAccessWindow::setCreateMatchButtonEnableStatus() {
    bool mustSetSensitive = mustCreateMatchBeEnabled();
    pBtnCrearPartida->set_sensitive(mustSetSensitive);
}

bool GameAccessWindow::mustCreateMatchBeEnabled() {
    std::string map = cmbMapsText->get_active_text();
    return (map.compare(STR_NONE) != 0) && !(entryMatchName->get_text().empty());
}

void GameAccessWindow::on_chkElement_clicked() {
    handlePlayButtonsAvailability();
}

void GameAccessWindow::handlePlayButtonsAvailability(){
    if (isSomeElementCheckActiveAndChecked()){
        pbtnJugar->set_sensitive(true);
        pbtnUnirse->set_sensitive(true);
    } else {
        pbtnJugar->set_sensitive(false);
        pbtnUnirse->set_sensitive(false);
    }
}

bool GameAccessWindow::isSomeElementCheckActiveAndChecked(){
    return (pchkAire->get_sensitive() && pchkAire->get_active())
            || (pchkAgua->get_sensitive() && pchkAgua->get_active())
            || (pchkFuego->get_sensitive() && pchkFuego->get_active())
            || (pchkTierra->get_sensitive() && pchkTierra->get_active());
}

void GameAccessWindow::on_cmbMatches_changed(){
    std::string matchName = cmbMatchesText->get_active_text();

    if (matchName.compare(STR_NONE) != 0){
        std::string request = MessageFactory::getUnavailableElementsRequest(clientId, matchName);

        TextMessage textMessage(request);
        textMessage.sendTo(const_cast<Socket &>(*client));
    }
}

void GameAccessWindow::on_btnCrearPartida_clicked() {
    std::string mapName = cmbMapsText->get_active_text();
    std::string matchName = entryMatchName->get_text();
    
    std::string request =  MessageFactory::getNewMatchRequest(clientId, mapName, matchName);

    TextMessage textMessage(request);
    textMessage.sendTo(const_cast<Socket &>(*client));
}

void GameAccessWindow::on_btnJugar_clicked() {
    pWindow->hide();

    GamePlayWindow game(client, &toReceive, &toSend, clientId);
    game.run();

    pWindow->show();
}

void GameAccessWindow::on_btnUnirse_clicked() {
    std::cout << "hice click en unirse..." << std::endl;
    std::string matchName = cmbMatchesText->get_active_text();
    std::vector<std::string> elements = getSelectedElements();

    std::string request = MessageFactory::getEnterMatchRequest(clientId, matchName, elements);

    TextMessage textMessage(request);
    textMessage.sendTo(const_cast<Socket &>(*client));
}

std::vector<std::string> GameAccessWindow::getSelectedElements(){
    std::vector<std::string> toReturn;

    if (pchkAire->get_active()){
        toReturn.push_back(STR_AIR);
    }
    if (pchkFuego->get_active()){
        toReturn.push_back(STR_FIRE);
    }
    if (pchkTierra->get_active()){
        toReturn.push_back(STR_TERRAIN);
    }
    if (pchkAgua->get_active()){
        toReturn.push_back(STR_WATER);
    }

    return toReturn;
}

void GameAccessWindow::initButtonCreateMatch(Glib::RefPtr<Gtk::Builder> &refBuilder) {
    refBuilder->get_widget("btnCrearPartida", pBtnCrearPartida);
    if (pBtnCrearPartida) {
        pBtnCrearPartida->set_sensitive(false);
        pBtnCrearPartida->signal_clicked()
                .connect(
                        sigc::mem_fun(*this, &GameAccessWindow::on_btnCrearPartida_clicked)
                );
        }
}

void GameAccessWindow::initButtonPlay(Glib::RefPtr<Gtk::Builder> &refBuilder) {
    refBuilder->get_widget("btnJugar", pbtnJugar);
    if (pbtnJugar) {
        pbtnJugar->set_sensitive(false);
        pbtnJugar->signal_clicked()
                .connect(
                        sigc::mem_fun(*this, &GameAccessWindow::on_btnJugar_clicked)
                );
    }
}

void GameAccessWindow::initButtonJoin(Glib::RefPtr<Gtk::Builder> &refBuilder) {
    refBuilder->get_widget("btnUnirse", pbtnUnirse);
    if (pbtnUnirse) {
        pbtnUnirse->set_sensitive(false);
        pbtnUnirse->signal_clicked()
                .connect(
                        sigc::mem_fun(*this, &GameAccessWindow::on_btnUnirse_clicked)
                );
    }
}

void GameAccessWindow::initComboMaps(Glib::RefPtr<Gtk::Builder> &refBuilder) {
    refBuilder->get_widget("cmbMapas", cmbMapsText);
    cmbMapsText->append(STR_NONE);
    cmbMapsText->signal_changed()
                .connect(
                        sigc::mem_fun(*this, &GameAccessWindow::on_cmbMapas_changed)
                );
}

void GameAccessWindow::initEntryMatchName(Glib::RefPtr<Gtk::Builder> &refBuilder) {
    refBuilder->get_widget("entryNombrePartida", entryMatchName);
    entryMatchName->signal_changed()
            .connect(
                    sigc::mem_fun(*this, &GameAccessWindow::on_entryMatchName_changed)
            );
}

void GameAccessWindow::initComboMatches(Glib::RefPtr<Gtk::Builder> &refBuilder) {
    refBuilder->get_widget("cmbPartidas", cmbMatchesText);
    cmbMatchesText->append(STR_NONE);
    cmbMatchesText->signal_changed()
            .connect(
                    sigc::mem_fun(*this, &GameAccessWindow::on_cmbMatches_changed)
            );
}

void GameAccessWindow::initCheckboxElements(Glib::RefPtr<Gtk::Builder> &refBuilder) {
    refBuilder->get_widget("chkAire", pchkAire);
    if (pchkAire) {
        pchkAire->set_sensitive(false);
        pchkAire->signal_clicked()
                .connect(
                        sigc::mem_fun(*this, &GameAccessWindow::on_chkElement_clicked)
                );
    }

    refBuilder->get_widget("chkAgua", pchkAgua);
    if (pchkAgua) {
        pchkAgua->set_sensitive(false);
        pchkAgua->signal_clicked()
                .connect(
                        sigc::mem_fun(*this, &GameAccessWindow::on_chkElement_clicked)
                );
    }

    refBuilder->get_widget("chkFuego", pchkFuego);
    if (pchkFuego) {
        pchkFuego->set_sensitive(false);
        pchkFuego->signal_clicked()
                .connect(
                        sigc::mem_fun(*this, &GameAccessWindow::on_chkElement_clicked)
                );
    }

    refBuilder->get_widget("chkTierra", pchkTierra);
    if (pchkTierra) {
        pchkTierra->set_sensitive(false);
        pchkTierra->signal_clicked()
                .connect(
                        sigc::mem_fun(*this, &GameAccessWindow::on_chkElement_clicked)
                );
    }
}


void GameAccessWindow::addMapsToCombo(const std::vector<std::string> &maps) {
    for (std::string map : maps){
        cmbMapsText->append(map);
    }
}

void GameAccessWindow::addMatchesToCombo(const std::vector<std::string> &matches) {
    for (std::string match : matches){
        cmbMatchesText->append(match);
    }
}

void GameAccessWindow::addMatchToCombo(const std::string &mapName, const std::string &matchName) {
    cmbMatchesText->append(mapName, matchName);
}

void GameAccessWindow::setAvailableElements(const std::list<std::string> &unavailableElements) {
    pchkAire->set_sensitive(true);
    pchkFuego->set_sensitive(true);
    pchkTierra->set_sensitive(true);
    pchkAgua->set_sensitive(true);

    for (std::string unavailableElement : unavailableElements){
        if (STR_AIR.compare(unavailableElement) == 0){
            pchkAire->set_sensitive(false);
        } else if (STR_FIRE.compare(unavailableElement) == 0){
            pchkFuego->set_sensitive(false);
        } else if (STR_TERRAIN.compare(unavailableElement) == 0){
            pchkTierra->set_sensitive(false);
        } else if (STR_WATER.compare(unavailableElement) == 0){
            pchkAgua->set_sensitive(false);
        }
    }
}

void GameAccessWindow::setAvailableElementsForJoin(const std::list<std::string> &unavailableElements) {
    for (std::string element : unavailableElements){
        if (STR_AIR.compare(element) == 0){
            pchkAire->set_active(false);
            pchkAire->set_sensitive(false);
        } else if (STR_FIRE.compare(element) == 0){
            pchkAire->set_active(false);
            pchkFuego->set_sensitive(false);
        } else if (STR_TERRAIN.compare(element) == 0){
            pchkAire->set_active(false);
            pchkTierra->set_sensitive(false);
        } else if (STR_WATER.compare(element) == 0){
            pchkAire->set_active(false);
            pchkAgua->set_sensitive(false);
        }
    }
}

void GameAccessWindow::setJoinedToMatch(int clientId, std::string mName){
    if (clientId == this->getClientId()){
        pbtnUnirse->set_sensitive(false);
        pbtnJugar->set_sensitive(true);

        matchName = mName;
    }
}


void GameAccessWindow::setClientId(int cid){
    clientId = cid;
}

int GameAccessWindow::getClientId() {
    return clientId;
}

bool GameAccessWindow::isNotValidClientId(){
    return clientId == -1;
}


