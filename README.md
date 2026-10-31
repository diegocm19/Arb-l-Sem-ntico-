# Semántico — Motor de Inferencia en Español

Aplicación de escritorio para convertir reglas escritas en español a fórmulas
proposicionales y construir su tableau semántico. El resultado indica si la base
de conocimiento es consistente y muestra las ramas abiertas (modelos).

## Requisitos

- Windows 10/11
- MSYS2 UCRT64
- Qt 6 para MinGW UCRT64

Instala los paquetes desde la terminal **MSYS2 UCRT64**:

```sh
pacman -S --needed mingw-w64-ucrt-x86_64-gcc \
  mingw-w64-ucrt-x86_64-qt6-base \
  mingw-w64-ucrt-x86_64-qt6-tools \
  mingw-w64-ucrt-x86_64-make \
  mingw-w64-ucrt-x86_64-cmake
```

## Compilar y ejecutar

### Desde PowerShell

En PowerShell, usa `$env:Path` (no `set PATH=...;%PATH%`, que es sintaxis de
`cmd.exe`). Ejecuta estos comandos; cambia la ruta si guardaste el proyecto en
otra carpeta:

```powershell
$env:Path = "C:\msys64\ucrt64\bin;$env:Path"
Set-Location -LiteralPath "C:\Misión del lenguaje natural al árbol semántico"
mingw32-make
```

### Desde `cmd.exe`

También puedes abrir `cmd.exe` y ejecutar:

```bat
set PATH=C:\msys64\ucrt64\bin;%PATH%
cd /d "C:\Misión del lenguaje natural al árbol semántico"
mingw32-make
```

El comando `mingw32-make` configura con CMake, compila y abre la aplicación.
El makefile asigna y libera una unidad temporal durante la compilación para que
Qt y MinGW manejen correctamente la ruta con acentos. No escribas `>>` si
aparece al inicio de una línea: es el indicador de continuación de PowerShell,
no parte del comando. Para compilar sin ejecutar:

```bat
mingw32-make build
```

El ejecutable queda en `build\bin\Semantico.exe`. Para limpiar los archivos
generados, usa `mingw32-make clean`.

## Sintaxis admitida

- `no` para negación
- `y` para conjunción
- `o` para disyunción
- `si ... entonces ...` para implicación
- `... si y solo si ...` para bicondicional
- Paréntesis para agrupar expresiones

Ejemplo:

```text
si llueve entonces suelo mojado
llueve
no suelo mojado
```
