#include "GamePlayWindow.h"

#include <iostream>
#include <fstream>
#include "../sdl/Dot.h"
#include "../sdl/Utils.h"
#include <SDL2/SDL_image.h>
#include <algorithm>
#include "../common/MessageFactory.h"
#include "../common/Protocol.h"

GamePlayWindow::GamePlayWindow(Socket *s, SharedBuffer *in, SharedBuffer *out, int cId)
        : server(s), toReceive(in), toSend(out), clientId(cId) {

    abmonibleTexture = new LTexture();
    blookHawkTexture = new LTexture();
    goatmanTexture = new LTexture();
    greenDaemonTexture = new LTexture();
    spectreTexture = new LTexture();
    zombieTexture = new LTexture();
}

GamePlayWindow::~GamePlayWindow() {
    delete abmonibleTexture;
    delete blookHawkTexture;
    delete goatmanTexture;
    delete greenDaemonTexture;
    delete spectreTexture;
    delete zombieTexture;

    for (unsigned i = 0; i < hordes.size(); ++i) {
        std::vector<Enemy *> enemies = static_cast<std::vector<Enemy *> &&>(hordes[i]->getEnemies());
        for (unsigned j = 0; j < enemies.size(); ++j) {
            delete enemies[j];
        }
        delete hordes[i];
    }
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

    bluePortalTexture.loadFromFile("images/sprites/portal-blue2.png", gRenderer);
    redPortalTexture.loadFromFile("images/sprites/portal-red.png", gRenderer);

    abmonibleTexture->loadFromFile("images/sprites/enemy-abominable.png", gRenderer, 0xFF, 0x00, 0x99);
    blookHawkTexture->loadFromFile("images/sprites/enemy-blood-hawk.png", gRenderer, 0xAA, 0xAA, 0xAA);
    goatmanTexture->loadFromFile("images/sprites/enemy-goatman.png", gRenderer, 0xAA, 0xAA, 0xAA);
    greenDaemonTexture->loadFromFile("images/sprites/enemy-green-daemon.png", gRenderer, 0xAA, 0xAA, 0xAA);
    spectreTexture->loadFromFile("images/sprites/enemy-spectre.png", gRenderer, 0xAA, 0xAA, 0xAA);
    zombieTexture->loadFromFile("images/sprites/enemy-zombie.png", gRenderer, 0xAA, 0xAA, 0xAA);

    //Load tile map
    if (!setTiles()) {
        printf("Failed to load tile set!\n");
        success = false;
    }

    return success;
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

                //Move to next tile spot

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
            for (int i = 0; i < TILE_EARTH_TOWER; ++i) {
                gTileClips[i].x = 0;
                gTileClips[i].y = 0;
                gTileClips[i].w = ISO_TILE_WIDTH;
                gTileClips[i].h = ISO_TILE_HEIGHT;
            }

            gTileClips[TILE_EARTH_TOWER].x = 0;
            gTileClips[TILE_EARTH_TOWER].y = 0;
            gTileClips[TILE_EARTH_TOWER].w = ISO_TILE_WIDTH;
            gTileClips[TILE_EARTH_TOWER].h = 194;
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

    gPromptTextTexture.generateFromText(text, gRenderer, font, textColor, bgColor);

    gPromptTextTexture.render(gRenderer, 50, 50);
}

void GamePlayWindow::handleMouseEvents(SDL_Rect camera, std::string mov_description, SDL_Event e) {
    if (e.type == SDL_MOUSEBUTTONDOWN) {
        Point point = Utils::getMouseRelativePoint(camera);

        /* Si hice click y tengo algún evento marcado para disparar
         * (por ejemplo, marqué un lugar para poner una torre, o quiero
         * poner una torre) manejo dicho evento. */
        switch (e.button.button) {
            //case GAME_EVENT_PUT_TOWER: {
            case SDL_BUTTON_LEFT: {
                std::string request;
                request = MessageFactory::getPutTowerRequest(clientId, point.x, point.y, true);
                toSend->addData(request);
                break;
            }
            case SDL_BUTTON_RIGHT: {
                //enemy.kill();
                break;
            }
            default:
                std::cout << "se hizo click con algún botón que no se usa.";
                /*if (point.isPositive()) {
                    int tilePos = point.x * TILES_COLUMNS + point.y;
                    tileSet[tilePos]->handleEvent(e, mov_description);
                }*/
        }
    }
}

