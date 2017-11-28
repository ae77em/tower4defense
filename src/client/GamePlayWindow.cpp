#include "GamePlayWindow.h"

#include "Dot.h"
#include "SdlUtils.h"
#include "MessageFactory.h"
#include "Protocol.h"
#include "GreenDaemon.h"
#include "Zombie.h"
#include "TextMessage.h"
#include "Request.h"
#include "BloodHawk.h"
#include "Goatman.h"
#include "Spectre.h"
#include "Air.h"
#include "Fire.h"
#include "Earth.h"
#include "Water.h"
#include "ExitPortal.h"

#include <iostream>
#include <fstream>
#include <SDL2/SDL_image.h>
#include <algorithm>
#include <utility>
#include <string>
#include <map>
#include <vector>

GamePlayWindow::GamePlayWindow(Socket *s,
                               SharedBuffer *in,
                               SharedBuffer *ot,
                               int cId,
                               std::vector<std::string> &elems,
                               std::string mn,
                               std::string mp)
        : server(s),
          nonPlayerNotifications(in),
          playerNotifications(ot),
          clientId(cId),
          playerElements(elems),
          matchName(std::move(mn)),
          map(model::Map::loadFromString(mp)) {
    abmonibleTexture = new Texture();
    blookHawkTexture = new Texture();
    goatmanTexture = new Texture();
    greenDaemonTexture = new Texture();
    spectreTexture = new Texture();
    zombieTexture = new Texture();

    typeOfTowerToPut = -1;
    isCastingSpells = false;
    timeOfLastSpell = -TIME_FOR_ENABLE_ACTION;
    timeOfLastTowerPutted = -TIME_FOR_ENABLE_ACTION;
    timeOfLastUpgradeMessage = -TIME_FOR_SHOW_TEMPORARY_MESSAGE;

    map = model::Map::loadFromString(mp);

    TILES_ROWS = map.dimensions().y;
    TILES_COLUMNS = map.dimensions().x;
    TOTAL_TILES = TILES_ROWS * TILES_COLUMNS;

    towerSelected = -1;
}

GamePlayWindow::~GamePlayWindow() {
    delete abmonibleTexture;
    delete blookHawkTexture;
    delete goatmanTexture;
    delete greenDaemonTexture;
    delete spectreTexture;
    delete zombieTexture;

    for (unsigned i = 0; i < hordes.size(); ++i) {
        std::vector<Enemy *> enemies = hordes[i].getEnemies();
        for (unsigned j = 0; j < enemies.size(); ++j) {
            delete enemies[j];
        }
    }

    for (unsigned i = 0; i < towers.size(); ++i) {
        delete towers[i];
    }
}

void GamePlayWindow::renderTimeMessages(SDL_Rect &camera) {
    int t;
    if ((t = (SDL_GetTicks() - timeOfLastSpell)) < TIME_FOR_ENABLE_ACTION) {
        t /= 1000;
        t = 20 - t;
        std::string message("Puede lanzar hechizo nuevamete en: ");
        message.append(std::to_string(t));
        message.append(" seg");
        renderText(camera, message, 0, 0, 1, 100, 0);
    }

    if ((t = (SDL_GetTicks() - timeOfLastTowerPutted)) <
        TIME_FOR_ENABLE_ACTION) {
        t /= 1000;
        t = 20 - t;
        std::string message("Puede poner torre nuevamente en: ");
        message.append(std::to_string(t));
        message.append(" seg");
        renderText(camera, message, 0, 0, 1, 150, 0);
    }

    if ((SDL_GetTicks() - timeOfLastUpgradeMessage) <
        TIME_FOR_SHOW_TEMPORARY_MESSAGE) {
        if (!towerUpgradeInfoMessage.empty()) {
            renderText(camera, towerUpgradeInfoMessage, 0, 0, 200, 0, 0);
        }
    }
}

