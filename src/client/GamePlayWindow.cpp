#include "GamePlayWindow.h"

#include "../sdl/Dot.h"
#include "../sdl/Utils.h"
#include "../common/MessageFactory.h"
#include "../common/Protocol.h"
#include "../sdl/enemies/GreenDaemon.h"
#include "../sdl/enemies/Zombie.h"
#include "../common/TextMessage.h"
#include "../common/Request.h"
#include "../sdl/enemies/BloodHawk.h"
#include "../sdl/enemies/Goatman.h"
#include "../sdl/enemies/Spectre.h"
#include "../sdl/towers/Air.h"
#include "../sdl/towers/Fire.h"
#include "../sdl/towers/Earth.h"
#include "../sdl/towers/Water.h"

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
    towerIdThatRequiresInfo = -1;
    isCastingSpells = false;
    timeOfLastSpell = -TIME_FOR_ENABLE_ACTION;
    timeOfLastTowerPutted = -TIME_FOR_ENABLE_ACTION;

    map = model::Map::loadFromString(mp);

    TILES_ROWS = map.dimensions().y;
    TILES_COLUMNS = map.dimensions().x;
    TOTAL_TILES = TILES_ROWS * TILES_COLUMNS;
}

GamePlayWindow::~GamePlayWindow() {
    /* enemies text */
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
        renderText(camera, message, 1, 100);
    }

    if ((t = (SDL_GetTicks() - timeOfLastTowerPutted)) <
        TIME_FOR_ENABLE_ACTION) {
        t /= 1000;
        t = 20 - t;
        std::string message("Puede poner torre nuevamente en: ");
        message.append(std::to_string(t));
        message.append(" seg");
        renderText(camera, message, 1, 150);
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
        printf("Failed to load dot texture!\n");
        success = false;
    }

    //Load tile texture
    for (unsigned i = 0; i < TILES_IMAGES_PATHS.size(); ++i) {
        if (!gTileTextures[i].loadFromFile(TILES_IMAGES_PATHS[i], gRenderer)) {
            printf("Failed to load tile set texture!\n");
            success = false;
        }
    }

    bluePortalTexture
            .loadFromFile("images/sprites/portal-blue2.png", gRenderer);
    redPortalTexture
            .loadFromFile("images/sprites/portal-red.png", gRenderer);
    towerButtonsTexture
            .loadFromFile("images/sprites/towers-buttons.png", gRenderer);

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
        printf("Failed to load tile set!\n");
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

            //Read tile from map file
            tileSaved = map.tile(i, j);

            if (tileSaved == 'E' || tileSaved == 'S') { // es portal
                portals.push_back(new Portal());
                tileType = tileIdTranslator.at(map.getBackgroundStyle());
            } else if (tileSaved == '.') { // campo
                tileType = tileIdTranslator.at(map.getBackgroundStyle());
            } else if (tileSaved == 'x') { // firme
                tileType = tileIdTranslator.at(tileSaved);
            } else if (tileSaved == '#') { // es la nada
                tileType = tileIdTranslator.at(tileSaved);
            }

            Point pt(i, j);
            if (pointIsInPaths(pt, paths)) {
                tileType = TILE_WAY;
            }


            //If the number is a valid tile number
            if (tileType >= TILE_EMPTY && tileType <= TILE_TOWER) {
                tileSet.push_back(std::move(Tile(i, j, tileType)));
            } else {
                //If we don't recognize the tile type
                //Stop loading map
                printf("Error loading map: Invalid tile type at %d!\n", i);
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

        /* button tower clips definition */
        for (unsigned i = 0; i < CANT_TOWERS_BUTTONS; ++i) {
            for (unsigned j = 0; j < CANT_TOWERS_BUTTONS_STATES; ++j) {
                towerButtonsClips[i][j].x = i * 80;
                towerButtonsClips[i][j].y = j * 80 + 1;
                towerButtonsClips[i][j].w = 80;
                towerButtonsClips[i][j].h = 80;
            }
        }
    }

    loadPortalSprites();

    //If the map was loaded fine
    return tilesLoaded;
}

