#!/bin/bash

#########################################################################################
# Script de instalación tower4defense                                                 #
# - Descarga de librerías                                                               #
# - Instalación de librerías                                                            #
# - Compilación de aplicaciones                                                         #
#########################################################################################
echo "Iniciando script de instalación de tower4defense..."
echo ""
echo "       _                           ___     _       __                       "
echo "      | |                         /   |   | |     / _|                      "
echo "      | |_ _____      _____ _ __ / /| | __| | ___| |_ ___ _ __  ___  ___    "
echo "      | __/ _ \ \ /\ / / _ \ '__/ /_| |/ _\` |/ _ \  _/ _ \ '_ \/ __|/ _ \   "
echo "      | || (_) \ V  V /  __/ |  \___  | (_| |  __/ ||  __/ | | \__ \  __/   "
echo "       \__\___/ \_/\_/ \___|_|      |_/\__,_|\___|_| \___|_| |_|___/\___|   "
echo ""

#
# cmake
#
echo "Instalando cmake3"
sudo apt-get install cmake

#
# jsoncpp
#
echo "Instalando jsoncpp"
sudo apt-get install libjsoncpp-dev

#
# gtkmm
#
echo "Instalando gtkmm"
sudo apt-get install libgtkmm-3.0-dev

#
# SDL2
#
echo "Instalando SDL2"
sudo apt-get install libsdl2-dev

echo "Instalando SDL2 Image"
sudo apt-get install libsdl2-image-dev

echo "Instalando SDL2 TTF"
sudo apt-get install libsdl2-ttf-dev

#
# Compilo tower4defense
#
echo "Instalando tower4defense"
echo "Generando Makefile"
cmake .
echo "Compilando tower4defense"
make

echo "El proceso de instalación ha terminado"
echo "Disfrute de tower4defense"