bool GamePlayWindow::init() {
    //Initialization flag
    bool success = true;

    //Initialize SDL
    if (TTF_Init() < 0) {
        std::cerr << "TTF could not initialize! SDL Error: "
                  << TTF_GetError()
                  << std::endl;
        success = false;
    } else if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL Error: "
                  << SDL_GetError()
                  << std::endl;
        success = false;
    } else {
        //Set texture filtering to linear
        if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
            std::cout << "Warning: Linear texture filtering not enabled!";
        }

        //Create window
        gWindow = SDL_CreateWindow("Tower4Defense", SDL_WINDOWPOS_UNDEFINED,
                                   SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
                                   SCREEN_HEIGHT, SDL_WINDOW_SHOWN |
                                                  SDL_WINDOW_RESIZABLE);
        if (gWindow == nullptr) {
            std::cerr << "Window could not be created! SDL Error: "
                      << SDL_GetError()
                      << std::endl;
            success = false;
        } else {
            //Create renderer for window
            gRenderer = SDL_CreateRenderer(gWindow, -1,
                                           SDL_RENDERER_ACCELERATED |
                                           SDL_RENDERER_PRESENTVSYNC);
            if (gRenderer == nullptr) {
                std::cerr << "Renderer could not be created! SDL Error: "
                          << SDL_GetError()
                          << std::endl;
                success = false;
            } else {
                //Initialize renderer color
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

                //Initialize PNG loading
                int imgFlags = IMG_INIT_PNG;
                if (!(IMG_Init(imgFlags) & imgFlags)) {
                    std::cerr << "SDL_image could not initialize! "
                            "SDL_image Error: "
                              << IMG_GetError()
                              << std::endl;
                    success = false;
                }

                font = TTF_OpenFont("resources/fonts/UbuntuMono-R.ttf", 16);
            }
        }
    }

    return success;
}

bool GamePlayWindow::loadMedia() {
    //Loading success flag
    bool success = true;

    //Load dot texture
    if (!dotTexture.loadFromFile("dot.bmp", gRenderer, 0x00, 0xFF, 0xFF)) {
        std::cerr << "Failed to load dot texture!" << std::endl;
        success = false;
    }

    //Load tile texture
    for (unsigned i = 0; i < TILES_IMAGES_PATHS.size(); ++i) {
        if (!gTileTextures[i].loadFromFile(TILES_IMAGES_PATHS[i], gRenderer)) {
            std::cerr << "Failed to load tile set texture!" << std::endl;
            success = false;
        }
    }

    bluePortalTexture
            .loadFromFile("images/sprites/portal-blue2.png", gRenderer);
    redPortalTexture
            .loadFromFile("images/sprites/portal-red.png", gRenderer);

    towerButtonsTexture
            .loadFromFile("images/sprites/towers-buttons.png", gRenderer);

    prohibitedTexture
            .loadFromFile("images/sprites/prohibited-sign.png", gRenderer);

    /* enemies */
    abmonibleTexture
            ->loadFromFile("images/sprites/enemy-abominable.png",
                           gRenderer, 0xFF, 0x00, 0x99);
    blookHawkTexture
            ->loadFromFile("images/sprites/enemy-blood-hawk.png",
                           gRenderer, 0xAA, 0xAA, 0xAA);
    goatmanTexture
            ->loadFromFile("images/sprites/enemy-goatman.png",
                           gRenderer, 0xAA, 0xAA, 0xAA);
    greenDaemonTexture
            ->loadFromFile("images/sprites/enemy-green-daemon.png",
                           gRenderer, 0xAA, 0xAA, 0xAA);
    spectreTexture
            ->loadFromFile("images/sprites/enemy-spectre.png",
                           gRenderer, 0xAA, 0xAA, 0xAA);
    zombieTexture
            ->loadFromFile("images/sprites/enemy-zombie.png",
                           gRenderer, 0xAA, 0xAA, 0xAA);

    /* towers */
    airTexture
            .loadFromFile("images/sprites/tower-air.png",
                          gRenderer, 0xFF, 0x00, 0x99);

    earthTexture
            .loadFromFile("images/sprites/tower-earth.png",
                          gRenderer, 0xFF, 0x00, 0x99);

    waterTexture
            .loadFromFile("images/sprites/tower-water.png",
                          gRenderer, 0xFF, 0x00, 0x99);

    fireTexture
            .loadFromFile("images/sprites/tower-fire.png",
                          gRenderer, 0xFF, 0x00, 0x99);

    //Load tile map
    if (!setTiles()) {
        std::cerr << "Failed to load tile set!" << std::endl;
        success = false;
    }

    return success;
}

void GamePlayWindow::close() {
    //Free loaded images
    dotTexture.free();
    for (unsigned i = 0; i < TOTAL_TILE_SPRITES; ++i) {
        gTileTextures[i].free();
    }

    abmonibleTexture->free();
    blookHawkTexture->free();
    goatmanTexture->free();
    greenDaemonTexture->free();
    spectreTexture->free();
    zombieTexture->free();

    //Destroy window
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gWindow = nullptr;
    gRenderer = nullptr;

    //Quit SDL subsystems
    IMG_Quit();
    SDL_Quit();
}

