#include "GameAccess.h"
#include "../common/MessageFactory.h"
#include "../common/Socket.h"
#include "../common/TextMessage.h"
#include <gtkmm.h>
#include <iostream>

GameAccess::GameAccess(const Socket &c, const std::string &h, const uint16_t &p) : client(c), host(h), port(p) {
    clientId = -1;
}

GameAccess::~GameAccess(){}

void GameAccess::on_cmbMapas_changed(){
    setCreateMatchButtonEnableStatus();
}

void GameAccess::on_entryMatchName_changed(){
    setCreateMatchButtonEnableStatus();
}

void GameAccess::setCreateMatchButtonEnableStatus() {
    bool mustSetSensitive = mustCreateMatchBeEnabled();
    pBtnCrearPartida->set_sensitive(mustSetSensitive);
}

bool GameAccess::mustCreateMatchBeEnabled() {
    std::string map = cmbMapsText->get_active_text();
    return (map.compare(STR_NONE) != 0) && !(entryMatchName->get_text().empty());
}

void GameAccess::on_cmbElements_changed(){
    std::string elementName = cmbElementsText->get_active_text();

    if (elementName.compare(STR_NONE) != 0){
        /*std::string request = MessageFactory::getMatchElementsRequest(clientId, elementName);

        TextMessage textMessage(request);
        textMessage.sendTo(const_cast<Socket &>(client));*/
        pbtnJugar->set_sensitive(true);
        pbtnUnirse->set_sensitive(true);
    } else {
        pbtnJugar->set_sensitive(false);
        pbtnUnirse->set_sensitive(false);
    }
}

void GameAccess::on_cmbMatches_changed(){
    std::string matchName = cmbMatchesText->get_active_text();

    if (matchName.compare(STR_NONE) != 0){
        std::string request = MessageFactory::getMatchElementsRequest(clientId, matchName);

        TextMessage textMessage(request);
        textMessage.sendTo(const_cast<Socket &>(client));
    }
}

void GameAccess::on_btnCrearPartida_clicked() {
    std::string mapName = cmbMapsText->get_active_text();
    std::string matchName = entryMatchName->get_text();
    
    std::string request =  MessageFactory::getNewMatchRequest(clientId, mapName, matchName);

    TextMessage textMessage(request);
    textMessage.sendTo(const_cast<Socket &>(client));
}

void GameAccess::on_btnJugar_clicked() {
    /*GameWindowHandler *gwh = new GameWindowHandler(host, port);
    gameWindowHandlers.push_back(gwh);
    gwh->start();*/
    std::string matchName = entryMatchName->get_text();

    std::string request = MessageFactory::getStartMatchRequest(clientId, matchName);

    TextMessage textMessage(request);
    textMessage.sendTo(const_cast<Socket &>(client));
}

void GameAccess::on_btnUnirse_clicked() {
    std::string matchName = cmbMatchesText->get_active_text();
    std::string elementName = cmbElementsText->get_active_text();

    std::string request =  MessageFactory::getRemoveElementRequest(clientId, matchName, elementName);

    TextMessage textMessage(request);
    textMessage.sendTo(const_cast<Socket &>(client));
}

void GameAccess::initButtonCreateMatch(Glib::RefPtr<Gtk::Builder> &refBuilder) {
    refBuilder->get_widget("btnCrearPartida", pBtnCrearPartida);
    pBtnCrearPartida->set_sensitive(false);
    if (pBtnCrearPartida) {
            pBtnCrearPartida->signal_clicked()
                    .connect(
                            sigc::mem_fun(*this, &GameAccess::on_btnCrearPartida_clicked)
                    );
        }
}

void GameAccess::initButtonPlay(Glib::RefPtr<Gtk::Builder> &refBuilder) {
    refBuilder->get_widget("btnJugar", pbtnJugar);
    pbtnJugar->set_sensitive(false);
    if (pbtnJugar) {
            pbtnJugar->signal_clicked()
                    .connect(
                            sigc::mem_fun(*this, &GameAccess::on_btnJugar_clicked)
                    );
        }
}

