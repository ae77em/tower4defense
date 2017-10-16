#ifndef CLIENT_STARTWINDOW_H
#define CLIENT_STARTWINDOW_H

#include <gtkmm.h>

class StartWindow : Gtk::Window {
public:
    StartWindow();
    StartWindow(const StartWindow& orig);
    virtual ~StartWindow();
};

#endif /* CLIENT_STARTWINDOW_H */