bool GamePlayWindow::setTiles() {
    //Success flag
    bool tilesLoaded = true;
    int tileType;

    std::vector<std::vector<Point>> &paths = map.getPaths();

    //The tile offset
    char tileSaved;

    //Initialize the tiles
    for (int i = 0; i < map.dimensions().x; ++i) {
        for (int j = 0; j < map.dimensions().y; ++j) {
            //Determines what kind of tile will be made

            /* Read tile from map file (see tile value reference in Map.h) */
            tileSaved = map.tile(i, j);

            if (tileSaved == '.' || tileSaved == 'E' || tileSaved == 'S') {
                tileType = tileIdTranslator.at(map.getBackgroundStyle());

                if (tileSaved == 'E') {
                    portals.push_back(new EnterPortal(i * CARTESIAN_TILE_WIDTH,
                                                      j * CARTESIAN_TILE_HEIGHT,
                                                      gRenderer,
                                                      &bluePortalTexture));
                } else if (tileSaved == 'S') {
                    portals.push_back(new ExitPortal(i * CARTESIAN_TILE_WIDTH,
                                                     j * CARTESIAN_TILE_HEIGHT,
                                                     gRenderer,
                                                     &redPortalTexture));
                }
            } else {
                tileType = tileIdTranslator.at(tileSaved);
            }

            Point pt(i, j);
            if (pointIsInPaths(pt, paths)) {
                tileType = TILE_WAY;
            }

            //If the number is a valid tile number
            if (tileType >= TILE_EMPTY && tileType <= TILE_EXIT_PORTAL) {
                tileSet.push_back(std::move(Tile(i, j, tileType)));
            } else {
                //If we don't recognize the tile type
                //Stop loading map
                std::cerr
                        << "Error loading map: Invalid tile type at "
                        << i
                        << std::endl;
                tilesLoaded = false;
                break;
            }
        }
    }

    //Clip the sprite sheet
    if (tilesLoaded) {
        // de 0 a 6 son los tiles de piso
        for (unsigned i = 0; i <= TILE_TOWER; ++i) {
            gTileClips[i].x = 0;
            gTileClips[i].y = 0;
            gTileClips[i].w = ISO_TILE_WIDTH;
            gTileClips[i].h = ISO_TILE_HEIGHT;
        }
    }

    //If the map was loaded fine
    return tilesLoaded;
}

void
GamePlayWindow::renderText(SDL_Rect &camera,
                           std::string text,
                           int x, int y,
                           Uint8 r, Uint8 g, Uint8 b) {
    SDL_Color textColor = {r, g, b, 0xFF};
    SDL_Color bgColor = {0, 0, 0, 0}; // fondo transparente

    TTF_SetFontStyle(font, TTF_STYLE_BOLD);

    promptTextTexture.generateFromText(text, gRenderer, font, textColor,
                                       bgColor);

    promptTextTexture.render(gRenderer, x, y);
}

void
GamePlayWindow::handleMouseEvents(SDL_Rect camera, SDL_Event e) {
    if (e.type == SDL_MOUSEBUTTONDOWN) {
        Point point = SdlUtils::getMouseRelativePoint(camera);

        /* Si hice click y tengo algún evento marcado para disparar
         * (por ejemplo, marqué un lugar para poner una torre, o quiero
         * poner una torre) manejo dicho evento. */
        switch (e.button.button) {
            case SDL_BUTTON_LEFT: {
                handleLeftButtonClick(point);
                break;
            }
            case SDL_BUTTON_RIGHT: {
                handleRightButtonClick(point);
                break;
            }
            default:
                std::cout << "se hizo click con algún botón que no se usa.";
        }
    }
}

