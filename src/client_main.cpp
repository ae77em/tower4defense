#include <gtkmm.h>
#include <iostream>
#include <string>
#include <giomm-2.4/giomm/application.h>
#include <fstream>
#include <streambuf>

#include "common_Socket.h"

Gtk::Window* pWindow = nullptr;
Gtk::Image* pImage = nullptr;
Gtk::Image* pGif = nullptr;

Socket client;

std::string host;
uint16_t port;

static void on_btnImagenYAnimacion_clicked() {
    if (pWindow) {

        std::string aviso = "IMAGEN Y ANIMACION CLICKEADO...";
        std::cout << "ENVÍO AL SERVER: " << aviso << std::endl;

        uint32_t size = aviso.length();
        client.send(reinterpret_cast<char*>(&size), sizeof(uint32_t));
        client.send(aviso.c_str(), aviso.length());

        char aux_response[2048] = { 0 };

        client.receive(reinterpret_cast<char*>(&size), sizeof(uint32_t));
        client.receive(aux_response, size);
        aviso = std::string(aux_response);

        std::cout << "RESPUESTA DEL SERVER: " << aviso << std::endl;
    }
}

static void on_btnImagen_clicked() {
    if (pWindow) {
        if (pImage->is_visible()) {
            pImage->hide();
        } else {
            pImage->show();
        }
    }
}

static void on_btnAnimacion_clicked() {
    if (pWindow) {
        if (pGif->is_visible()) {
            pGif->hide();
        } else {
            pGif->show();
        }
    }
}

static std::string get_file_content(std::string filename){
    std::ifstream t(filename);

    t.seekg(0, std::ios::end);
    size_t size = t.tellg();

    std::string buffer(size, ' ');

    t.seekg(0);
    t.read(&buffer[0], size);

    t.close();

    return buffer;
}

int main(int argc, char **argv) {
    auto app = Gtk::Application::create("org.gtkmm.examples.application", Gio::APPLICATION_HANDLES_OPEN);

    //Load the GtkBuilder file and instantiate its widgets:
    auto refBuilder = Gtk::Builder::create();
    try {
        std::string glade_xml = get_file_content("../resources/glade/game-window.glade");
        refBuilder->add_from_string(glade_xml);
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
    refBuilder->get_widget("mainWindow", pWindow);
    if (pWindow) {
        //Get the GtkBuilder-instantiated Button, and connect a signal handler:
        Gtk::Button* pBtnImagen = nullptr;
        refBuilder->get_widget("btnImagen", pBtnImagen);

        Gtk::Button* pBtnAnimacion = nullptr;
        refBuilder->get_widget("btnAnimacion", pBtnAnimacion);

        Gtk::Button* pBtnImagenYAnimacion = nullptr;
        refBuilder->get_widget("btnImagenYAnimacion", pBtnImagenYAnimacion);

        if (pBtnImagen) {
            refBuilder->get_widget("image", pImage);
            pBtnImagen->signal_clicked().connect(sigc::ptr_fun(on_btnImagen_clicked));
        }

        if (pBtnAnimacion) {
            refBuilder->get_widget("gif", pGif);
            pBtnAnimacion->signal_clicked().connect(sigc::ptr_fun(on_btnAnimacion_clicked));
        }

        if (pBtnImagenYAnimacion) {
            pBtnImagenYAnimacion->signal_clicked().connect(sigc::ptr_fun(on_btnImagenYAnimacion_clicked));
        }

        std::string host = std::string(argv[1]);
        uint16_t port = atoi(argv[2]);

        client.connect(host.c_str(), port);

        app->run(*pWindow);
    }

    delete pWindow;

    return 0;
}