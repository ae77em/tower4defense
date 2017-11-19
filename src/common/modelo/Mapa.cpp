#include "Mapa.h"
#include "../Point.h"
#include "../Message.h"
#include <string>
#include <vector>
#include <stdexcept>
#include <jsoncpp/json/json.h>
#include <iostream>
#include <fstream>

using namespace model;

Mapa::Mapa(unsigned x, unsigned y) :extension_x(x), extension_y(y),
        casillas(x * y, '.'), caminos(), estilo_fondo('g'), nombre(""),
        delay_hordas_seg(5) {}

char Mapa::casilla(unsigned x, unsigned y) {
    if (x >= extension_x || y >= extension_y)
        throw std::runtime_error("out of bounds access: " + std::to_string(x)
                + "," + std::to_string(y) + " on "
                + std::to_string(extension_x) + " x "
                + std::to_string(extension_y) + " map");
    return casillas[x + y * extension_x];
}

Point Mapa::dimensiones() {
    return Point(extension_x, extension_y);
}

void Mapa::setCasilla(char value, unsigned x, unsigned y) {
    if (x >= extension_x || y >= extension_y)
        throw std::runtime_error("out of bounds access: " + std::to_string(x)
                + "," + std::to_string(y) + " on "
                + std::to_string(extension_x) + " x "
                + std::to_string(extension_y) + " map");
    casillas[x + y * extension_x] = value;
}

std::vector<std::vector<Point>>& Mapa::getCaminos() {
    return caminos;
}

void Mapa::agregarCamino(const std::vector<Point> &camino) {
    caminos.push_back(camino);
}

std::string Mapa::serialize() {
    Json::Value root;

    root["width"] = extension_x;
    root["height"] = extension_y;

    // Serializar las casillas
    root["fondo"] = std::string(&estilo_fondo, 1);
    std::string string_casillas(casillas.data(), extension_x * extension_y);
    root["tiles"] = string_casillas;

    // Serializar los caminos
    for (const auto& camino : caminos) {
        Json::Value path;
        for (const auto& point : camino) path.append(point.serialize());
        root["paths"].append(path);
    }

    // Serializar las hordas
    root["horde_delay"] = delay_hordas_seg;
    for (const auto& pair : hordas) {
        Json::Value horde;
        horde["path_index"] = pair.first;
        for (const auto& enemy_name : pair.second)
            horde["enemies"].append(enemy_name);
        root["hordes"].append(horde);
    }

    // Generar string a partir de Json::Value
    Json::StreamWriterBuilder builder;
    builder["commentStyle"] = "None";
    builder["indentation"] = "    ";
    return Json::writeString(builder, root);
}

const std::vector<std::pair<int, std::vector<std::string>>>& Mapa::getHordas() {
    return hordas;
}

void Mapa::agregarHorda(int camino, std::vector<std::string> enemigos) {
    if ( (int)caminos.size() <= camino )
        throw std::runtime_error("tratando de agregar horda a camino "
                "inexistente " + std::to_string(camino));
    if (camino < 0)
        throw std::runtime_error("tratando de agregar horda con "
                "indice negativo" + std::to_string(camino));
    hordas.emplace_back(camino, enemigos);
}

int Mapa::getDelay() const {
    return delay_hordas_seg;
}

void Mapa::setDelay(int delay) {
    if (delay < 0) throw std::runtime_error("delay negativo "
            + std::to_string(delay));
    delay_hordas_seg = delay;
}

char Mapa::getEstiloFondo() {
    return estilo_fondo;
}

void Mapa::setEstiloFondo(char estilo) {
    estilo_fondo = estilo;
}

bool Mapa::estaDentro(Point &p) const {
    return p.isPositive()
        && (p.x < (int)extension_x)
        && (p.y < (int)extension_y);
}

std::string &Mapa::getNombre() {
    return nombre;
}

void Mapa::setNombre(std::string s) {
    nombre = s;
}

Mapa Mapa::cargarDesdeString(std::string json) {
    Message m;
    m.deserialize(json);
    Json::Value root = static_cast<Json::Value &&>(m.getData());

    Mapa map(root["width"].asUInt(), root["height"].asUInt());

    // Deserializar las casillas
    map.estilo_fondo = root["fondo"].asString().c_str()[0];
    std::string str_casillas = root["tiles"].asString();
    map.casillas = std::vector<char>(str_casillas.begin(), str_casillas.end());

    // Deserializar los caminos
    for (const auto& path : root["paths"]) {
        map.caminos.emplace_back();
        auto &camino = map.caminos.back();
        for (const auto& point : path)
            camino.push_back(Point::deserialize(point));
    }

    // Deserializar las hordas
    map.delay_hordas_seg = root["horde_delay"].asInt();
    for (const auto& pair : root["hordes"]) {
        std::vector<std::string> enemigos;
        for (const auto& name : pair["enemies"])
            enemigos.push_back(name.asString());
        map.hordas.emplace_back(pair["path_index"].asInt(), enemigos);
    }

    return map;
}

Mapa Mapa::cargarDesdeArchivo(std::string filename){
    std::fstream map_file;
    map_file.open(filename, std::ios::in | std::ios::binary);
    if (!map_file) throw std::runtime_error("Could not open file " + filename);

    // Load file contents into string
    std::string contents;
    map_file.seekg(0, std::ios::end);
    contents.resize(map_file.tellg());
    map_file.seekg(0, std::ios::beg);
    map_file.read(&contents[0], contents.size());

    map_file.close();
    return cargarDesdeString(contents);
}
