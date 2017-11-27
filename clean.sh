#!/bin/bash

#########################################################################################
# Script de instalación tower4defense                                                 #
# - Descarga de librerías                                                               #
# - Instalación de librerías                                                            #
# - Compilación de aplicaciones                                                         #
#########################################################################################
echo "Iniciando script de desinstalación de tower4defense..."
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
echo "Desinstalando cmake3"
sudo apt-get remove cmake

#
# jsoncpp
#
echo "Desinstalando jsoncpp"
sudo apt-get remove libjsoncpp-dev

#
# gtkmm
#
echo "Desinstalando gtkmm"
sudo apt-get remove libgtkmm-3.0-dev

#
# SDL2
#
echo "Desinstalando SDL2"
sudo apt-get remove libsdl2-dev

echo "Desinstalando SDL2 Image"
sudo apt-get remove libsdl2-image-dev

echo "Desinstalando SDL2 TTF"
sudo apt-get remove libsdl2-ttf-dev

#
# Compilo tower4defense
#
echo "Desinstalando tower4defense"
rm *.exe
echo "tower4defense fue desinstalado exitosamente...vuelva pronto"