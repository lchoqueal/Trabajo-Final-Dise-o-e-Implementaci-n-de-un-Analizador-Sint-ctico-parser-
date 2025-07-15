#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>
#include <fstream>

struct Token {
    std::string tipo;
    std::string lexema;
    int linea;
    int col;
};

class Lexer {
public:
    Lexer(std::ifstream& source);
    Token getToken();
    Token peekToken();

private:
    std::ifstream& input;
    std::string linea_actual;
    int linea_actual_num;
    int pos_actual;
    std::vector<Token> buffer;
    bool dentro_comentario;

    Token extraerToken();
    bool es_palabra(const std::string& lexema, const std::vector<std::string>& lista);
    Token crearToken(const std::string& tipo, const std::string& lexema, int col);
};

#endif