void GamePlayWindow::handleServerNotifications(SDL_Rect camera) {
    int transactionsCounter = 0;
    std::string notification;

    Tower *tower = towers.at(0);

    while (toReceive->isProcessingYet() && transactionsCounter < MAX_SERVER_NOTIFICATIONS_PER_FRAME) {
        ++transactionsCounter;
        notification = toReceive->getNextData();

        Message message;
        std::string response;

        message.deserialize(notification);

        int op = MessageFactory::getOperation(message);
        std::cout << "llego op: " << std::to_string(op) << " al listener del juego..." << std::endl;

        switch (op) {
            case SERVER_NOTIFICATION_PUT_TOWER: {
                Point point = MessageFactory::getPoint(message);

                if (point.isPositive()) {
                    tower->setPosition(point.x, point.y);
                }
                break;
            }
            case SERVER_NOTIFICATION_SCENARIO_STATUS: {
                std::vector<Message> messages = MessageFactory::getMovementNotifications(message);

                for (Message aMessage : messages) {
                    Point point = MessageFactory::getPoint(aMessage);
                    Point finalPoint(1279, 400);
                    int dir = MessageFactory::getDirection(aMessage);
                    int enemyId = MessageFactory::getEnemyId(aMessage);
                    int hordeId = MessageFactory::getHordeId(aMessage);

                    try {
                        Horde *horde = hordes.at(hordeId);
                        Enemy *enemy = horde->getEnemieAt(enemyId);
                        enemy->setDirection(dir);
                        enemy->moveTo(point.x, point.y);
                    } catch (...) {
                        std::cerr << "No es posible mover el enemigo " << std::to_string(enemyId);
                        std::cerr << " de la horda " << std::to_string(hordeId);
                    }

                    /* ESTA ES LÓGICA DE NEGOCIO, Y VA EN EL SERVER...
                     * bool towerIsShooting =
                            enemy->itIsAlive() &&
                            enemy->getCollisionCircle().hasCollisionWith(tower.getCollisionCircle());
                    if (towerIsShooting) {
                        tower.setIsShooting(true);
                        int shotDamage = tower.getShotDamage();
                        enemy->quitLifePoints(shotDamage);
                        tower.sumExperiencePoints(shotDamage);
                        std::cout << "la torre tiene " << tower.getExperiencePoints() << " de experiencia."
                                  << std::endl;
                    } else {
                        tower.setIsShooting(false);
                    }

                    if (enemy->itIsAlive()) {
                        enemy->setDirection(dir);
                        enemy->moveTo(point.x, point.y);
                    } else if (towerIsShooting) {
                        tower.sumExperiencePoints(enemy->getBonus());
                        std::cout << "la torre sumó bonus de " << enemy->getBonus() << " puntos de experiencia."
                                  << std::endl;

                        std::vector<Enemy*> enemies = static_cast<std::vector<Enemy *> &&>(horde->getEnemies());
                        for (Enemy *enemy1 : enemies) {
                            if (enemy1->itIsAlive()) {
                                gameWon = false;
                                break;
                            } else {
                                gameWon = true;
                            }
                        }
                    }

                    if (enemy->itIsAlive()) {
                        if (point.x >= finalPoint.x && point.y >= finalPoint.y) {
                            gameLoose = true;
                        }
                    }

                    std::cout << "la torre tiene " << tower.getExperiencePoints() << " de experiencia." << std::endl;
                     */
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

void GamePlayWindow::run() {
    std::string mov_description;

    unsigned int gameEndedTime = 0;
    bool gameEnded = false;

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
                mov_description = "";
                while (SDL_PollEvent(&e) != 0) {
                    //User requests quit
                    if (e.type == SDL_QUIT) {
                        quit = true;
                    }

                    if (gameEnded) {
                        if (SDL_GetTicks() - gameEndedTime > 3000) {
                            quit = true;
                        }
                    }

                    //Handle input for the dot
                    dot.handleEvent(e, mov_description);

                    handleMouseEvents(camera, mov_description, e);
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
                    tileSet[i]->render(camera, gTileClips, gRenderer, gTileTextures);
                }

                //Render dot
                dot.render(dotTexture, camera, gRenderer);

                animables.clear();
                for (Tower *tower : towers) {
                    //tower->animate(camera);
                    animables.push_back(reinterpret_cast<Animable *&&>(tower));
                }

                for (std::map<int, Horde *>::iterator it = hordes.begin(); it != hordes.end(); ++it) {
                    Horde *horde = it->second;

                    for (auto enemy : horde->getEnemies()) {
                        //enemy->animate(camera);
                        animables.push_back(reinterpret_cast<Animable *&&>(enemy));
                    }
                }

                std::sort(animables.begin(), animables.end(), Utils::animablesPositionComparator);

                for (Animable *animable : animables) {
                    animable->animate(camera);
                }

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

void GamePlayWindow::initializeGameActors() {
    Tower *tower = new Tower(10, 3, gRenderer, gSpriteSheetTextureTower);
    tower->loadMedia();
    tower->setSprites();

    towers.push_back(tower);

    for (int i = 0; i < 2; ++i) {
        Horde *horde = new Horde();
        for (int j = 0; j < 3; ++j) {
            Enemy *abmonible = new Abmonible(-1, -1, gRenderer, abmonibleTexture);
            abmonible->setSprites();
            abmonible->setTexture(abmonibleTexture);
            horde->addEnemy(abmonible);
        }
        std::pair<int, Horde *> pair(i, horde);
        hordes.insert(pair);
    }



    /*std::pair<int, std::vector<Enemy *>> pair2(1, *enemies);
    hordes.insert(pair2);*/

    /*Enemy *bloodHawk = new BloodHawk(1, 0, gRenderer, blookHawkTexture);
    bloodHawk->setSprites();
    bloodHawk->setTexture(blookHawkTexture);
    Enemy *goatman = new Goatman(2, 0, gRenderer, goatmanTexture);
    goatman->setSprites();
    goatman->setTexture(goatmanTexture);
    Enemy *greenDaemon = new GreenDaemon(3, 0, gRenderer, greenDaemonTexture);
    greenDaemon->setSprites();
    greenDaemon->setTexture(greenDaemonTexture);
    Enemy *spectre = new Spectre(4, 0, gRenderer, spectreTexture);
    spectre->setSprites();
    spectre->setTexture(spectreTexture);
    Enemy *zombie = new Zombie(5, 0, gRenderer, zombieTexture);
    zombie->setSprites();
    zombie->setTexture(zombieTexture);

    enemies.push_back(bloodHawk);
    enemies.push_back(goatman);
    enemies.push_back(greenDaemon);
    enemies.push_back(spectre);
    enemies.push_back(zombie);*/


}


