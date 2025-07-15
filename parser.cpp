#include "parser.h"
#include <iostream>
#include <stdexcept>

using namespace std;

Parser::Parser(Lexer& lexer) : lexer(lexer), token_actual(lexer.getToken()) {}

void Parser::error(const string& mensaje) {
    throw runtime_error("[Error Sintáctico en línea " + to_string(token_actual.linea) + ", columna " + to_string(token_actual.col) + "]: " + mensaje + ". Se encontró '" + token_actual.lexema + "' (" + token_actual.tipo + ").");
}

void Parser::consumir(const string& tipo_esperado) {
    if (token_actual.tipo == tipo_esperado) {
        token_actual = lexer.getToken();
    } else {
        error("Se esperaba un token de tipo '" + tipo_esperado + "'");
    }
}

Token Parser::verSiguienteToken() {
    return lexer.peekToken();
}

void Parser::parse() {
    parsePrograma();
    cout << "Análisis sintáctico completado sin errores." << endl;
}

void Parser::parsePrograma() {
    parseFuncionPrincipal();
    while (token_actual.tipo != "EOF") {
        parseFuncionUsuario();
    }
}

void Parser::parseFuncionPrincipal() {
    consumir("TIPO_FUNCION");
    consumir("boss");
    consumir("PARENTESIS_ABRE");
    consumir("PARENTESIS_CIERRA");
    consumir("LLAVE_ABRE");
    while (token_actual.tipo != "LLAVE_CIERRA" && token_actual.tipo != "EOF") {
        parseInstruccion();
    }
    consumir("LLAVE_CIERRA");
}

void Parser::parseFuncionUsuario() {
    consumir("TIPO_DATO");
    consumir("IDENTIFICADOR");
    consumir("PARENTESIS_ABRE");
    if (token_actual.tipo == "TIPO_DATO") {
        parseParametros();
    }
    consumir("PARENTESIS_CIERRA");
    consumir("LLAVE_ABRE");
    while (token_actual.tipo != "LLAVE_CIERRA" && token_actual.tipo != "EOF") {
        parseInstruccion();
    }
    consumir("LLAVE_CIERRA");
}

void Parser::parseParametros() {
    consumir("TIPO_DATO");
    consumir("IDENTIFICADOR");
    while (token_actual.tipo == "COMA") {
        consumir("COMA");
        consumir("TIPO_DATO");
        consumir("IDENTIFICADOR");
    }
}

void Parser::parseInstruccion() {
    if (token_actual.tipo == "lanza" || token_actual.tipo == "sellada") {
        parseDeclaracionVar();
    } else if (token_actual.tipo == "IDENTIFICADOR" && verSiguienteToken().tipo == "ASIGNACION") {
        parseAsignacion();
    } else if (token_actual.tipo == "yeah") {
        parseCondicional();
    } else if (token_actual.tipo == "giro") {
        parseBucle();
    } else if (token_actual.tipo == "devuelve") {
        consumir("devuelve");
        consumir("PARENTESIS_ABRE");
        parseExpresion();
        consumir("PARENTESIS_CIERRA");
        consumir("FIN_LINEA");
    } else if (token_actual.tipo == "sueltalo" || token_actual.tipo == "entralo") {
        consumir(token_actual.tipo);
        consumir("PARENTESIS_ABRE");
        if (token_actual.tipo != "PARENTESIS_CIERRA") parseExpresion();
        consumir("PARENTESIS_CIERRA");
        consumir("FIN_LINEA");
    } else if (token_actual.tipo == "IDENTIFICADOR") {
        parseLlamadaFuncion();
        consumir("FIN_LINEA");
    } else if (token_actual.tipo == "quiebre") {
        consumir("quiebre");
        consumir("PARENTESIS_ABRE");
        consumir("PARENTESIS_CIERRA");
        consumir("FIN_LINEA");
    } else if (token_actual.tipo != "LLAVE_CIERRA" && token_actual.tipo != "EOF") {
        error("Se esperaba una instrucción válida");
    }
}