void GamePlayWindow::loadPortalSprites() {
    unsigned int x_i = 0;
    unsigned int y_i = 0;

    for (int i = 0; i < 30; ++i) {
        gSpriteClipsPortalBlue[i].x = i * 136;
        gSpriteClipsPortalBlue[i].y = 0;
        gSpriteClipsPortalBlue[i].w = 136;
        gSpriteClipsPortalBlue[i].h = 181;

        gSpriteClipsPortalRed[i].x = x_i * 136;
        gSpriteClipsPortalRed[i].y = y_i * 185;
        gSpriteClipsPortalRed[i].w = 136;
        gSpriteClipsPortalRed[i].h = 185;

        if (x_i < 10) {
            ++x_i;
        } else if (x_i == 10) {
            x_i = 0;
        }

        if (i == 9) {
            y_i = 1;
        } else if (i == 19) {
            y_i = 2;
        }
    }
}

void
GamePlayWindow::renderText(SDL_Rect &camera, std::string text, int x, int y) {
    SDL_Color textColor = {0xFF, 0xFF, 0, 0xFF}; // letra amarilla
    SDL_Color bgColor = {0, 0, 0, 0}; // fondo transparente (supuestamente)

    TTF_SetFontStyle(font, TTF_STYLE_BOLD);

    gPromptTextTexture.generateFromText(text, gRenderer, font, textColor,
                                        bgColor);

    gPromptTextTexture.render(gRenderer, x, y);
}

void
GamePlayWindow::handleMouseEvents(SDL_Rect camera, SDL_Event e) {
    if (e.type == SDL_MOUSEBUTTONDOWN) {
        Point point = Utils::getMouseRelativePoint(camera);

        /* Si hice click y tengo algún evento marcado para disparar
         * (por ejemplo, marqué un lugar para poner una torre, o quiero
         * poner una torre) manejo dicho evento. */
        switch (e.button.button) {
            //case GAME_EVENT_PUT_TOWER: {
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
            if (mousePosX <= 240) {
                typeOfUpgradeToDo = UPGRADE_RANGE;
            } else if (mousePosX <= 280) {
                typeOfUpgradeToDo = UPGRADE_DAMAGE;
            } else if (mousePosX <= 320) {
                typeOfUpgradeToDo = UPGRADE_REACH;
            } else {
                typeOfUpgradeToDo = UPGRADE_SLOWDOWN;
            }

            doUpgradeRequest();
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
        std::cout << "encontre una torre" << std::endl;
        doTowerInfoRequest();
    } else {
        towerDamageDataMessage = "";
        // no hago nada, por ahora
    }
}

bool GamePlayWindow::isAValidPutTowerRequest(Point &point) {
    return typeOfTowerToPut != -1
           && (SDL_GetTicks() - timeOfLastTowerPutted) >
              TIME_FOR_ENABLE_ACTION
           && isFirmTerrain(point);
}

void GamePlayWindow::doTowerInfoRequest() const {
    std::string request =
            MessageFactory::getTowerInfoRequest(clientId, matchName,
                                                towerSelected);
    sendToServer(request);
}

void GamePlayWindow::doUpgradeRequest() const {
    std::string request =
            MessageFactory::getUpgradeRequest(matchName,
                                              towerSelected,
                                              typeOfUpgradeToDo);

    sendToServer(request);
}

void GamePlayWindow::doPutTowerRequest(const Point &point) const {
    std::string request =
            MessageFactory::getPutTowerRequest(matchName,
                                               typeOfTowerToPut,
                                               point.x,
                                               point.y);

    sendToServer(request);
}

void GamePlayWindow::doCastSpellRequest(const Point &point) const {
    std::string request =
            MessageFactory::getCastSpellRequest(matchName,
                                                point.x,
                                                point.y);
    sendToServer(request);
}

void GamePlayWindow::sendToServer(const std::string &request) const {
    TextMessage toSend(request);
    toSend.sendTo(reinterpret_cast<Socket &>(*server));
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

        std::cout << "llego op: " << std::to_string(op)
                  << " al listener del juego..." << std::endl;

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
                    //bool isVisible = request.getAsBool(IS_VISIBLE_KEY);
                    bool isAlive = request.getAsBool(IS_ALIVE_KEY);
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
                        enemy->moveTo(scenarioPoint.x, scenarioPoint.y);
                        //enemy->setIsVisible(isVisible);
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
                // aplicar upgrade
                break;
            };
            default:
                response = "no reconocida";
        }

        std::cout << "notification: " << notification << std::endl;
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
        if (tileSet.at(tilePos).getType() == TILE_TOWER) {
            for (unsigned i = 0; i < towers.size(); ++i) {
                if (tileSet.at(tilePos).getTower() == towers[i]) {
                    towerSelected = i;
                    isATowerPoint = true;
                    break;
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

    std::string towerData("Tower data");
    std::string item;

    towerDamageDataMessage.assign("Daño: ");
    item = request.getAsString("damage");
    towerDamageDataMessage.append(item);

    towerRangeDataMessage.assign("Rango: ");
    item = request.getAsString("range");
    towerRangeDataMessage.append(item);

    towerReachDataMessage.assign("Alcance: ");
    item = request.getAsString("reach");
    towerReachDataMessage.append(item);

    towerSlowdownDataMessage.assign("Ralentizado: ");
    item = request.getAsString("slowDown");
    towerSlowdownDataMessage.append(item);
}

void GamePlayWindow::run() {
    unsigned int gameEndedTime = 0;
    bool gameEnded = false;

    //Start up SDL and create window
    if (!init()) {
        printf("Failed to initializde!\n");
    } else {
        //Load media
        if (!loadMedia()) {
            printf("Failed to load media!\n");
        } else {
            //Main loop flag
            bool quit = false;

            //Event handler
            SDL_Event e;

            //The dot that will be moving around on the screen
            Dot dot;

            //Level camera
            camera = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};

            //While application is running
            while (!quit) {
                gameEnded = gameStatus != GAME_STATUS_UNDECIDED;
                if (gameEnded) {
                    if (gameEndedTime == 0) {
                        gameEndedTime = SDL_GetTicks();
                    } else if (SDL_GetTicks() - gameEndedTime > 3000) {
                        quit = true;
                    }
                }

                //Handle events on queue
                while (SDL_PollEvent(&e) != 0) {
                    //User requests quit
                    if (e.type == SDL_QUIT) {
                        quit = true;
                    }

                    //Handle input for the dot
                    dot.handleEvent(e);

                    handleMouseEvents(camera, e);
                }

                handleServerNotifications(camera);

                //Move the dot
                dot.move();
                dot.setCamera(camera);

                //Clear screen
                SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 0xFF);
                SDL_RenderClear(gRenderer);

                //Render level
                renderLevel();

                // Rendereo punto para mover pantalla
                dot.render(dotTexture, camera, gRenderer);

                loadAnimables();
                animateAnimables();

                // Muestro los botones de las torres que puedo poner
                towerButtonsTexture.render(gRenderer, 1, 1);

                renderMessages();

                //Update screen
                SDL_RenderPresent(gRenderer);
            }
        }

        //Free resources and close SDL
        close();
    }/*

    server->shutdown(); // TODO ver si esto corresponde hacerlo acá...*/
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
    if (gameStatus == GAME_STATUS_WON) {
        renderText(camera, "Partida ganada...");
    } else if (gameStatus == GAME_STATUS_LOST) {
        renderText(camera, "Partida perdida...");
    }

    renderTimeMessages(camera);
    if (!towerDamageDataMessage.empty()) {
        renderText(camera, "Datos de la torre:", 1, 250);
        renderText(camera, towerDamageDataMessage, 1, 266);
        renderText(camera, towerRangeDataMessage, 1, 282);
        renderText(camera, towerReachDataMessage, 1, 298);
        renderText(camera, towerSlowdownDataMessage, 1, 314);
    }
}

