# RetOS v1 Manual

## Booting up

Para iniciar el SO con QEMU en Linux, ejecute:

```sh
chmod +x ./run.sh
./run.sh
```

Se le va a solicitar permisos de sudo.

## Module selector

Para elegir el módulo que desea utilizar, presione el número que
corresponda al programa deseado

1. Shell
    - Principal punto de manejo del SO
2. Current time
    - Imprima en pantalla la hora
3. Dump registers
    - Muestre, si está disponible, un dump de los registros del OS
4. Exceptions
    - Compruebe el funcionamiento del manejo de excepciones del OS
5. Shut down
    - Finaliza la sesión y devuelve el control a Kernel

## Dump registers

El dump de los registros se generan automáticamente al producirse una
excepción manejada o manualmente al presionar la tecla Alt en cualquier
momento. Solo el último dump se preserva, y al momento de solicitarlo
se consume.

## Exceptions

El módulo permite el testeo del manejo de excepciones por parte de
Kernel. Cuenta con 3 opciones:

1. División por 0
2. Código de operación inválido
3. Volver al inicio

## Shell

Permite un manejo mediante texto del sistema operativo. Cuenta con
múltiples comandos, de los cuales los principales son:

1. help [command]
    - Muestra una lista de todos los comandos disponibles, o del
    comando especificado.

2. snake [players]
    - Inicia el juego snake, por default 1, mínimo 1 máximo 3.
    Se utilizan las teclas `wasd`, `ijkl` y `vb` para el tercer jugador.

3. set-theme [theme]
    - Modifica la temática del snake. Actualmente acepta:
        - mario
        - windows
        - camelot
        - creation
        - pong
        - idyllic

4. set-size [n]
    - Cambia el tamaño del texto de la shell.

5. piano
    - Utilice su teclado como un piano

Para más comandos (y sí que los hay), utilice `help` en la shell.
