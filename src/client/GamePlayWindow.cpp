#include "GamePlayWindow.h"

#include <iostream>
#include <fstream>
#include "../sdl/Dot.h"
#include "../sdl/Utils.h"
#include <SDL2/SDL_image.h>
#include <algorithm>
#include "../common/MessageFactory.h"
#include "../common/Protocol.h"
#include "../sdl/enemies/GreenDaemon.h"
#include "../sdl/enemies/Zombie.h"
#include "../common/TextMessage.h"

GamePlayWindow::GamePlayWindow(Socket *s,
                               SharedBuffer *in,
                               int cId,
                               std::vector<std::string> &elems,
                               std::string mn)
        : server(s),
          toReceive(in),
          clientId(cId),
          playerElements(elems),
          matchName(mn) {
    abmonibleTexture = new LTexture();
    blookHawkTexture = new LTexture();
    goatmanTexture = new LTexture();
    greenDaemonTexture = new LTexture();
    spectreTexture = new LTexture();
    zombieTexture = new LTexture();

    typeOfTowerToPut = -1;
    towerIdThatRequiresInfo = -1;
    isCastingSpells = false;
}

GamePlayWindow::~GamePlayWindow() {
    delete abmonibleTexture;
    delete blookHawkTexture;
    delete goatmanTexture;
    delete greenDaemonTexture;
    delete spectreTexture;
    delete zombieTexture;

    for (unsigned i = 0; i < hordes.size(); ++i) {
        std::vector<Enemy *> enemies =
                static_cast<std::vector<Enemy *> &&>(hordes[i]->getEnemies());
        for (unsigned j = 0; j < enemies.size(); ++j) {
            delete enemies[j];
        }
        delete hordes[i];
    }
}

void GamePlayWindow::run() {
    unsigned int gameEndedTime = 0;
    bool gameEnded;

    std::vector<Animable *> animables;

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
            SDL_Rect camera = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};

            initializeGameActors();

            //While application is running
            while (!quit) {
                gameEnded = gameWon || gameLoose;
                if (gameEnded && gameEndedTime == 0) {
                    gameEndedTime = SDL_GetTicks();
                }

                //Handle events on queue
                while (SDL_PollEvent(&e) != 0) {
                    //User requests quit
                    if (e.type == SDL_QUIT) {
                        quit = true;
                    }

                    /*if (gameEnded) {
                        if (SDL_GetTicks() - gameEndedTime > 3000) {
                            quit = true;
                        }
                    }*/

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
                for (int i = 0; i < TOTAL_TILES; ++i) {
                    if (tileSet[i]->itIsMarked()) {
                        tileSet[i]->verifyIfMustContinueMarked();

                        if (!tileSet[i]->itIsMarked()){
                            tileSet[i]->setType(TILE_FIRM); // HORRIBLE ESTO...REFACTORIZAR CUANDO SE PUEDA
                        }
                    }

                    tileSet[i]->render(camera, gTileClips, gRenderer,
                                       gTileTextures);
                }

                // Rendereo punto para mover pantalla
                dot.render(dotTexture, camera, gRenderer);

                /* Remuevo los animables del vector (ojo, siguen existiendo,
                 * pero no están en el vector), ya que pueden ir cambiando de
                 * posición en cada iteración, y los vuelvo a cargar y ordenar,
                 * para volver a mostrarlos bien.
                 */
                animables.clear();
                for (Tower *tower : towers) {
                    animables.push_back(reinterpret_cast<Animable *&&>(tower));
                }

                for (std::map<int, Horde *>::iterator it = hordes.begin();
                     it != hordes.end(); ++it) {
                    Horde *horde = it->second;

                    for (auto enemy : horde->getEnemies()) {
                        animables.push_back(
                                reinterpret_cast<Animable *&&>(enemy));
                    }
                }

                std::sort(animables.begin(), animables.end(),
                          Utils::animablesPositionComparator);

                for (Animable *animable : animables) {
                    animable->animate(camera);
                }

                // Muestro los botones de las torres que puedo poner
                towerButtonsTexture.render(gRenderer, 1, 1);

                if (gameEnded) {
                    if (gameWon) {
                        renderText(camera, "Partida ganada...");
                    } else {
                        renderText(camera, "Partida perdida...");
                    }
                }

                //Update screen
                SDL_RenderPresent(gRenderer);
            }
        }

        //Free resources and close SDL
        close();
    }

    server->shutdown(); // TODO ver si esto corresponde hacerlo acá...
}

