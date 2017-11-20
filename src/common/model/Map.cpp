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
        tiles(x * y, '.'), paths(), background_style('g'), name(""),
        delay_hordes_seg(5) {}

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
    root["horde_delay"] = delay_hordes_seg;
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

const std::vector<std::pair<int, std::vector<std::string>>>& Map::getHordes() {
    return hordas;
}

void Map::addHorde(int camino, std::vector<std::string> enemigos) {
    if ( (int)paths.size() <= camino )
        throw std::runtime_error("tratando de agregar horda a camino "
                "inexistente " + std::to_string(camino));
    if (camino < 0)
        throw std::runtime_error("tratando de agregar horda con "
                "indice negativo" + std::to_string(camino));
    hordas.emplace_back(camino, enemigos);
}

int Map::getDelay() const {
    return delay_hordes_seg;
}

void Map::setDelay(int delay) {
    if (delay < 0) throw std::runtime_error("delay negativo "
            + std::to_string(delay));
    delay_hordes_seg = delay;
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
    map.delay_hordes_seg = root["horde_delay"].asInt();
    for (const auto& pair : root["hordes"]) {
        std::vector<std::string> enemigos;
        for (const auto& name : pair["enemies"])
            enemigos.push_back(name.asString());
        map.hordas.emplace_back(pair["path_index"].asInt(), enemigos);
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
