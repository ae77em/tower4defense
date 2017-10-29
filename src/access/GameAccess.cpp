#include "GameAccess.h"
#include <gtkmm.h>
#include <iostream>


void GameAccess::on_cmbMapas_changed(){
    std::cout << "Elemento elegido: " << pcmbMapas->get_active_text() << std::endl;
}

void GameAccess::on_btnCrearPartida_clicked(Glib::ustring s) {
    std::cout << "string: " << s << std::endl;
}

void GameAccess::on_btnJugar_clicked(Glib::ustring s) {
    std::cout << "string: " << s << std::endl;
/*
    GameWindowHandler *gwh = new GameWindowHandler(host, port);

    gameWindowHandlers.push_back(gwh);

    gwh->start();
*/
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
        //Get the GtkBuilder-instantiated Button, and connect a signal handler:
        refBuilder->get_widget("btnCrearPartida", pBtnCrearPartida);

        if (pBtnCrearPartida) {
            //pBtnCrearPartida->signal_clicked().connect(sigc::ptr_fun(on_btnCrearPartida_clicked));
            pBtnCrearPartida->signal_clicked()
                    .connect(
                            sigc::bind<Glib::ustring>(
                                    sigc::mem_fun(*this, &GameAccess::on_btnCrearPartida_clicked), "button 1")
                    );
        }


        refBuilder->get_widget("btnJugar", pbtnJugar);

        if (pbtnJugar) {
            //pbtnJugar->signal_clicked().connect(sigc::ptr_fun(on_btnJugar_clicked));
            pbtnJugar->signal_clicked()
                    .connect(
                            sigc::bind<Glib::ustring>(
                                    sigc::mem_fun(*this, &GameAccess::on_btnJugar_clicked), "button 1")
                    );
        }


        refBuilder->get_widget("cmbMapas", pcmbMapas);

        if (pcmbMapas) {
            pcmbMapas->append("aa");
            //pcmbMapas->signal_changed().connect(bind(sigc::ptr_fun(on_cmbMapas_changed), pcmbMapas));
            pcmbMapas->signal_changed()

                    .connect(
                            sigc::mem_fun(*this,
                                          &GameAccess::on_cmbMapas_changed
                            )
                    );
        }

        app->run(*pWindow);
    }

    delete pWindow;

    for (unsigned int i = 0; i < gameWindowHandlers.size(); ++i){
        gameWindowHandlers[i]->join();
        delete gameWindowHandlers[i];
    }
}

void GameAccess::setMaps(const std::vector<std::string> &maps) {
    pcmbMapas->clear();
    for (std::string map : maps){
        pcmbMapas->append("bb");
    }
    pcmbMapas->queue_draw();

}

bool GameAccess::isCmbMapsLoaded(){
    return pcmbMapas != nullptr;
}