void GamePlayWindow::handleLeftButtonClick(Point &point) {
    int mousePosX, mousePosY;
    SDL_GetMouseState(&mousePosX, &mousePosY);

    if (isClickOnTowerButton(mousePosX, mousePosY)) {
        typeOfTowerToPut = -1;

        if (mousePosX <= 40) {
            if (hasElement(STR_AIR)) {
                typeOfTowerToPut = TOWER_AIR;
            }
        } else if (mousePosX <= 80) {
            if (hasElement(STR_FIRE)) {
                typeOfTowerToPut = TOWER_FIRE;
            }
        } else if (mousePosX <= 120) {
            if (hasElement(STR_WATER)) {
                typeOfTowerToPut = TOWER_WATER;
            }
        } else if (mousePosX <= 160) {
            if (hasElement(STR_EARTH)) {
                typeOfTowerToPut = TOWER_EARTH;
            }
        } else if (mousePosX <= 200) {
            if ((SDL_GetTicks() - timeOfLastSpell) > TIME_FOR_ENABLE_ACTION) {
                isCastingSpells = true;
            }
        } else if (towerSelected != -1) {
            // quiero hacer upgrade
            typeOfUpgradeToDo = -1;
            if (mousePosX <= 240) {
                typeOfUpgradeToDo = UPGRADE_RANGE;
            } else if (mousePosX <= 280) {
                typeOfUpgradeToDo = UPGRADE_DAMAGE;
            } else if (mousePosX <= 320) {
                if (towers.at(towerSelected)->isFire()) {
                    typeOfUpgradeToDo = UPGRADE_REACH;
                } else {
                    timeOfLastUpgradeMessage = SDL_GetTicks();
                    towerUpgradeInfoMessage = "El alcance puede ser aumentado"
                            " únicamente en torres de fuego.";
                }
            } else {
                if (towers.at(towerSelected)->isWater()) {
                    typeOfUpgradeToDo = UPGRADE_SLOWDOWN;
                } else {
                    timeOfLastUpgradeMessage = SDL_GetTicks();
                    towerUpgradeInfoMessage = "El ralentizado puede ser "
                            "aumentado únicamente en torres de agua.";
                }
            }

            if (typeOfUpgradeToDo != -1) {
                doUpgradeRequest();
            }
        }
    } else if (isAValidPutTowerRequest(point)) {
        doPutTowerRequest(point);
        timeOfLastTowerPutted = SDL_GetTicks();
    } else if (isCastingSpells) {
        if (isGroundTerrain(point)) {
            doCastSpellRequest(point);

            isCastingSpells = false;
            timeOfLastSpell = SDL_GetTicks();
        }
    } else if (isATowerPoint(point)) {
        doTowerInfoRequest();
    } else {
        // limpio los buffers de los mensajes de info que muestro
        towerDamageDataMessage = "";
        towerUpgradeInfoMessage = "";
    }
}

bool GamePlayWindow::isAValidPutTowerRequest(Point &point) {
    return typeOfTowerToPut != -1
           && (SDL_GetTicks() - timeOfLastTowerPutted) >
              TIME_FOR_ENABLE_ACTION
           && isFirmTerrain(point);
}

void GamePlayWindow::doTowerInfoRequest() {
    std::string request =
            MessageFactory::getTowerInfoRequest(clientId,
                                                matchName,
                                                towerSelected);
    sendToServer(request);
}

void GamePlayWindow::doUpgradeRequest() {
    std::string request =
            MessageFactory::getUpgradeRequest(clientId,
                                              matchName,
                                              towerSelected,
                                              typeOfUpgradeToDo);

    sendToServer(request);
}

void GamePlayWindow::doPutTowerRequest(const Point &point) {
    std::string request =
            MessageFactory::getPutTowerRequest(matchName,
                                               typeOfTowerToPut,
                                               point.x,
                                               point.y);

    sendToServer(request);
}

void GamePlayWindow::doCastSpellRequest(const Point &point) {
    std::string request =
            MessageFactory::getCastSpellRequest(matchName,
                                                point.x,
                                                point.y);
    sendToServer(request);
}

void GamePlayWindow::sendToServer(const std::string &request) {
    try {
        TextMessage toSend(request);
        toSend.sendTo(reinterpret_cast<Socket &>(*server));
    } catch (...){
        gameStatus = GAME_STATUS_DISCONECTED;
    }
}

void GamePlayWindow::handleRightButtonClick(Point point) {
    if (isFirmTerrain(point)) {
        std::string request =
                MessageFactory::getMarkTileRequest(matchName,
                                                   point.x,
                                                   point.y);

        sendToServer(request);
    }
}

