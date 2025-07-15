#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include <string>
#include <vector>

class Parser {
public:
    Parser(Lexer& lexer);
    void parse();

private:
    Lexer& lexer;
    Token token_actual;

    void consumir(const std::string& tipo_esperado);
    void error(const std::string& mensaje);
    Token verSiguienteToken();

    void parsePrograma();
    void parseFuncionPrincipal();
    void parseFuncionUsuario();
    void parseParametros();
    void parseInstruccion();
    void parseDeclaracionVar();
    void parseAsignacion();
    void parseCondicional();
    void parseBucle();
    void parseLlamadaFuncion();
    void parseExpresion();
    void parseExpresionLogica();
    void parseExpresionRelacional();
    void parseExpresionAritmetica();
    void parseTermino();
    void parseFactor();
};

#endif