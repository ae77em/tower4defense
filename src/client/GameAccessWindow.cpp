#include "GameAccessWindow.h"
#include "MessageFactory.h"
#include "TextMessage.h"
#include "Protocol.h"
#include <string>
#include <list>
#include <vector>
#include <iostream>

GameAccessWindow::GameAccessWindow(Socket *c,
                                   SharedBuffer &tsnd,
                                   SharedBuffer &trcv,
                                   SharedBuffer &ot)
        : server(c), toSend(tsnd), toReceive(trcv), other(ot) {
    clientId = -1;
    gameStarted = false;
}

GameAccessWindow::~GameAccessWindow() {
    delete game;
}


void GameAccessWindow::run() {
    uiMutex.lock();
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
        initDispatcher(refBuilder);

        uiMutex.unlock();
        app->run(*pWindow);
    } else {
        uiMutex.unlock();
    }

    delete pWindow;

    // les aviso a los encargados de comunicación que terminé
    if (!gameStarted) {
        server->shutdown();
    }
}

void GameAccessWindow::on_cmbMapas_changed() {
    setCreateMatchButtonEnableStatus();
}

void GameAccessWindow::on_entryMatchName_changed() {
    setCreateMatchButtonEnableStatus();
    std::string match = cmbMatchesText->get_active_text();

    if (hasValidValue(match)) {}
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
    return (map.compare(STR_NONE) != 0) &&
           !(entryMatchName->get_text().empty());
}

void GameAccessWindow::on_chkElement_clicked() {
    handlePlayButtonsAvailability();
}

void GameAccessWindow::handlePlayButtonsAvailability() {
    if (isSomeElementCheckActiveAndChecked()) {
        pbtnJugar->set_sensitive(true);
        pbtnUnirse->set_sensitive(true);
    } else {
        pbtnJugar->set_sensitive(false);
        pbtnUnirse->set_sensitive(false);
    }
}

bool GameAccessWindow::isSomeElementCheckActiveAndChecked() {
    return (pchkAire->get_sensitive() && pchkAire->get_active())
           || (pchkAgua->get_sensitive() && pchkAgua->get_active())
           || (pchkFuego->get_sensitive() && pchkFuego->get_active())
           || (pchkTierra->get_sensitive() && pchkTierra->get_active());
}

void GameAccessWindow::on_cmbMatches_changed() {
    std::string matchName = cmbMatchesText->get_active_text();

    if (matchName.compare(STR_NONE) != 0) {
        std::string request = MessageFactory::getUnavailableElementsRequest(
                clientId, matchName);

        TextMessage textMessage(request);
        textMessage.sendTo(const_cast<Socket &>(*server));
    }
}

void GameAccessWindow::on_btnCreateMatch_clicked() {
    std::string mapName = cmbMapsText->get_active_text();
    std::string matchName = entryMatchName->get_text();

    std::string request = MessageFactory::getNewMatchRequest(clientId, mapName,
                                                             matchName);

    TextMessage textMessage(request);
    textMessage.sendTo(const_cast<Socket &>(*server));
}

void GameAccessWindow::on_btnPlay_clicked() {
    std::string matchName = cmbMatchesText->get_active_text();

    std::string request = MessageFactory::getStartMatchRequest(clientId,
                                                               matchName);

    TextMessage textMessage(request);
    textMessage.sendTo(const_cast<Socket &>(*server));
}

void GameAccessWindow::on_btnJoin_clicked() {
    std::string matchName = cmbMatchesText->get_active_text();
    std::vector<std::string> elements = getSelectedElements();


    std::string request = MessageFactory::getEnterMatchRequest(clientId,
                                                               matchName,
                                                               elements);

    TextMessage textMessage(request);
    textMessage.sendTo(const_cast<Socket &>(*server));
}

std::vector<std::string> GameAccessWindow::getSelectedElements() {
    std::vector<std::string> toReturn;

    if (pchkAire->get_active()) {
        toReturn.push_back(STR_AIR);
    }
    if (pchkFuego->get_active()) {
        toReturn.push_back(STR_FIRE);
    }
    if (pchkTierra->get_active()) {
        toReturn.push_back(STR_EARTH);
    }
    if (pchkAgua->get_active()) {
        toReturn.push_back(STR_WATER);
    }

    myElements = toReturn;

    return toReturn;
}

void GameAccessWindow::initButtonCreateMatch(
        Glib::RefPtr<Gtk::Builder> &refBuilder) {
    refBuilder->get_widget("btnCrearPartida", pBtnCrearPartida);
    if (pBtnCrearPartida) {
        pBtnCrearPartida->set_sensitive(false);
        pBtnCrearPartida->signal_clicked()
                .connect(
                        sigc::mem_fun(*this,
                                      &GameAccessWindow::on_btnCreateMatch_clicked));
    }
}

