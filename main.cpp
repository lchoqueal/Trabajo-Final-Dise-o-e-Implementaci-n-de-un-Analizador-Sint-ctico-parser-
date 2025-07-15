#ifdef _WIN32
#include <windows.h>
#endif

#include <iostream>
#include <fstream>
#include <locale>
#include <stdexcept>
#include "lexer.h"
#include "parser.h"

void imprimirTokensEnArchivo(const std::string& archivoFuente, const std::string& archivoSalida) {
    std::ifstream archivo(archivoFuente);
    if (!archivo.is_open()) {
        std::cerr << "No se pudo abrir el archivo para imprimir tokens." << std::endl;
        return;
    }
    std::ofstream salida(archivoSalida);
    if (!salida.is_open()) {
        std::cerr << "No se pudo crear el archivo de salida." << std::endl;
        return;
    }
    Lexer lexer(archivo);
    Token tok;
    do {
        tok = lexer.getToken();
        salida << tok.tipo << "\t" << tok.lexema
               << "\t[línea " << tok.linea << ", col " << tok.col << "]" << std::endl;
    } while (tok.tipo != "EOF");
    salida.close();
}

int main(int argc, char* argv[]) {
    #ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    #endif
    std::setlocale(LC_ALL, "");
    if (argc != 2) {
        std::cerr << "Uso: " << argv[0] << " <archivo.urb>" << std::endl;
        return 1;
    }

    //Llamada a la función para imprimir tokens en un archivo
    //Descomentar la siguiente línea para poder imprimir los tokens en un archivo
    //imprimirTokensEnArchivo(argv[1], "tokens.txt");

    //Comentar las siguientes lineas si no se desea analizar el archivo y solo se quiere imprimir los tokens

    std::ifstream archivo(argv[1]);
    if (!archivo.is_open()) {
        std::cerr << "Error: No se pudo abrir el archivo '" << argv[1] << "'" << std::endl;
        return 1;
    }
    try {
        Lexer lexer(archivo);
        Parser parser(lexer);
        parser.parse();
    } catch (const std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}