/* Remuevo los animables del vector (ojo, siguen existiendo,
    * pero no están en el vector), ya que pueden ir cambiando de
    * posición en cada iteración, y los vuelvo a cargar y ordenar,
    * para volver a mostrarlos bien.
    */
void GamePlayWindow::loadAnimables() {
    // limpio la cola de prioridad...
    animables = std::priority_queue<Animable *, std::vector<Animable *>,
            lessThanByPoint>();

    // la cargo con torres...
    Tower *tp = nullptr;
    std::map<int, Tower *>::const_iterator tIt;
    for (tIt = towers.begin(); tIt != towers.end(); ++tIt) {
        tp = tIt->second;
        animables.push(tp);
    }

    // la cargo con enemigos...
    std::map<int, DrawableHorde>::const_iterator hIt;
    for (hIt = hordes.begin(); hIt != hordes.end(); ++hIt) {
        DrawableHorde horde = hIt->second;

        for (Enemy *enemy : horde.getEnemies()) {
            animables.push(enemy);
        }
    }

}

void GamePlayWindow::renderLevel() {
    for (int i = 0; i < TOTAL_TILES; ++i) {
        if (tileSet.at(i).itIsMarked()) {
            tileSet.at(i).verifyIfMustContinueMarked();

            if (!tileSet.at(i).itIsMarked()) {
                tileSet.at(i).setType(
                        TILE_FIRM); // todo refactor
            }
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

    toPut->setSprites();
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
        enemy->setSprites();
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