void GamePlayWindow::handleServerNotifications(SDL_Rect camera) {
    int transactionsCounter = 0;
    std::string notification;

    while (nonPlayerNotifications->isProcessingYet() &&
           transactionsCounter < MAX_SERVER_NOTIFICATIONS_PER_FRAME) {
        ++transactionsCounter;
        notification = nonPlayerNotifications->getNextData();

        Message message;
        message.deserialize(notification);
        Request request(message);
        std::string response;

        int op = request.getAsInt(OPERATION_KEY);

        switch (op) {
            case SERVER_NOTIFICATION_SCENARIO_STATUS: {
                std::vector<Message> messages =
                        MessageFactory::getMovementNotifications(message);

                for (Message &aMessage : messages) {
                    Request request(aMessage);

                    Point scenarioPoint = MessageFactory::getPoint(aMessage);
                    int dir = MessageFactory::getDirection(aMessage);
                    int enemyId = request.getAsInt(ENEMY_ID_KEY);
                    int hordeId = request.getAsInt(HORDE_ID_KEY);
                    bool isAlive = request.getAsBool(IS_ALIVE_KEY);
                    bool isVisible = request.getAsBool(IS_VISIBLE_KEY);
                    double energyPercentaje =
                            request.getAsDouble(ENERGY_PERCENTAJE_KEY);

                    //FIXME: reducir el scope del try-catch
                    try {
                        DrawableHorde horde = hordes.at(hordeId);
                        Enemy *enemy = horde.getEnemieAt(enemyId);
                        enemy->setDirection(dir);
                        /* El dato que me llega es la posición relativa al
                         * escenario  de juego, no las coordenadas.
                         * */
                        enemy->setIsAlive(isAlive);
                        enemy->setIsVisible(isVisible);
                        enemy->moveTo(scenarioPoint.x, scenarioPoint.y);
                        enemy->setEnergyPercentaje(energyPercentaje);
                    } catch (...) {
                        std::cerr << "No es posible mover el enemigo "
                                  << std::to_string(enemyId);
                        std::cerr << " de la horda "
                                  << std::to_string(hordeId);
                    }
                }
                break;
            }
            case SERVER_NOTIFICATION_CREATE_HORDE: {
                int hordeId = request.getAsInt("hordeId");
                int hordeType = request.getAsInt("hordeType");
                int amount = request.getAsInt("amount");

                addNewHorde(hordeId, hordeType, amount);
                break;
            }
            case SERVER_NOTIFICATION_MATCH_ENDED: {
                nonPlayerNotifications->setClientProcessEnded(true);
                gameStatus = request.getAsInt(MATCH_STATUS_KEY);
                break;
            }
            case SERVER_NOTIFICATION_PUT_TOWER: {
                int towerId = request.getAsInt("towerId");
                int towerType = request.getAsInt("towerType");
                /* El dato que me llega es la posición relativa al
                 * escenario  de juego, no las coordenadas.
                 * */
                int x = request.getAsInt("xCoord") / CARTESIAN_TILE_WIDTH;
                int y = request.getAsInt("yCoord") / CARTESIAN_TILE_HEIGHT;

                putTower(towerId, towerType, x, y);
                break;
            }
            case SERVER_NOTIFICATION_MARK_TILE: {
                Point point = MessageFactory::getPoint(message);

                if (point.isPositive()) {
                    setToMarkedTile(point);
                }
                break;
            }
            case SERVER_NOTIFICATION_TOWER_INFO: {
                loadTowerInfo(message);
                break;
            }
            case SERVER_NOTIFICATION_CAST_SPELL: {
                Point point = MessageFactory::getPoint(message);

                if (point.isPositive()) {
                    setToFirmTile(point);
                }
                break;
            }
            case SERVER_NOTIFICATION_APPLY_UPGRADE: {
                Request request(message);
                timeOfLastUpgradeMessage = SDL_GetTicks();
                towerUpgradeInfoMessage = request.getAsString("info");
                break;
            };
            default:
                response = "no reconocida";
        }
    }
}

bool GamePlayWindow::isClickOnTowerButton(int mousePosX, int mousePosY) const {
    return (mousePosX >= TOWER_BUTTONS_X_POS
            && mousePosX <= TOWER_BUTTONS_WIDTH + TOWER_BUTTONS_Y_POS)
           &&
           (mousePosY >= TOWER_BUTTONS_Y_POS
            && mousePosY <= TOWBER_BUTTONS_HEIGHT + TOWER_BUTTONS_Y_POS);
}

bool GamePlayWindow::isFirmTerrain(Point &point) {
    bool isFirmTerrain = false;

    if (point.isPositive()) {
        int tilePos = point.x * TILES_COLUMNS + point.y;
        isFirmTerrain =
                tileSet.at(tilePos).getType() == TILE_FIRM
                || tileSet.at(tilePos).getType() == TILE_FIRM_MARKED;
    }

    return isFirmTerrain;
}

bool GamePlayWindow::isATowerPoint(Point &point) {
    bool isATowerPoint = false;

    if (point.isPositive()) {
        int tilePos = point.x * TILES_COLUMNS + point.y;
        if (tilePos < (int) tileSet.size()) {
            if (tileSet.at(tilePos).getType() == TILE_TOWER) {
                for (unsigned i = 0; i < towers.size(); ++i) {
                    if (tileSet.at(tilePos).getTower() == towers[i]) {
                        towerSelected = i;
                        isATowerPoint = true;
                        break;
                    }
                }
            }
        } else {
            towerSelected = -1;
        }
    }

    return isATowerPoint;
}

