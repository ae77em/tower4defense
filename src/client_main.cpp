#include <gtkmm.h>
#include <iostream>

Gtk::Window* pWindow = nullptr;
Gtk::Image* pImage = nullptr;
Gtk::Image* pGif = nullptr;

static void on_btnImagenYAnimacion_clicked()
{
    if(pWindow){
       std::cout << "imagen y animacion" << std::endl;
    }
}

static void on_btnImagen_clicked() {
    if(pWindow){
        if (pImage->is_visible()){
            pImage->hide();
        } else {
            pImage->show();
        }
    }
}

static void on_btnAnimacion_clicked() {
    if(pWindow){
        if (pGif->is_visible()){
            pGif->hide();
        } else {
            pGif->show();
        }
    }
}

int main (int argc, char **argv)
{
  auto app = Gtk::Application::create(argc, argv);

  //Load the GtkBuilder file and instantiate its widgets:
  auto refBuilder = Gtk::Builder::create();
  try
  {
    refBuilder->add_from_file("game-window.glade");
  }
  catch(const Glib::FileError& ex)
  {
    std::cerr << "FileError: " << ex.what() << std::endl;
    return 1;
  }
  catch(const Glib::MarkupError& ex)
  {
    std::cerr << "MarkupError: " << ex.what() << std::endl;
    return 1;
  }
  catch(const Gtk::BuilderError& ex)
  {
    std::cerr << "BuilderError: " << ex.what() << std::endl;
    return 1;
  }

  //Get the GtkBuilder-instantiated Dialog:
  refBuilder->get_widget("mainWindow", pWindow);
  if(pWindow)
  {
    //Get the GtkBuilder-instantiated Button, and connect a signal handler:
    Gtk::Button* pBtnImagen = nullptr;
    refBuilder->get_widget("btnImagen", pBtnImagen);

    Gtk::Button* pBtnAnimacion = nullptr;
    refBuilder->get_widget("btnAnimacion", pBtnAnimacion);

    Gtk::Button* pBtnImagenYAnimacion = nullptr;
    refBuilder->get_widget("btnImagenYAnimacion", pBtnImagenYAnimacion);

    if (pBtnImagen){
        refBuilder->get_widget("image", pImage);
        pBtnImagen->signal_clicked().connect( sigc::ptr_fun(on_btnImagen_clicked) );
    }

    if (pBtnAnimacion){
        refBuilder->get_widget("gif", pGif);
        pGif->get_animation();
        pBtnAnimacion->signal_clicked().connect( sigc::ptr_fun(on_btnAnimacion_clicked) );
    }

    if (pBtnImagenYAnimacion){
        pBtnImagenYAnimacion->signal_clicked().connect( sigc::ptr_fun(on_btnImagenYAnimacion_clicked) );
    }

    app->run(*pWindow);
  }

  delete pWindow;

  return 0;
}