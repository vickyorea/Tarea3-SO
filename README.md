# Simulador de Planificación de CPU

## Información general

| Campo | Información |
| --- | --- |
| Asignatura | Sistemas Operativos |
| Número de tarea | 3 |
| Semestre | 2027-1 |

## Integrantes

| Nombre completo | Número de cuenta | Correo electrónico |
| --- | --- | --- |
| Laura Victoria Orea Romero | 320293294 | <lauravorea@ciencias.unam.mx> |
| Joaquín Medina Peralta | 320202513 | <joaquin.medina513@ciencias.unam.mx> |

## Algoritmos implementados

- **Round Robin**

- **Shortest Job First**

## Estructura del proyecto

```text
Tarea3-SO/
├── README.md
├── reporte.pdf
├── Makefile
├── src/
│   ├── sjf_simulador.c
│   └── roundRobin_simulador.c
├── resultados/
│   ├── sjf/
│   └── roundRobin/
└── datos/
    ├── prueba1.txt
    └── prueba2.txt
```

## Requisitos

### Linux / WSL

- GCC con soporte para C11.
- GNU Make.

En distribuciones basadas en Debian/Ubuntu, normalmente basta con contar con `build-essential`.

### Windows

La forma recomendada de ejecutar el proyecto es mediante uno de estos entornos:

- **WSL**, usando GCC, Make y Bash como en Linux; o
- **MSYS2/MinGW64**, con GCC, Make, Bash y `diff` disponibles.

## Compilación

Para compilar el proyecto:

```bash
make
```

Después de la compilación se genera el ejecutable ```roundRobin_simulador``` y ```sjf_simulador```
los cuales estarán en la carpeta ```/build``` con
los respectivos nombres.

Para eliminar los archivos generados durante la compilación:

```bash
make clean
```

## Ejecución

Asegúrate de haber compilado previamente los archivos fuente ejecutando el comando make en la raíz de tu proyecto. Ambos simuladores generarán sus respectivos reportes dentro de la carpeta resultados/.

### Para ejecutar Round Robin

El simulador de Round Robin se puede ejecutar pasando el archivo de texto como parámetro o leyendo directamente desde la entrada estándar.

- Mediante archivo por parámetro:

```Bash
    ./roundRobin_simulador Pruebachiquita.txt
```

- Mediante redirección (entrada estándar):

```Bash
    cat Pruebachiquita.txt | ./roundRobin_simulador
```

### Para ejecutar Shortest Job First

El simulador SJF comparte exactamente la misma sintaxis de ejecución para procesar los datos de entrada.

- Mediante archivo por parámetro:

```Bash
    ./sjf_simulador Pruebachiquita.txt
```

- Mediante redirección (entrada estándar):

```Bash
    cat Pruebachiquita.txt | ./sjf_simulador
```

## Notas de ejecución

- Verificado con `gcc -std=c11 -Wall -Wextra -Wpedantic` sin advertencias.
- Verificado en Linux con 13/13 pruebas exitosas.
- `make clean` elimina `build/` y el ejecutable generado.
- No se entregan binarios, archivos objeto ni contenido de `build/`.

## Métricas:
El simulador calcula las siguientes  métricas:

- Tiempo de retorno = Tiempo de finalización - Tiempo de llegada
- Tiempo de espera = Tiempo de retorno - Ráfaga de CPU
- Tiempo de respuesta = Tiempo de inicio - Tiempo de llegada