void GameAccess::initButtonJoin(Glib::RefPtr<Gtk::Builder> &refBuilder) {
    refBuilder->get_widget("btnUnirse", pbtnUnirse);
    pbtnUnirse->set_sensitive(false);
    if (pbtnUnirse) {
        pbtnUnirse->signal_clicked()
                .connect(
                        sigc::mem_fun(*this, &GameAccess::on_btnUnirse_clicked)
                );
    }
}

void GameAccess::initComboMaps(Glib::RefPtr<Gtk::Builder> &refBuilder) {
    refBuilder->get_widget("cmbMapas", cmbMapsText);
    cmbMapsText->append(STR_NONE);
    cmbMapsText->signal_changed()
                .connect(
                        sigc::mem_fun(*this, &GameAccess::on_cmbMapas_changed)
                );
}

void GameAccess::initEntryMatchName(Glib::RefPtr<Gtk::Builder> &refBuilder) {
    refBuilder->get_widget("entryNombrePartida", entryMatchName);
    entryMatchName->signal_changed()
            .connect(
                    sigc::mem_fun(*this, &GameAccess::on_entryMatchName_changed)
            );
}

void GameAccess::initComboMatches(Glib::RefPtr<Gtk::Builder> &refBuilder) {
    refBuilder->get_widget("cmbPartidas", cmbMatchesText);
    cmbMatchesText->append(STR_NONE);
    cmbMatchesText->signal_changed()
            .connect(
                    sigc::mem_fun(*this, &GameAccess::on_cmbMatches_changed)
            );
}

void GameAccess::initComboElements(Glib::RefPtr<Gtk::Builder> &refBuilder) {
    refBuilder->get_widget("cmbElementos", cmbElementsText);
    cmbElementsText->append(STR_NONE);
    cmbElementsText->signal_changed()
            .connect(
                    sigc::mem_fun(*this, &GameAccess::on_cmbElements_changed)
            );
}

void GameAccess::addMapsToCombo(const std::vector<std::string> &maps) {
    for (std::string map : maps){
        cmbMapsText->append(map);
    }
}

void GameAccess::addMatchToCombo(const std::string &mapName, const std::string &matchName) {
    cmbMatchesText->append(mapName, matchName);
}

void GameAccess::addElementsToCombo(const std::list<std::string> &elements) {
    for (std::string element : elements){
        cmbElementsText->append(element);
    }
}

void GameAccess::addMap(std::string map) {
    /*Gtk::TreeModel::Row row = *(cmbMapsModel->append());
    //row[cmbMapsColumns.getId()] = 0;
    row[cmbMapsColumns.getValue()] = map;*/
}

void GameAccess::dropMap(std::string map) {
    /*Gtk::TreeModel::Children children = cmbMapsModel->children();
    Gtk::TreeModel::Row row;

    Gtk::TreeModel::Children::iterator iter;

    for (iter = children.begin(); iter != children.end(); ++iter) {
        row = *iter;

        if (row[cmbMapsColumns.getValue()] == map) {
            cmbMapsModel->erase(row);
            break;
        }
    }

*/    //updateHeader();
}

void GameAccess::updateHeader() {
    /*Gtk::TreeViewColumn *col = m_TreeView.get_column(0);

    std::string encabezado = "Nivel " + std::to_string(nivel);

    encabezado += " ( " + std::to_string(jugConectados);
    encabezado += "/" + std::to_string(jugNecesarios) + " )";

    col->set_title(encabezado);
    col->set_alignment(0.5);*/
}


void GameAccess::setClientId(int cid){
    clientId = cid;
}

int GameAccess::getClientId() {
    return clientId;
}

bool GameAccess::isNotValidClientId(){
    return clientId == -1;
}

void GameAccess::run() {
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
        initComboElements(refBuilder);

        app->run(*pWindow);
    }

    delete pWindow;

    for (unsigned int i = 0; i < gameWindowHandlers.size(); ++i){
        gameWindowHandlers[i]->join();
        delete gameWindowHandlers[i];
    }
}