void GameAccessWindow::initButtonPlay(Glib::RefPtr<Gtk::Builder> &refBuilder) {
    refBuilder->get_widget("btnJugar", pbtnJugar);
    if (pbtnJugar) {
        pbtnJugar->set_sensitive(false);
        pbtnJugar->signal_clicked()
                .connect(
                        sigc::mem_fun(*this,
                                      &GameAccessWindow::on_btnPlay_clicked));
    }
}

void GameAccessWindow::initButtonJoin(Glib::RefPtr<Gtk::Builder> &refBuilder) {
    refBuilder->get_widget("btnUnirse", pbtnUnirse);
    if (pbtnUnirse) {
        pbtnUnirse->set_sensitive(false);
        pbtnUnirse->signal_clicked()
                .connect(
                        sigc::mem_fun(*this,
                                      &GameAccessWindow::on_btnJoin_clicked));
    }
}

void GameAccessWindow::initComboMaps(Glib::RefPtr<Gtk::Builder> &refBuilder) {
    refBuilder->get_widget("cmbMapas", cmbMapsText);
    cmbMapsText->append(STR_NONE);
    cmbMapsText->signal_changed()
            .connect(
                    sigc::mem_fun(*this,
                                  &GameAccessWindow::on_cmbMapas_changed));
    cmbMapsText->set_border_width(10);
}

void
GameAccessWindow::initEntryMatchName(Glib::RefPtr<Gtk::Builder> &refBuilder) {
    refBuilder->get_widget("entryNombrePartida", entryMatchName);
    entryMatchName->signal_changed()
            .connect(
                    sigc::mem_fun(*this,
                                  &GameAccessWindow::on_entryMatchName_changed));
    entryMatchName->set_max_length(10);
}

void
GameAccessWindow::initComboMatches(Glib::RefPtr<Gtk::Builder> &refBuilder) {
    refBuilder->get_widget("cmbPartidas", cmbMatchesText);
    cmbMatchesText->append(STR_NONE);
    cmbMatchesText->signal_changed()
            .connect(
                    sigc::mem_fun(*this,
                                  &GameAccessWindow::on_cmbMatches_changed));
    cmbMatchesText->set_border_width(8);
}

void
GameAccessWindow::initCheckboxElements(Glib::RefPtr<Gtk::Builder> &refBuilder) {
    refBuilder->get_widget("chkAire", pchkAire);
    if (pchkAire) {
        pchkAire->set_sensitive(false);
        pchkAire->signal_clicked()
                .connect(
                        sigc::mem_fun(*this,
                                      &GameAccessWindow::on_chkElement_clicked));
    }

    refBuilder->get_widget("chkAgua", pchkAgua);
    if (pchkAgua) {
        pchkAgua->set_sensitive(false);
        pchkAgua->signal_clicked()
                .connect(
                        sigc::mem_fun(*this,
                                      &GameAccessWindow::on_chkElement_clicked));
    }

    refBuilder->get_widget("chkFuego", pchkFuego);
    if (pchkFuego) {
        pchkFuego->set_sensitive(false);
        pchkFuego->signal_clicked()
                .connect(
                        sigc::mem_fun(*this,
                                      &GameAccessWindow::on_chkElement_clicked));
    }

    refBuilder->get_widget("chkTierra", pchkTierra);
    if (pchkTierra) {
        pchkTierra->set_sensitive(false);
        pchkTierra->signal_clicked()
                .connect(
                        sigc::mem_fun(*this,
                                      &GameAccessWindow::on_chkElement_clicked));
    }
}


void GameAccessWindow::addMapsToCombo(const std::vector<std::string> &maps) {
    for (std::string map : maps) {
        cmbMapsText->append(map);
    }
}

void GameAccessWindow::addMatchesToCombo(
        const std::vector<std::string> &matches) {
    for (std::string match : matches) {
        cmbMatchesText->append(match);
    }
}

void GameAccessWindow::addMatchToCombo(int clientId,
                                       const std::string &matchName) {
    cmbMatchesText->append(matchName);

    if (clientId == getClientId()) {
        cmbMatchesText->set_active_text(matchName);
    }
}

