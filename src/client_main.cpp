#include <SDL2/SDL.h>
#include <gtkmm.h>
#include <iostream>
#include <stdio.h>
#include <gtkmm-3.0/gtkmm/socket.h>

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

Gtk::Window* pWindow = nullptr;

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The surface contained by the window
SDL_Surface* gScreenSurface = NULL;

//The image we will load and show
SDL_Surface* gHelloWorld = NULL;

static void on_btnMarcarZona_clicked() {
    if (pWindow) {
        std::cout << "marqué zona..." << std::endl;
    }
}

static void on_btnColocarTorre_clicked() {
    if (pWindow) {
        std::cout << "coloqué torre..." << std::endl;
    }
}

bool loadMedia() {
    //Loading success flag
    bool success = true;

    //Load splash image
    gHelloWorld = SDL_LoadBMP("hello_world.bmp");
    if (gHelloWorld == NULL) {
        printf("Unable to load image %s! SDL Error: %s\n", "02_getting_an_image_on_the_screen/hello_world.bmp", SDL_GetError());
        success = false;
    }

    return success;
}

gboolean do_sdl_stuff(gpointer data) {

    //Load media
    if (!loadMedia()) {
        printf("Failed to load media!\n");
    } else {
        //Apply the image
        SDL_BlitSurface(gHelloWorld, NULL, gScreenSurface, NULL);

        //Update the surface
        SDL_UpdateWindowSurface(gWindow);

        //Wait two seconds
        //SDL_Delay(2000);
    }

    return TRUE;
}

void clear(void) {
    /* Put clear userevent on queue */
    SDL_Event event;
    event.type = SDL_USEREVENT;
    event.user.code = 0;
    SDL_PushEvent(&event);
}

int main(int argc, char **argv) {
    auto app = Gtk::Application::create(argc, argv);

    //Load the GtkBuilder file and instantiate its widgets:
    auto refBuilder = Gtk::Builder::create();
    try {
        refBuilder->add_from_file("glade/game-window.glade");
    } catch (const Glib::FileError& ex) {
        std::cerr << "FileError: " << ex.what() << std::endl;
        return 1;
    } catch (const Glib::MarkupError& ex) {
        std::cerr << "MarkupError: " << ex.what() << std::endl;
        return 1;
    } catch (const Gtk::BuilderError& ex) {
        std::cerr << "BuilderError: " << ex.what() << std::endl;
        return 1;
    }

    //Get the GtkBuilder-instantiated Dialog:
    refBuilder->get_widget("mainGrid", pWindow);

    // SDL
    /* Make SDL windows appear inside socket window */
    Gtk::Socket *sock = nullptr;
    char winhack[1024];

    auto socket = Gtk::manage(new Gtk::Socket());
    //add(*socket);

    pWindow->add(*socket);

    //refBuilder->get_widget("pantallaSDL", sock);

    snprintf(winhack, sizeof winhack, "SDL_WINDOWID=%i", (int) sock->get_id());

//    SDL_putenv(winhack);

    //Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    } else {
        //Create window
        gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (gWindow == NULL) {
            printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
            return EXIT_FAILURE;
        } else {
            //Get window surface
            gScreenSurface = SDL_GetWindowSurface(gWindow);
        }
    }


    /* Clear the SDL window */
    clear();

    /* Setup SDL event handlig timer */
    g_timeout_add(100, do_sdl_stuff, gWindow);

    //


    if (pWindow) {
        //Get the GtkBuilder-instantiated Button, and connect a signal handler:
        Gtk::Button* pBtnMarcarZona = nullptr;
        refBuilder->get_widget("btnMarcarZona", pBtnMarcarZona);

        Gtk::Button* pBtnColocarTorre = nullptr;
        refBuilder->get_widget("btnColocarTorre", pBtnColocarTorre);

        if (pBtnMarcarZona) {
            //refBuilder->get_widget("image", pImage);
            pBtnMarcarZona->signal_clicked().connect(sigc::ptr_fun(on_btnMarcarZona_clicked));
        }

        if (pBtnColocarTorre) {
            //refBuilder->get_widget("gif", pGif);
            pBtnColocarTorre->signal_clicked().connect(sigc::ptr_fun(on_btnColocarTorre_clicked));
        }

        app->run(*pWindow);
    }

    delete pWindow;

    return 0;
}