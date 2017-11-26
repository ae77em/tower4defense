#include "Map.h"
#include "../Point.h"
#include "../Message.h"
#include <string>
#include <vector>
#include <stdexcept>
#include <jsoncpp/json/json.h>
#include <iostream>
#include <fstream>

using namespace model;

Map::Map(unsigned x, unsigned y) :extension_x(x), extension_y(y),
        tiles(x * y, '.'), paths(), background_style('g'), name("") {}

char Map::tile(unsigned x, unsigned y) {
    if (x >= extension_x || y >= extension_y)
        throw std::runtime_error("out of bounds access: " + std::to_string(x)
                + "," + std::to_string(y) + " on "
                + std::to_string(extension_x) + " x "
                + std::to_string(extension_y) + " map");
    return tiles[x + y * extension_x];
}

Point Map::dimensions() {
    return Point(extension_x, extension_y);
}

void Map::setTile(char value, unsigned x, unsigned y) {
    if (x >= extension_x || y >= extension_y)
        throw std::runtime_error("out of bounds access: " + std::to_string(x)
                + "," + std::to_string(y) + " on "
                + std::to_string(extension_x) + " x "
                + std::to_string(extension_y) + " map");
    tiles[x + y * extension_x] = value;
}

std::vector<std::vector<Point>>& Map::getPaths() {
    return paths;
}

void Map::addPaths(const std::vector<Point> &camino) {
    paths.push_back(camino);
}

std::string Map::serialize() {
    Json::Value root;

    root["width"] = extension_x;
    root["height"] = extension_y;

    // Serializar las casillas
    root["fondo"] = std::string(&background_style, 1);
    std::string string_casillas(tiles.data(), extension_x * extension_y);
    root["tiles"] = string_casillas;

    // Serializar los caminos
    for (const auto& camino : paths) {
        Json::Value path;
        for (const auto& point : camino) path.append(point.serialize());
        root["paths"].append(path);
    }

    // Serializar las hordas
    for (const auto& entry : hordas) {
        Json::Value horde;
        horde["enemy_type"] = std::get<0>(entry);
        horde["horde_size"] = std::get<1>(entry);
        horde["path_index"] = std::get<2>(entry);
        horde["delay"] = std::get<3>(entry);
        root["hordes"].append(horde);
    }

    // Generar string a partir de Json::Value
    Json::StreamWriterBuilder builder;
    builder["commentStyle"] = "None";
    builder["indentation"] = "    ";
    return Json::writeString(builder, root);
}

const std::vector<entry>& Map::getHordes() const {
    return hordas;
}

void Map::addHorde(const std::string& enemy_type, int horde_size,
        int path_index, int delay_seconds) {
    if (horde_size < 1)
        throw std::runtime_error("tratando de agregar horda con "
                "menos de un enemigo");
    if ((int)paths.size() <= path_index)
        throw std::runtime_error("tratando de agregar horda a camino "
                "inexistente " + std::to_string(path_index));
    if (path_index < 0)
        throw std::runtime_error("tratando de agregar horda con "
                "indice negativo" + std::to_string(path_index));
    if (delay_seconds < 0)
        throw std::runtime_error("tratando de agregar horda con "
                "delay negativo");
    hordas.emplace_back(enemy_type, horde_size,
            path_index, delay_seconds);
}

char Map::getBackgroundStyle() {
    return background_style;
}

void Map::setBackgroundStyle(char estilo) {
    background_style = estilo;
}

bool Map::isIn(Point &p) const {
    return p.isPositive()
        && (p.x < (int)extension_x)
        && (p.y < (int)extension_y);
}

std::string &Map::getName() {
    return name;
}

void Map::setName(std::string s) {
    name = s;
}

Map Map::loadFromString(std::string json) {
    Message m;
    m.deserialize(json);
    Json::Value root = static_cast<Json::Value &&>(m.getData());

    Map map(root["width"].asUInt(), root["height"].asUInt());

    // Deserializar las casillas
    map.background_style = root["fondo"].asString().c_str()[0];
    std::string str_casillas = root["tiles"].asString();
    map.tiles = std::vector<char>(str_casillas.begin(), str_casillas.end());

    // Deserializar los caminos
    for (const auto& path : root["paths"]) {
        map.paths.emplace_back();
        auto &camino = map.paths.back();
        for (const auto& point : path)
            camino.push_back(Point::deserialize(point));
    }

    // Deserializar las hordas
    for (const auto& entry : root["hordes"]) {
        map.hordas.emplace_back(
                entry["enemy_type"].asString(),
                entry["horde_size"].asInt(),
                entry["path_index"].asInt(),
                entry["delay"].asInt()
        );
    }

    return map;
}

Map Map::loadFromFile(std::string filename){
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
    return loadFromString(contents);
}

Map::Map() { }

void Map::checkValid() {
    //TODO: al menos un portal de entrada

    //TODO: un unico camino sale de cada portal de entrada

    // Cada camino comienza en un portal de entrada
    // Cada camino termina en un portal de salida
    for (const auto& path : paths) {
        auto start = path.front();
        if (tile(start.x, start.y) != 'E')
            throw std::runtime_error("path does not begin with portal");

        auto end = path.back();
        if (tile(end.x, end.y) != 'S')
            throw std::runtime_error("path does not end with portal");
    }

    //TODO: cada portal (de entrada o salida) es comienzo/final de un camino
}