bool GamePlayWindow::isGroundTerrain(Point &point) {
    bool isGroundTerrain = false;

    if (point.isPositive()) {
        int tilePos = point.x * TILES_COLUMNS + point.y;
        isGroundTerrain = tileSet.at(tilePos).getType() == TILE_DESERT
                          || tileSet.at(tilePos).getType() == TILE_GRASS
                          || tileSet.at(tilePos).getType() == TILE_ICE
                          || tileSet.at(tilePos).getType() == TILE_LAVA;
    }

    return isGroundTerrain;
}

bool GamePlayWindow::hasElement(const std::string &element) const {
    return
            std::find(playerElements.begin(), playerElements.end(), element)
            != playerElements.end();
}

void GamePlayWindow::setToTowerTile(Point point, Tower *tower) {
    if (point.isPositive()) {
        int tilePos = point.x * TILES_COLUMNS + point.y;
        tileSet.at(tilePos).setType(TILE_TOWER);
        tileSet.at(tilePos).setTower(tower);
    }
}

void GamePlayWindow::setToMarkedTile(Point &point) {
    if (point.isPositive()) {
        int tilePos = point.x * TILES_COLUMNS + point.y;
        tileSet.at(tilePos).setType(TILE_FIRM_MARKED);
        tileSet.at(tilePos).setIsMarked(true);
        tileSet.at(tilePos).setMarkedTime(SDL_GetTicks());
    }
}

void GamePlayWindow::setToFirmTile(Point &point) {
    if (point.isPositive()) {
        int tilePos = point.x * TILES_COLUMNS + point.y;
        tileSet.at(tilePos).setType(TILE_FIRM);
        tileSet.at(tilePos).setIsMarked(false);
        tileSet.at(tilePos).setMarkedTime(0);
    }
}

void GamePlayWindow::loadTowerInfo(Message message) {
    Request request(message);

    std::string item;

    item = request.getAsString("class");
    towerClassDataMessage.assign(item);

    item = request.getAsString("experience");
    towerExperiencePointsDataMessage.assign(item);

    item = request.getAsString("damage");
    towerDamageDataMessage.assign(item);

    item = request.getAsString("range");
    towerRangeDataMessage.assign(item);

    item = request.getAsString("reach");
    towerReachDataMessage.assign(item);

    item = request.getAsString("slowDown");
    towerSlowdownDataMessage.assign(item);
}

void GamePlayWindow::run() {
    //Start up SDL and create window

    if (!init()) {
        std::cerr << "Failed to initializde!" << std::endl;
    } else {
        //Level camera
        camera = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
        renderSplashScreen();

        //Load media
        if (!loadMedia()) {
            std::cerr << "Failed to load media!" << std::endl;
            renderMessageInCenterOfScreen("NO SE PUDO CARGAR EL JUEGO...");
        } else {
            //Main loop flag
            bool quit = false;

            //Event handler
            SDL_Event e;

            //The dot that will be moving around on the screen
            Dot dot;

            //While application is running
            while (!quit) {
                try {
                    //Handle events on queue
                    while (SDL_PollEvent(&e) != 0) {
                        //User requests quit
                        if (e.type == SDL_QUIT) {
                            quit = true;
                        }

                        // Change dot velocity
                        dot.handleEvent(e, gWindow);

                        handleMouseEvents(camera, e);
                    }

                    handleServerNotifications(camera);

                    // Move the camera
                    dot.move();
                    dot.setCamera(camera);

                    // Clear screen
                    SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 0xFF);
                    SDL_RenderClear(gRenderer);

                    renderLevel();
                    dot.render(dotTexture, camera, gRenderer);

                    loadAnimables();
                    animateAnimables();

                    towerButtonsTexture.render(gRenderer, 1, 1);
                    renderProhibited();
                    renderMessages();

                    //Update screen
                    SDL_RenderPresent(gRenderer);
                } catch (...) {
                    std::cerr << "Excepción en el loop de juego." << std::endl;
                    std::cerr << "Se sigue con la ejecución." << std::endl;
                }
            }
        }

        //Free resources and close SDL
        close();
    }

    server->shutdown(); // TODO ver si esto corresponde hacerlo acá...*/
}

void GamePlayWindow::renderMessageInCenterOfScreen(std::string message) {
    SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 0xFF);
    SDL_RenderClear(gRenderer);
    renderText(camera, message,
                   (SCREEN_WIDTH / 2) - message.size() / 2,
                   (SCREEN_HEIGHT / 2) - 8,
               0xFF, 0x00, 0x00);
    SDL_RenderPresent(gRenderer);
}

