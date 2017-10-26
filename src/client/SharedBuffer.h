#ifndef INTERMEDIATE_BUFFER_H
#define INTERMEDIATE_BUFFER_H

#include <string>
#include <queue>
#include <mutex>
#include <condition_variable>

class SharedBuffer {
private:
    std::mutex m;
    std::condition_variable cv;
    std::queue<std::string> buffer;
    bool isClientProcessesEnded = false;
    
public:
    SharedBuffer();
    SharedBuffer(const SharedBuffer& orig);
    virtual ~SharedBuffer();

    /*
     * Retorna el siguiente registro alojado en el buffer.
     */
    std::string getNextData();

    /*
     * Agrega un registro al buffer.
     */
    void addData(const std::string &s);

    /*
     * Retorna true si el buffer está cargado o hay algún cliente
     * consumiendolo.
     */
    bool isProcessingYet() const;

    /*
     * Retorna true si el buffer tiene data.
     */
    bool hasData() const;

    /*
     * Retorna true si no hay más datos en el buffer y no hay nadie
     * utilizándolo.
     */
    bool isEnded() const;

    /*
     * Cambia el estado de procesamiento del cliente.
     */
    void setClientProcessEnded(bool ended);
};

#endif /* INTERMEDIATE_BUFFER_H */