void GameAccessWindow::setAvailableElements(
        const std::list<std::string> &unavailableElements) {
    pchkAire->set_sensitive(true);
    pchkFuego->set_sensitive(true);
    pchkTierra->set_sensitive(true);
    pchkAgua->set_sensitive(true);

    for (std::string unavailableElement : unavailableElements) {
        if (STR_AIR.compare(unavailableElement) == 0) {
            pchkAire->set_sensitive(false);
        } else if (STR_FIRE.compare(unavailableElement) == 0) {
            pchkFuego->set_sensitive(false);
        } else if (STR_EARTH.compare(unavailableElement) == 0) {
            pchkTierra->set_sensitive(false);
        } else if (STR_WATER.compare(unavailableElement) == 0) {
            pchkAgua->set_sensitive(false);
        }
    }
}

void GameAccessWindow::setAvailableElementsForJoin(
        const std::list<std::string> &unavailableElements) {
    for (std::string element : unavailableElements) {
        if (STR_AIR.compare(element) == 0) {
            pchkAire->set_active(false);
            pchkAire->set_sensitive(false);
        } else if (STR_FIRE.compare(element) == 0) {
            pchkFuego->set_active(false);
            pchkFuego->set_sensitive(false);
        } else if (STR_EARTH.compare(element) == 0) {
            pchkTierra->set_active(false);
            pchkTierra->set_sensitive(false);
        } else if (STR_WATER.compare(element) == 0) {
            pchkAgua->set_active(false);
            pchkAgua->set_sensitive(false);
        }
    }
}

void GameAccessWindow::setJoinedToMatch(int clientId, std::string mName) {
    if (clientId == this->getClientId()) {
        pbtnUnirse->set_sensitive(false);
        pbtnJugar->set_sensitive(true);

        matchName = mName;
    }
}

void GameAccessWindow::setClientId(int cid) {
    clientId = cid;
}

int GameAccessWindow::getClientId() {
    return clientId;
}

void GameAccessWindow::startMatch(std::string matchName, std::string map) {
    gameStarted = true;
    pWindow->hide();

    game = new GamePlayWindow(server,
                              &toReceive,
                              &other,
                              clientId,
                              myElements,
                              matchName,
                                map);

    game->start();
    game->join();

    server->shutdown(); // apago el server porque me voy
    /*// Acá debiera levantarse nuevamente la ventana de acceso.
     * pWindow->show_all();
    pWindow->show_now();*/
}

void GameAccessWindow::initDispatcher(Glib::RefPtr<Gtk::Builder> &refPtr) {
    dispatcher.connect(sigc::mem_fun(*this, &GameAccessWindow::updateUIData));
}

void GameAccessWindow::notify(std::string &dtl) {
    uiMutex.lock();
    dataForUILoad.push(dtl);
    dispatcher.emit();
    uiMutex.unlock();
}

void GameAccessWindow::updateUIData() {
    Message message;
    std::string response = "";
    std::string dataToLoad;

    while (!dataForUILoad.empty()) {

        dataToLoad = dataForUILoad.front();
        dataForUILoad.pop();

        message.deserialize(dataToLoad);

        int op = MessageFactory::getOperation(message);

        std::cout << "llego operación: " << std::to_string(op) << std::endl;

        switch (op) {
            case SERVER_NOTIFICATION_CLIENT_ID: {
                int clientId = MessageFactory::getClientId(message);
                setClientId(clientId);
                break;
            }
            case SERVER_NOTIFICATION_GET_ALL_MAPS: {
                std::vector<std::string> maps;
                maps = MessageFactory::getMaps(message);
                addMapsToCombo(maps);
                break;
            }
            case SERVER_NOTIFICATION_GET_ALL_MATCHES: {
                std::vector<std::string> matches;
                matches = MessageFactory::getMatches(message);
                addMatchesToCombo(matches);
                break;
            }
            case SERVER_NOTIFICATION_NEW_MATCH: {
                int clientId = MessageFactory::getClientId(message);
                std::string matchName = MessageFactory::getMatchName(message);

                addMatchToCombo(clientId, matchName);
                break;
            }
            case SERVER_NOTIFICATION_GET_UNAVAILABLE_ELEMENTS: {
                std::list<std::string> elements = MessageFactory::getElements(
                        message);
                setAvailableElements(elements);
                break;
            }
            case SERVER_NOTIFICATION_ENTER_EXISTING_MATCH: {
                int clientId = MessageFactory::getClientId(message);
                std::list<std::string> elements = MessageFactory::getElements(
                        message);

                if (getClientId() != clientId) {
                    setAvailableElementsForJoin(elements);
                }
                break;
            }
            case SERVER_NOTIFICATION_START_MATCH: {
                std::string matchName = MessageFactory::getMatchName(message);
                std::string serializedMap = MessageFactory::getSerializedMap(message);
                startMatch(matchName, serializedMap);
                break;
            }
            default:
                std::cout << "caso imposible";
        }
    }
}