void GamePlayWindow::renderSplashScreen(){
    std::string line1, line2, line3, line4, line5, line6, line7, line8;
    SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 0xFF);
    SDL_RenderClear(gRenderer);
    int center = (SCREEN_WIDTH / 2) - 40 * 8;
    int middle = (SCREEN_HEIGHT / 2) - 8;
    line1 = R"(       _                           ___     _       __                    )";
    line2 = R"(      | |                         /   |   | |     / _|                   )";
    line3 = R"(      | |_ _____      _____ _ __ / /| | __| | ___| |_ ___ _ __  ___  ___ )";
    line4 = R"(      | __/ _ \ \ /\ / / _ \ '__/ /_| |/ _` |/ _ \  _/ _ \ '_ \/ __|/ _ \)";
    line5 = R"(      | || (_) \ V  V /  __/ |  \___  | (_| |  __/ ||  __/ | | \__ \  __/)";
    line6 = R"(       \__\___/ \_/\_/ \___|_|      |_/\__,_|\___|_| \___|_| |_|___/\___|)";
    line7 = "                                 ";
    line8 = "                                 CARGANDO...";
    renderText(camera, line1, center, middle - 18*3, 0xFF, 0x00, 0x00);
    renderText(camera, line2, center, middle - 18*2, 0xFF, 0x00, 0x00);
    renderText(camera, line3, center, middle - 18*1, 0xFF, 0x00, 0x00);
    renderText(camera, line4, center, middle + 18*0, 0xFF, 0x00, 0x00);
    renderText(camera, line5, center, middle + 18*1, 0xFF, 0x00, 0x00);
    renderText(camera, line6, center, middle + 18*2, 0xFF, 0x00, 0x00);
    renderText(camera, line7, center, middle + 18*3, 0xFF, 0x00, 0x00);
    renderText(camera, line8, center, middle + 18*4, 0xFF, 0x00, 0x00);
    SDL_RenderPresent(gRenderer);
}

void GamePlayWindow::animateAnimables() {
    Animable *anPtr = nullptr;
    while (!animables.empty()) {
        anPtr = animables.top();
        anPtr->animate(camera);
        animables.pop();
    }
}

void GamePlayWindow::renderMessages() {
    std::string message = "";
    if (gameStatus == GAME_STATUS_WON) {
        message = "FELICIDADES, PARTIDA GANADA :) ...";
        renderText(camera, message, 0, 0, 0, 0, 0);
    } else if (gameStatus == GAME_STATUS_LOST) {
        message = "Partida perdida :( ...";
        renderText(camera, message, 0, 0, 0, 0, 0);
    } else if (gameStatus == GAME_STATUS_DISCONECTED) {
        message = "Hemos perdido comunicación con el server :(..."
                "lamentablemente tendrás que reiniciar el juego.";
        renderMessageInCenterOfScreen(message);
    }

    renderTimeMessages(camera);

    if (!towerDamageDataMessage.empty()) {
        message = "Datos de la torre:";
        renderText(camera, message, 1, 250);
        renderText(camera, towerClassDataMessage, 1, 266);
        renderText(camera, towerExperiencePointsDataMessage, 1, 282);
        renderText(camera, towerDamageDataMessage, 1, 298);
        renderText(camera, towerRangeDataMessage, 1, 314);
        renderText(camera, towerReachDataMessage, 1, 330);
        renderText(camera, towerSlowdownDataMessage, 1, 346);
    }
}

/* Remuevo los animables del vector (ojo, siguen existiendo,
 * pero no están en el vector), ya que pueden ir cambiando de
 * posición en cada iteración, y los vuelvo a cargar y ordenar,
 * para volver a mostrarlos bien.
 */
void GamePlayWindow::loadAnimables() {
    // limpio la cola de prioridad
    animables = std::priority_queue<Animable *, std::vector<Animable *>,
            lessThanByPoint>();

    // la cargo con torres
    Tower *tp = nullptr;
    std::map<int, Tower *>::const_iterator tIt;
    for (tIt = towers.begin(); tIt != towers.end(); ++tIt) {
        tp = tIt->second;
        animables.push(tp);
    }

    // la cargo con enemigos
    std::map<int, DrawableHorde>::const_iterator hIt;
    for (hIt = hordes.begin(); hIt != hordes.end(); ++hIt) {
        DrawableHorde horde = hIt->second;

        for (Enemy *enemy : horde.getEnemies()) {
            animables.push(enemy);
        }
    }

    for (unsigned i = 0; i < portals.size(); ++i) {
        Animable *portal = portals[i];
        animables.push(portal);
    }

}

