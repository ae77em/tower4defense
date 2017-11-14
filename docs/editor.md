# Editor

## Introduccion

El editor es un programa **modal**. Al abrirlo se entra en modo **TILE**.

## Modos

### TILE
En modo TILE el editor edita los casilleros del mapa

#### Tiles
* 1 torre de agua
* 2 torre de tierra
* 3 torre de fuego
* 4 torre de aire
* e portal de entrada
* q portal de salida
* Esc alternar entre espacio transitable y espacio vacio

#### Transiciones
* PATH: 'r'
* COMMAND ':'

### PATH
En modo PATH el editor agrega nuevos caminos al mapa.  Para agregar un
punto al camino, hacer click en la casilla deseada (se vera el camino
solo cuando halla al menos dos puntos).  Para terminar el camino,
apretar Shift + Click; esto agregara un punto final al camino.
Para cancelar un camino a medio terminar, presionar Esc.

#### Transiciones
* TILE 'r'

### COMMAND
En modo COMMAND el editor acepta comandos por texto.

#### Comandos
* ( q | quit )
* ( o | open ) FILE
* ( s | save ) FILE
* ( n | new ) SIZE
* ( bg | background ) ( desert | grass | ice | lava )

#### Transiciones
* TILE 'Esc', 'Alt' + 'Backspace', o borrando hasta el final de la linea
