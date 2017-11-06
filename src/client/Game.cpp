#include "Game.h"

#include <iostream>
#include <fstream>
#include "../sdl/Dot.h"
#include "../sdl/Utils.h"
#include <SDL2/SDL_image.h>
#include "../common/MessageFactory.h"
#include "../common/Protocol.h"

Game::Game(SharedBuffer &in, SharedBuffer &out, int cId)
        : dataFromServer(in), dataToServer(out), clientId(cId) {}

Game::~Game() {}

bool Game::init() {
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

bool Game::loadMedia() {
    //Loading success flag
    bool success = true;

    //Load dot texture
    if (!gDotTexture.loadFromFile("dot.bmp", gRenderer)) {
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

    gSpriteSheetTexturePortalBlue
            .loadFromFile("images/sprites/portal-blue2.png", gRenderer);
    gSpriteSheetTexturePortalRed
            .loadFromFile("images/sprites/portal-red.png", gRenderer);

    //Load tile map
    if (!setTiles()) {
        printf("Failed to load tile set!\n");
        success = false;
    }

    return success;
}

void Game::close() {
    //Deallocate tiles
    for (int i = 0; i < TOTAL_TILES; ++i) {
        if (tileSet[i] == NULL) {
            delete tileSet[i];
            tileSet[i] = NULL;
        }
    }

    //Free loaded images
    gDotTexture.free();
    for (int i = 0; i < TOTAL_TILE_SPRITES; ++i) {
        gTileTextures[i].free();
    }

    //Destroy window
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    gRenderer = NULL;

    //Quit SDL subsystems
    IMG_Quit();
    SDL_Quit();
}

bool Game::setTiles() {
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

void Game::loadPortalSprites() {
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

void Game::handleMouseEvents(SDL_Rect camera, std::string mov_description, SDL_Event e) {
    if (e.type == SDL_MOUSEBUTTONDOWN) {
        Point point = Utils::getMouseRelativePoint(camera);

        //matarBichoSiLeHiceClick(camera, enemy); // para pruebas

        /* Si hice click y tengo algún evento marcado para disparar
         * (por ejemplo, marqué un lugar para poner una torre, o quiero
         * poner una torre) manejo dicho evento. */
        switch (eventDispatched) {
            case GAME_EVENT_PUT_TOWER: {
                std::string request;
                request = MessageFactory::getPutTowerRequest(clientId, point.x, point.y, true);
                dataToServer.addData(request);
                break;
            }
            case GAME_EVENT_KILL_ENEMY: {
                //enemy.kill();
                break;
            }
            default:
                if (point.isPositive()) {
                    int tilePos = point.x * TILES_COLUMNS + point.y;
                    tileSet[tilePos]->handleEvent(e, mov_description);
                }
        }
    }
}

void Game::matarBichoSiLeHiceClick(const SDL_Rect &camera, Enemy &enemy) {
    int mousePosX;
    int mousePosY;
    SDL_GetMouseState(&mousePosX, &mousePosY);

    mousePosX += camera.x;
    mousePosY += camera.y;

    DecimalPoint otherPoint = Utils::screenToMapDecimal(mousePosX, mousePosY);

    SDL_Rect pointRect = {int(otherPoint.x * CARTESIAN_TILE_WIDTH), int(otherPoint.y * CARTESIAN_TILE_HEIGHT), 1, 1};

    if (Utils::checkCollision(pointRect, enemy.getWalkBox())) {
        enemy.quitLifePoints(50);

        if (!enemy.itIsAlive()) {
            eventDispatched = 3;
        }
    } else {
        eventDispatched = 1;
    }

}

void Game::handleServerNotifications(SDL_Rect camera, Tower &tower) {
    int transactionsCounter = 0;
    std::string notification;

    while (dataFromServer.hasData() && transactionsCounter < MAX_SERVER_NOTIFICATIONS_PER_FRAME) {
        ++transactionsCounter;
        notification = dataFromServer.getNextData();

        Message message;
        std::string response;

        message.deserialize(notification);

        int op = MessageFactory::getOperation(message);

        switch (op) {
            case SERVER_NOTIFICATION_PUT_TOWER: {
                Point point = MessageFactory::getPoint(message);

                if (point.isPositive()) {
                    tower.setPosition(point.x, point.y);
                }
                break;
            }
            case SERVER_NOTIFICATION_MOVE_ENEMY: {
                Point point = MessageFactory::getPoint(message);
                Point finalPoint(1279, 400);
                int dir = MessageFactory::getDirection(message);
                int enemyId = MessageFactory::getEnemyId(message);

                Enemy *enemy = enemies.at(enemyId);

                bool towerIsShooting =
                        enemy->itIsAlive() &&
                        enemy->getCollisionCircle().hasCollisionWith(tower.getCollisionCircle());
                if (towerIsShooting) {
                    tower.setIsShooting(true);
                    int shotDamage = tower.getShotDamage();
                    enemy->quitLifePoints(shotDamage);
                    tower.sumExperiencePoints(shotDamage);
                    std::cout << "la torre tiene " << tower.getExperiencePoints() << " de experiencia." << std::endl;
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

                    for (Enemy *enemy1 : enemies){
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
                break;
        }
        default:
            response = "no reconocida";
    }

    std::cout << "notification: " << notification << std::endl;
}

}

void Game::run() {
    std::string mov_description;

    unsigned int gameEndedTime = 0;
    bool gameEnded = false;

    //Start up SDL and create window
    if (!init()) {
        printf("Failed to initializde!\n");
    } else {
        for (int x= 0; x < 5; ++x){
            Enemy *abmonible = new Abmonible(0, 0, gRenderer);
            abmonible->loadMedia();
            abmonible->setSprites();
            enemies.push_back(abmonible);
        }

        Tower tower(0, 0, gRenderer, gSpriteSheetTextureTower);
        tower.loadMedia();
        tower.setSprites();

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

                    eventDispatched = 1;
                    handleMouseEvents(camera, mov_description, e);
                    eventDispatched = 0;
                }

                handleServerNotifications(camera, tower);

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
                dot.render(gDotTexture, camera, gRenderer);

                tower.animate(camera);

                for (Enemy *enemy : enemies){
                    enemy->animate(camera);
                }

                if (gameEnded) {
                    if (gameWon) {
                        renderText(camera, "Partida ganada :)");
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
}

void Game::renderText(SDL_Rect &camera, std::string text) {
    SDL_Color textColor = {0xFF, 0xFF, 0, 0xFF}; // letra amarilla
    SDL_Color bgColor = {0, 0, 0, 0}; // fondo transparente (supuestamente)

    TTF_SetFontStyle(font, TTF_STYLE_BOLD);

    gPromptTextTexture.generateFromText(text, gRenderer, font, textColor, bgColor);

    gPromptTextTexture.render(gRenderer, 50, 50);
}

