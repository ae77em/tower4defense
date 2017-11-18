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
        casillas(x * y, '.'), caminos(), estilo_fondo('g') {}

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
    enemigos.emplace_back();
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

    // Generar string a partir de Json::Value
    Json::StreamWriterBuilder builder;
    builder["commentStyle"] = "None";
    builder["indentation"] = "    ";
    return Json::writeString(builder, root);
}

Mapa::Mapa(const std::string &filename) {
    Json::Value root;
    Json::Reader reader;
    reader.parse(filename, root);

    extension_x = root["width"].asUInt();
    extension_y = root["height"].asUInt();

    // Deserializar las casillas
    estilo_fondo = root["fondo"].asString().c_str()[0];
    std::string str_casillas = root["tiles"].asString();
    casillas = std::vector<char>(str_casillas.begin(), str_casillas.end());

    // Deserializar los caminos
    for (const auto& path : root["paths"]) {
        caminos.emplace_back();
        auto &camino = caminos.back();
        for (const auto& point : path)
            camino.push_back(Point::deserialize(point));
    }
}

std::vector<std::vector<model::Enemy>>& Mapa::getEnemigos() {
    return enemigos;
}

void Mapa::agregarEnemigo(int indice_camino, const Enemy &e) {
    enemigos.at(indice_camino).push_back(e);
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

unsigned Mapa::getExtensionX(){
    return extension_x;
}

unsigned Mapa::getExtensionY(){
    return extension_y;
}

Mapa::Mapa() { }

void Mapa::cargarDesdeString(std::string json) {
    Message m;
    m.deserialize(json);
    Json::Value root = static_cast<Json::Value &&>(m.getData());

    extension_x = root["width"].asUInt();
    extension_y = root["height"].asUInt();

    // Deserializar las casillas
    estilo_fondo = root["fondo"].asString().c_str()[0];
    std::string str_casillas = root["tiles"].asString();
    casillas = std::vector<char>(str_casillas.begin(), str_casillas.end());

    // Deserializar los caminos
    for (const auto& path : root["paths"]) {
        caminos.emplace_back();
        auto &camino = caminos.back();
        for (const auto& point : path)
            camino.push_back(Point::deserialize(point));
    }
}

void Mapa::cargarDesdeArchivo(std::string filename){
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
    cargarDesdeString(contents);
}
