#include "Game.h"

#include <gtkmm.h>
#include <iostream>
#include <fstream>
#include "Dot.h"
#include "../sdl/Utils.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../common/MessageFactory.h"

Game::Game(SharedBuffer &in, SharedBuffer &out) : dataFromServer(in), dataToServer(out) {}

Game::~Game() {}

bool Game::init() {
    //Initialization flag
    bool success = true;

    //Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
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
            }
        }
    }

    return success;
}

bool Game::loadMedia(Tile *tiles[]) {
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

    gSpriteSheetTexturePortalBlue.loadFromFile("images/sprites/portal-blue2"
                                                       ".png", gRenderer);
    gSpriteSheetTexturePortalRed.loadFromFile("images/sprites/portal-red.png", gRenderer);

    //Load tile map
    if (!setTiles(tiles)) {
        printf("Failed to load tile set!\n");
        success = false;
    }

    return success;
}

void Game::close(Tile *tiles[]) {
    //Deallocate tiles
    for (int i = 0; i < TOTAL_TILES; ++i) {
        if (tiles[i] == NULL) {
            delete tiles[i];
            tiles[i] = NULL;
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

bool Game::setTiles(Tile *tiles[]) {
    //Success flag
    bool tilesLoaded = true;

    //The tile offsets
    int x = 0, y = 0, k = 0;

    //Open the map
    std::ifstream map("lazy.map");

    //If the map couldn't be loaded
    if (!map.is_open()) {
        printf("Unable to load map file!\n");
        tilesLoaded = false;
    } else {
        //Initialize the tiles
        for (int j = 0; j < TILES_COLUMNS; ++j) {
            for (int i = 0; i < TILES_ROWS; ++i) {
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
                x = i;
                y = j;

                //If the number is a valid tile number
                if ((tileType >= 0) /*&& (tileType < TOTAL_TILE_SPRITES)*/) {
                    k = i * TILES_COLUMNS + j;
                    tiles[k] = new Tile(x, y);
                }                //If we don't recognize the tile type
                else {
                    //Stop loading map
                    printf("Error loading map: Invalid tile type at %d!\n", i);
                    tilesLoaded = false;
                    break;
                }
            }
        }

        //Clip the sprite sheet
        if (tilesLoaded) {
            gTileClips[GRASS].x = 0;
            gTileClips[GRASS].y = 0;
            gTileClips[GRASS].w = ISO_TILE_WIDTH;
            gTileClips[GRASS].h = ISO_TILE_HEIGHT;

            gTileClips[EARTH_TOWER].x = 0;
            gTileClips[EARTH_TOWER].y = 0;
            gTileClips[EARTH_TOWER].w = ISO_TILE_WIDTH;
            gTileClips[EARTH_TOWER].h = 194;
        }

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

    //Close the file
    map.close();

    //If the map was loaded fine
    return tilesLoaded;
}

void Game::handleMouseEvents(Tile *const *tileSet,
                             const SDL_Rect &camera,
                             std::string &mov_description,
                             SDL_Event &e) const {
    if (e.type == SDL_MOUSEBUTTONDOWN) {
        Point point = Utils::getMouseRelativePoint(camera);

        if (point.isPositive()) {
            /* Si hice click y tengo algún evento marcado para disparar
             * (por ejemplo, marqué un lugar para poner una torre, o quiero
             * poner una torre) manejo dicho evento. */
            switch (currentEventDispatched) {
                case GAME_EVENT_PUT_TOWER: {
                    std::cout << "voy a mandar un pedido para poner una torre..." << std::endl;
                    std::string request;
                    request = MessageFactory::getPutTowerRequest(point.x, point.y);
                    dataToServer.addData(request);
                    break;
                }
                default:
                    int tilePos = point.x * TILES_COLUMNS + point.y;
                    tileSet[tilePos]->handleEvent(e, mov_description);
            }
        }
    }
}

void Game::handleServerNotifications(Tile *tiles[], SDL_Rect camera) {
    int transactionsCounter = 0;
    std::string notification;

    while (dataFromServer.hasData() && transactionsCounter < MAX_SERVER_NOTIFICATIONS_PER_FRAME){
        notification = dataFromServer.getNextData();

        std::cout << "notification: " << notification << std::endl;
    }
}



void Game::run() {
    std::string mov_description;

    //Start up SDL and create window
    if (!init()) {
        printf("Failed to initialize!\n");
    } else {
        //The level tiles
        Tile *tileSet[TOTAL_TILES];

        Tile portalBlue(0, 0);
        Tile portalRed(5, 5);

        int frameToDraw = 0;

        //Load media
        if (!loadMedia(tileSet)) {
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

            //int frame = 0;

            //While application is running
            while (!quit) {
                //Handle events on queue
                mov_description = "";
                while (SDL_PollEvent(&e) != 0) {
                    //User requests quit
                    if (e.type == SDL_QUIT) {
                        quit = true;
                    }

                    //Handle input for the dot
                    dot.handleEvent(e, mov_description);

                    currentEventDispatched = 1;
                    handleMouseEvents(tileSet, camera, mov_description, e);
                    currentEventDispatched = 0;
                }

                handleServerNotifications(tileSet, camera);

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

                frameToDraw = (SDL_GetTicks() / 75) % 30;

                SDL_Rect *currentClip = &gSpriteClipsPortalBlue[frameToDraw];
                portalBlue.renderSprite(camera, currentClip, gRenderer,
                                        &gSpriteSheetTexturePortalBlue);

                currentClip = &gSpriteClipsPortalRed[frameToDraw];
                portalRed.renderSprite(camera, currentClip, gRenderer,
                                       &gSpriteSheetTexturePortalRed);

                //Update screen
                SDL_RenderPresent(gRenderer);
            }
        }

        //Free resources and close SDL
        close(tileSet);
    }
}