bool GamePlayWindow::init() {
    //Initialization flag
    bool success = true;

    //Initialize SDL
    if (TTF_Init() < 0) {
        printf("TTF could not initialize! SDL Error: %s\n", TTF_GetError());
        success = false;
    } else if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        success = false;
    } else {
        //Set texture filtering to linear
        if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
            printf("Warning: Linear texture filtering not enabled!");
        }

        //Create window
        gWindow = SDL_CreateWindow("Tower4Defense", SDL_WINDOWPOS_UNDEFINED,
                                   SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
                                   SCREEN_HEIGHT, SDL_WINDOW_SHOWN |
                                                  SDL_WINDOW_RESIZABLE);
        if (gWindow == NULL) {
            printf("Window could not be created! SDL Error: %s\n",
                   SDL_GetError());
            success = false;
        } else {
            //Create renderer for window
            gRenderer = SDL_CreateRenderer(gWindow, -1,
                                           SDL_RENDERER_ACCELERATED |
                                           SDL_RENDERER_PRESENTVSYNC);
            if (gRenderer == NULL) {
                printf("Renderer could not be created! SDL Error: %s\n",
                       SDL_GetError());
                success = false;
            } else {
                //Initialize renderer color
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

                //Initialize PNG loading
                int imgFlags = IMG_INIT_PNG;
                if (!(IMG_Init(imgFlags) & imgFlags)) {
                    printf("SDL_image could not initialize! SDL_image Error: %s\n",
                           IMG_GetError());
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
    for (int i = 0; i < TOTAL_TILE_SPRITES; ++i) {
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

    //Load tile map
    if (!setTiles()) {
        printf("Failed to load tile set!\n");
        success = false;
    }

    return success;
}

void GamePlayWindow::initializeGameActors() {
    Tower *tower = new Tower(10, 3, gRenderer, gSpriteSheetTextureTower);
    tower->loadMedia();
    tower->setSprites();

    towers.push_back(tower);

    for (int i = 0; i < 2; ++i) {
        Horde *horde = new Horde();
        for (int j = 0; j < 3; ++j) {
            Enemy *greenDaemon = new GreenDaemon(-1, -1, gRenderer,
                                                 greenDaemonTexture);
            greenDaemon->setSprites();
            horde->addEnemy(greenDaemon);
        }
        std::pair<int, Horde *> pair(i, horde);
        hordes.insert(pair);

        Horde *horde2 = new Horde();
        for (int j = 0; j < 3; ++j) {
            Enemy *zombie = new Zombie(-1, -1, gRenderer,
                                       zombieTexture);
            zombie->setSprites();
            horde2->addEnemy(zombie);
        }
        std::pair<int, Horde *> pair2(i + 1, horde2);
        hordes.insert(pair2);
    }
}

void GamePlayWindow::close() {
    //Deallocate tiles
    for (int i = 0; i < TOTAL_TILES; ++i) {
        if (tileSet[i] == NULL) {
            delete tileSet[i];
            tileSet[i] = NULL;
        }
    }

    //Free loaded images
    dotTexture.free();
    for (int i = 0; i < TOTAL_TILE_SPRITES; ++i) {
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
    gWindow = NULL;
    gRenderer = NULL;

    //Quit SDL subsystems
    IMG_Quit();
    SDL_Quit();
}

bool GamePlayWindow::setTiles() {
    //Success flag
    bool tilesLoaded = true;

    //The tile offset
    int k = 0;

    //Open the map
    std::ifstream map("resources/maps/mapa1.map");

    //If the map couldn't be loaded
    if (!map.is_open()) {
        printf("Unable to load map file!\n");
        tilesLoaded = false;
    } else {
        //Initialize the tiles
        for (int i = 0; i < TILES_ROWS; ++i) {
            for (int j = 0; j < TILES_COLUMNS; ++j) {
                //Determines what kind of tile will be made
                int tileType = -1;

                //Read tile from map file
                map >> tileType;

                //If the was a problem in reading the map
                if (map.fail()) {
                    //Stop loading map
                    printf("Error loading map: Unexpected end of file!\n");
                    tilesLoaded = false;
                    break;
                }

                //If the number is a valid tile number
                if ((tileType >= 0)) {
                    k = i * TILES_COLUMNS + j;
                    tileSet[k] = new Tile(i, j, tileType);
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
            for (unsigned i = 0; i <= TILE_FIRM_MARKED; ++i) {
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
    }

    //Close the file
    map.close();

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

void GamePlayWindow::renderText(SDL_Rect &camera, std::string text) {
    SDL_Color textColor = {0xFF, 0xFF, 0, 0xFF}; // letra amarilla
    SDL_Color bgColor = {0, 0, 0, 0}; // fondo transparente (supuestamente)

    TTF_SetFontStyle(font, TTF_STYLE_BOLD);

    gPromptTextTexture.generateFromText(text, gRenderer, font, textColor,
                                        bgColor);

    gPromptTextTexture.render(gRenderer, 50, 50);
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

        if (mousePosX <= 80) {
            if (hasElement(STR_AIR)){
                typeOfTowerToPut = TOWER_AIR;
                std::cout << "seleccion torre aire para poner...." << std::endl;
            }
        } else if (mousePosX <= 160) {
            if (hasElement(STR_FIRE)){
                typeOfTowerToPut = TOWER_FIRE;
                std::cout << "seleccion torre fueg para poner...." << std::endl;
            }
        } else if (mousePosX <= 240) {
            if (hasElement(STR_WATER)){
                typeOfTowerToPut = TOWER_WATER;
                std::cout << "seleccion torre agua para poner...." << std::endl;
            }
        } else {
            if (hasElement(STR_EARTH)){
                typeOfTowerToPut = TOWER_EARTH;
                std::cout << "seleccion torre tier para poner...." << std::endl;
            }
        }
    } else if (typeOfTowerToPut != -1) {
        if (isFirmTerrain(point)) {
            std::string request =
                    MessageFactory::getPutTowerRequest(clientId,
                                                       typeOfTowerToPut,
                                                       point.x,
                                                       point.y);

            std::cout << "hice click en agregar torre...." << std::endl;

            sendToServer(request);
        }
    } else if (isCastingSpells) {
        if (isGroundTerrain(point)) {
            std::string request =
                    MessageFactory::getCastSpellRequest(clientId,
                                                        point.x,
                                                        point.y);
            TextMessage toSend(request);
            toSend.sendTo(reinterpret_cast<Socket &>(*server));
        }
    } else if (isATowerPoint(point)) {
        std::string request =
                MessageFactory::getTowerInfoRequest(clientId,
                                                    towerIdThatRequiresInfo);
        sendToServer(request);
    } else {
        std::cout << "********************" << std::endl;
        std::cout << "********************" << std::endl;
        std::cout << "hice click con el botón izquierdo, pero no disparé "
                "ningún evento de juego...";
        std::cout << "********************" << std::endl;
        std::cout << "********************" << std::endl;
    }
}

void GamePlayWindow::sendToServer(const std::string &request) const {
    TextMessage toSend(request);
    toSend.sendTo(reinterpret_cast<Socket &>(*server));
}

void GamePlayWindow::handleRightButtonClick(Point point) {
    if (isFirmTerrain(point)) {
        std::cout << "******************..." << std::endl;
        std::cout << "******************..." << std::endl;
        std::cout << "Mando mensaje para marcar tile..." << std::endl;
        std::cout << "******************..." << std::endl;
        std::cout << "******************..." << std::endl;

        std::string request =
                MessageFactory::getMarkTileRequest(matchName,
                                                   point.x,
                                                   point.y);

        TextMessage toSend(request);
        toSend.sendTo(reinterpret_cast<Socket &>(*server));
    }
}

void GamePlayWindow::handleServerNotifications(SDL_Rect camera) {
    int transactionsCounter = 0;
    std::string notification;

    Tower *tower = towers.at(0);

    while (toReceive->isProcessingYet() &&
           transactionsCounter < MAX_SERVER_NOTIFICATIONS_PER_FRAME) {
        ++transactionsCounter;
        notification = toReceive->getNextData();

        Message message;
        std::string response;

        message.deserialize(notification);

        int op = MessageFactory::getOperation(message);
        std::cout << "llego op: " << std::to_string(op)
                  << " al listener del juego..." << std::endl;

        switch (op) {
            case SERVER_NOTIFICATION_PUT_TOWER: {
                Point point = MessageFactory::getPoint(message);

                if (point.isPositive()) {
                    Point screenPoint = Utils::mapToScreen(point.x, point.y);
                    tower->setPosition(screenPoint.x, screenPoint.y);
                }
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
                // mostrar info de la torre por pantalla
                break;
            }
            case SERVER_NOTIFICATION_CAST_SPELL: {
                // reemplazar el tile por el valor que corresponde
                break;
            }
            case SERVER_NOTIFICATION_SCENARIO_STATUS: {
                std::vector<Message> messages =
                        MessageFactory::getMovementNotifications(message);

                for (Message aMessage : messages) {
                    Point scenarioPoint = MessageFactory::getPoint(aMessage);
                    // no dibujo cosas fuera del escenario...
                    if (scenarioPoint.isPositive()) {
                        int dir = MessageFactory::getDirection(aMessage);
                        int enemyId = MessageFactory::getEnemyId(aMessage);
                        int hordeId = MessageFactory::getHordeId(aMessage);

                        try {
                            Horde *horde = hordes.at(hordeId);
                            Enemy *enemy = horde->getEnemieAt(enemyId);
                            enemy->setDirection(dir);
                            enemy->moveTo(scenarioPoint.x, scenarioPoint.y);
                        } catch (...) {
                            std::cerr << "No es posible mover el enemigo "
                                      << std::to_string(enemyId);
                            std::cerr << " de la horda "
                                      << std::to_string(hordeId);
                        }
                    }
                }
                break;
            }
            case SERVER_NOTIFICATION_MATCH_ENDED: {
                toReceive->setClientProcessEnded(true);
                gameWon = true;
                break;
            }
            default:
                response = "no reconocida";
        }

        std::cout << "notification: " << notification << std::endl;
    }
}

bool GamePlayWindow::isClickOnTowerButton(int mousePosX, int mousePosY) const {
    return (
                   mousePosX >= TOWER_BUTTONS_X_POS
                   && mousePosX <= TOWER_BUTTONS_WIDTH + TOWER_BUTTONS_Y_POS
           )
           &&
           (
                   mousePosY >= TOWER_BUTTONS_Y_POS
                   && mousePosY <= TOWBER_BUTTONS_HEIGHT + TOWER_BUTTONS_Y_POS
           );
}

bool GamePlayWindow::isFirmTerrain(Point &point) {
    bool isFirmTerrain = false;

    if (point.isPositive()) {
        int tilePos = point.x * TILES_COLUMNS + point.y;
        isFirmTerrain = tileSet[tilePos]->getType() == TILE_FIRM;
    }

    return isFirmTerrain;
}

bool GamePlayWindow::isATowerPoint(Point &point) {
    bool isATowerPoint = false;

    if (point.isPositive()) {
        for (unsigned i = 0; i < towers.size(); ++i) {
            if (point.isEqualsTo(towers[i]->getPoint())) {
                isATowerPoint = true;
                towerIdThatRequiresInfo = i;
                break;
            } else {
                towerIdThatRequiresInfo = -1;
            }
        }
    }

    return isATowerPoint;
}

bool GamePlayWindow::isGroundTerrain(Point &point) {
    bool isGroundTerrain = false;

    if (point.isPositive()) {
        int tilePos = point.x * TILES_COLUMNS + point.y;
        isGroundTerrain = tileSet[tilePos]->getType() == TILE_DESERT
                          || tileSet[tilePos]->getType() == TILE_GRASS
                          || tileSet[tilePos]->getType() == TILE_ICE
                          || tileSet[tilePos]->getType() == TILE_LAVA;
    }

    return isGroundTerrain;
}

bool GamePlayWindow::hasElement(const std::string &element) const {
    return
        std::find(playerElements.begin(), playerElements.end(), element)
                != playerElements.end();
}

void GamePlayWindow::setToMarkedTile(Point &point) {
    if (point.isPositive()) {
        int tilePos = point.x * TILES_COLUMNS + point.y;
        tileSet[tilePos]->setType(TILE_FIRM_MARKED);
        tileSet[tilePos]->setIsMarked(true);
        tileSet[tilePos]->setMarkedTime(SDL_GetTicks());
    }
}

void GamePlayWindow::setToFirmTile(Point &point) {
    if (point.isPositive()) {
        int tilePos = point.x * TILES_COLUMNS + point.y;
        tileSet[tilePos]->setType(TILE_FIRM);
        tileSet[tilePos]->setIsMarked(false);
        tileSet[tilePos]->setMarkedTime(0);
    }
}


