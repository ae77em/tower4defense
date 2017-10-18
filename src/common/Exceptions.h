#ifndef COMMON_EXCEPTIONS_H
#define COMMON_EXCEPTIONS_H

#include <exception>

struct newsocket_exception : public std::exception {
   const char * what() const throw() {
      return "No se pudo abrir el socket.";
   }
};

struct bind_exception : public std::exception {
   const char * what() const throw() {
      return "Error al realizar el bind.";
   }
};

struct listen_exception : public std::exception {
   const char * what() const throw() {
      return "Error al realizar el listen.";
   }
};

struct connect_exception : public std::exception {
   const char * what() const throw() {
      return "Error al realizar el connect.";
   }
};

struct accept_exception : public std::exception {
   const char * what() const throw() {
      return "Error al realizar el accept.";
   }
};

struct recv_exception : public std::exception {
   const char * what() const throw() {
      return "Error al realizar el send.";
   }
};

struct send_exception : public std::exception {
   const char * what() const throw() {
      return "Error al realizar el send.";
   }
};

struct gethost_exception : public std::exception {
   const char * what() const throw() {
      return "Error al obtener el host.";
   }
};

#endif /* COMMON_EXCEPTIONS_H */

