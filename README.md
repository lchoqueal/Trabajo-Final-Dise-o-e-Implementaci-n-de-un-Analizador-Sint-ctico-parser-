# Analizador Léxico y Sintáctico en C++

Este repositorio contiene el código fuente completo para un analizador léxico (lexer) y sintáctico (parser) implementado manualmente en C++, siguiendo la técnica de descenso recursivo. El programa analiza archivos de código fuente en el lenguaje definido para el proyecto, identificando tokens y verificando la sintaxis.

---

## Requisitos y Dependencias

- **Compilador C++:** Se recomienda GCC (g++) o MinGW en Windows.
- **Sistema Operativo:** Código compatible con Windows, Linux y MacOS.
- **No se utilizan dependencias externas o librerías adicionales.**
- **Terminal:** Utilizar powershell para una correcta muestra de mensaje de salida

---

## Compilación

Para compilar el analizador, abre una terminal en la carpeta del repositorio y ejecuta el siguiente comando:

```cmd
g++ main.cpp lexer.cpp parser.cpp -o CompiladorUrban -std=c++11
```

Esto generará un ejecutable llamado `CompiladorUrban`.

---

## Ejecución

Para analizar un archivo fuente:

```bash
.\CompiladorUrban.exe "Nombre del archivo".urb
```

Esto generará el archivo `tokens.txt` con la secuencia de tokens del archivo fuente. // Seguir con las instrucciones puestas en main.cpp para poder generar el archivo .txt  
El análisis sintáctico se ejecuta y mostrará en la consola si el programa es válido o reportará errores de sintaxis.

---

## Ejemplo de Salida

- **tokens.txt** contendrá líneas como:
  ```
  TIPO_FUNCION   _nada     [línea 1, col 1]
  IDENTIFICADOR  boss      [línea 1, col 7]
  ...
  ```
- En la consola, para un programa válido:
  ```
  Análisis sintáctico completado sin errores.
  ```
- Para errores sintácticos, se mostrará:
  ```
  [Error Sintáctico en línea 2, columna 10]: Se esperaba IDENTIFICADOR, se encontró ASIGNACION
  ```
