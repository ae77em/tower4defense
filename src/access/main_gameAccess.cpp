#include <gtkmm.h>
#include <iostream>
#include "AccessGameHandler.h"

Gtk::Window *pWindow = nullptr;


std::vector<GameWindowHandler*> gameWindowHandlers;

static void on_cmbMapas_changed(Gtk::ComboBoxText* combo){
    std::cout << "Elemento elegido: " << combo->get_active_text() << std::endl;
}

static void on_btnCrearPartida_clicked() {
    if (pWindow) {

    }
}

static void on_btnJugar_clicked() {
    GameWindowHandler *gwh = new GameWindowHandler();

    gameWindowHandlers.push_back(gwh);

    gwh->start();
}

int main(int argc, char **argv) {
    auto app = Gtk::Application::create(argc, argv);

    //Load the GtkBuilder file and instantiate its widgets:
    auto refBuilder = Gtk::Builder::create();
    try {
        refBuilder->add_from_file("resources/glade/game-access.glade");
    }
    catch (const Glib::FileError &ex) {
        std::cerr << "FileError: " << ex.what() << std::endl;
        return 1;
    }
    catch (const Glib::MarkupError &ex) {
        std::cerr << "MarkupError: " << ex.what() << std::endl;
        return 1;
    }
    catch (const Gtk::BuilderError &ex) {
        std::cerr << "BuilderError: " << ex.what() << std::endl;
        return 1;
    }

    //Get the GtkBuilder-instantiated Dialog:
    refBuilder->get_widget("mainWindow", pWindow);
    if (pWindow) {
        //Get the GtkBuilder-instantiated Button, and connect a signal handler:
        Gtk::Button *pBtnCrearPartida = nullptr;
        refBuilder->get_widget("btnCrearPartida", pBtnCrearPartida);

        if (pBtnCrearPartida) {
            pBtnCrearPartida->signal_clicked().connect(sigc::ptr_fun(on_btnCrearPartida_clicked));
        }


        Gtk::Button *pbtnJugar = nullptr;
        refBuilder->get_widget("btnJugar", pbtnJugar);

        if (pbtnJugar) {
            pbtnJugar->signal_clicked().connect(sigc::ptr_fun(on_btnJugar_clicked));
        }


        Gtk::ComboBoxText *pcmbMapas = nullptr;
        refBuilder->get_widget("cmbMapas", pcmbMapas );

        if (pcmbMapas) {
            pcmbMapas->append("mapa1");
            pcmbMapas->append("mapa2");
            pcmbMapas->signal_changed().connect(sigc::bind(sigc::ptr_fun(on_cmbMapas_changed), pcmbMapas));
        }

        app->run(*pWindow);
    }

    delete pWindow;

    for (unsigned int i = 0; i < gameWindowHandlers.size(); ++i){
        gameWindowHandlers[i]->join();
        delete gameWindowHandlers[i];
    }

    return 0;
}