void Parser::parseDeclaracionVar() {
    if (token_actual.tipo == "lanza" || token_actual.tipo == "sellada") {
        consumir(token_actual.tipo);
    } else {
        error("Se esperaba 'lanza' o 'sellada'");
    }
    consumir("TIPO_DATO");
    consumir("IDENTIFICADOR");
    if (token_actual.tipo == "ASIGNACION") {
        consumir("ASIGNACION");
        parseExpresion();
    }
    consumir("FIN_LINEA");
}

void Parser::parseAsignacion() {
    consumir("IDENTIFICADOR");
    consumir("ASIGNACION");
    parseExpresion();
    consumir("FIN_LINEA");
}

void Parser::parseCondicional() {
    consumir("yeah");
    consumir("PARENTESIS_ABRE");
    parseExpresion();
    consumir("PARENTESIS_CIERRA");
    consumir("LLAVE_ABRE");
    while (token_actual.tipo != "LLAVE_CIERRA") parseInstruccion();
    consumir("LLAVE_CIERRA");

    if (token_actual.tipo == "no_yeah") {
        consumir("no_yeah");
        consumir("PARENTESIS_ABRE");
        parseExpresion();
        consumir("PARENTESIS_CIERRA");
        consumir("LLAVE_ABRE");
        while (token_actual.tipo != "LLAVE_CIERRA") parseInstruccion();
        consumir("LLAVE_CIERRA");
    }

    if (token_actual.tipo == "nola") {
        consumir("nola");
        consumir("LLAVE_ABRE");
        while (token_actual.tipo != "LLAVE_CIERRA") parseInstruccion();
        consumir("LLAVE_CIERRA");
    }
}

void Parser::parseBucle() {
    consumir("giro");
    consumir("PARENTESIS_ABRE");
    parseExpresion();
    consumir("PARENTESIS_CIERRA");
    consumir("LLAVE_ABRE");
    while (token_actual.tipo != "LLAVE_CIERRA") parseInstruccion();
    consumir("LLAVE_CIERRA");
}

void Parser::parseLlamadaFuncion() {
    consumir("IDENTIFICADOR");
    consumir("PARENTESIS_ABRE");
    if (token_actual.tipo != "PARENTESIS_CIERRA") {
        parseExpresion();
        while (token_actual.tipo == "COMA") {
            consumir("COMA");
            parseExpresion();
        }
    }
    consumir("PARENTESIS_CIERRA");
}

void Parser::parseExpresion() {
    parseExpresionLogica();
}

void Parser::parseExpresionLogica() {
    parseExpresionRelacional();
    while (token_actual.tipo == "LOGICO" && (token_actual.lexema == "&&" || token_actual.lexema == "||")) {
        consumir("LOGICO");
        parseExpresionRelacional();
    }
}

void Parser::parseExpresionRelacional() {
    parseExpresionAritmetica();
    if (token_actual.tipo == "COMPARADOR") {
        consumir("COMPARADOR");
        parseExpresionAritmetica();
    }
}

void Parser::parseExpresionAritmetica() {
    parseTermino();
    while (token_actual.tipo == "OPERADOR" && (token_actual.lexema == "+" || token_actual.lexema == "-")) {
        consumir("OPERADOR");
        parseTermino();
    }
}

void Parser::parseTermino() {
    parseFactor();
    while (token_actual.tipo == "OPERADOR" && (token_actual.lexema == "*" || token_actual.lexema == "/")) {
        consumir("OPERADOR");
        parseFactor();
    }
}

void Parser::parseFactor() {
    if (token_actual.tipo == "NUMERO") {
        consumir("NUMERO");
    } else if (token_actual.tipo == "CADENA") {
        consumir("CADENA");
    } else if (token_actual.tipo == "BOOLEANO") {
        consumir("BOOLEANO");
    } else if (token_actual.tipo == "IDENTIFICADOR") {
        if (verSiguienteToken().tipo == "PARENTESIS_ABRE") {
            parseLlamadaFuncion();
        } else {
            consumir("IDENTIFICADOR");
        }
    } else if (token_actual.tipo == "PARENTESIS_ABRE") {
        consumir("PARENTESIS_ABRE");
        parseExpresion();
        consumir("PARENTESIS_CIERRA");
    } else if (token_actual.tipo == "LOGICO" && token_actual.lexema == "!") {
        consumir("LOGICO");
        parseFactor();
    } else {
        error("Se esperaba un número, cadena, booleano, identificador o una expresión entre paréntesis");
    }
}