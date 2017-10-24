# Dependencias

Para sus graficos, el proyecto utiliza SDL v2, con el paquete SDL_image.

Para la serializacion de objetos a json, el proyecto utiliza jsoncpp.

En un entorno derivado de Debian, las dependencias pueden adquirirse con:

sudo apt install cmake libsdl2-dev libsdl2-image-dev libjsoncpp-dev

# Compilacion

1)Nos paramos en el directorio principal y ejecutamos el comando:
cmake .

Esto hara que se genere el archivo Makefile

2)Por lo tanto ahora solo resta ejecutar el comando:
make