void GamePlayWindow::renderLevel() {
    for (int i = 0; i < TOTAL_TILES; ++i) {
        if (tileSet.at(i).itIsMarked()) {
            tileSet.at(i).verifyIfMustContinueMarked();

            if (!tileSet.at(i).itIsMarked())
                tileSet.at(i).setType(TILE_FIRM);
        }

        if (tileSet.at(i).isDrawable()) {
            tileSet.at(i).render(camera,
                                 gTileClips,
                                 gRenderer,
                                 gTileTextures);
        }
    }
}

void GamePlayWindow::putTower(int id, int type, int x, int y) {
    Tower *toPut = nullptr;
    switch (type) {
        case TOWER_AIR: {
            toPut = new Air(x, y, gRenderer, airTexture);
            break;
        }
        case TOWER_EARTH: {
            toPut = new Earth(x, y, gRenderer, earthTexture);
            break;
        }
        case TOWER_WATER: {
            toPut = new Water(x, y, gRenderer, waterTexture);
            break;
        }
        case TOWER_FIRE: {
            toPut = new Fire(x, y, gRenderer, fireTexture);
            break;
        }
        default: {
            toPut = new Tower(x, y, gRenderer, earthTexture);
        }
    }

    //toPut->setSprites();
    towers.insert(std::make_pair(id, toPut));

    /* Para ubicarlo en el array de tiles, tengo que obtener las coordenadas
     * como si fuera el escaque de un tablero de ajedrez, sin tener en cuenta
     * las dimensiones.
     * */
    Point point(x, y);
    setToTowerTile(point, toPut);
    toPut->setPosition(x * CARTESIAN_TILE_WIDTH, y * CARTESIAN_TILE_HEIGHT);
}

void GamePlayWindow::addNewHorde(int hordeId, int enemyType, int amount) {
    Enemy *enemy = nullptr;
    DrawableHorde horde;

    for (int i = 0; i < amount; ++i) {
        switch (enemyType) {
            case ENEMY_ABMONIBLE: {
                enemy = new Abmonible(-1, -1, gRenderer, abmonibleTexture);
                break;
            }
            case ENEMY_BLOOD_HAWK: {
                enemy = new BloodHawk(-1, -1, gRenderer, blookHawkTexture);
                break;
            }
            case ENEMY_GOATMAN: {
                enemy = new Goatman(-1, -1, gRenderer, goatmanTexture);
                break;
            }
            case ENEMY_GREEN_DAEMON: {
                enemy = new GreenDaemon(-1, -1, gRenderer, greenDaemonTexture);
                break;
            }
            case ENEMY_SPECTRE: {
                enemy = new Spectre(-1, -1, gRenderer, spectreTexture);
                break;
            }
            case ENEMY_ZOMBIE: {
                enemy = new Zombie(-1, -1, gRenderer, zombieTexture);
                break;
            }
            default: {
                enemy = new Abmonible(-1, -1, gRenderer, abmonibleTexture);
            }
        }
        //enemy->setSprites();
        enemy->setIsVisible(false);
        horde.addEnemy(enemy);
    }

    hordes.insert(std::make_pair(hordeId, horde));
}

bool GamePlayWindow::pointIsInPaths(Point point,
                                    std::vector<std::vector<Point>> &vector) {
    bool isInPaths = false;
    for (unsigned i = 0; i < vector.size(); ++i) {
        std::vector<Point> innerVector = vector.at(i);
        for (unsigned j = 0; j < innerVector.size(); ++j) {
            if (point.x == innerVector.at(j).x
                && point.y == innerVector.at(j).y) {
                isInPaths = true;
                break;
            }
        }
        if (isInPaths) break;
    }

    return isInPaths;
}

void GamePlayWindow::renderProhibited() {
    if (!hasElement(STR_AIR)) {
        prohibitedTexture.render(gRenderer, 1, 1);
    }

    if (!hasElement(STR_FIRE)) {
        prohibitedTexture.render(gRenderer, 41, 1);
        prohibitedTexture.render(gRenderer, 281, 1);
    }

    if (!hasElement(STR_WATER)) {
        prohibitedTexture.render(gRenderer, 81, 1);
        prohibitedTexture.render(gRenderer, 321, 1);
    }

    if (!hasElement(STR_EARTH)) {
        prohibitedTexture.render(gRenderer, 121, 1);
    }
}
