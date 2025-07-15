#include "lexer.h"
#include <cctype>
#include <vector>

using namespace std;

static const vector<string> palabras_reservadas = {"lanza", "sellada", "yeah", "no_yeah", "nola", "giro", "devuelve", "sueltalo", "entralo", "quiebre", "boss", "mandar"};
static const vector<string> tipos_dato = {"_numbra", "_suave", "_letra", "_turbo"};
static const vector<string> tipos_funcion = {"_nada"};
static const vector<string> booleanos = {"real", "fake"};

Lexer::Lexer(ifstream& source) : input(source), linea_actual_num(0), pos_actual(0), dentro_comentario(false) {}

bool Lexer::es_palabra(const string& lexema, const vector<string>& lista) {
    for (const auto& palabra : lista) {
        if (lexema == palabra) return true;
    }
    return false;
}

Token Lexer::crearToken(const string& tipo, const string& lexema, int col) {
    return {tipo, lexema, linea_actual_num, col};
}

Token Lexer::getToken() {
    if (!buffer.empty()) {
        Token t = buffer.front();
        buffer.erase(buffer.begin());
        return t;
    }
    return extraerToken();
}

Token Lexer::peekToken() {
    if (buffer.empty()) {
        buffer.push_back(extraerToken());
    }
    return buffer.front();
}

Token Lexer::extraerToken() {
    while (true) {
        if (pos_actual >= linea_actual.size()) {
            if (getline(input, linea_actual)) {
                linea_actual_num++;
                pos_actual = 0;
            } else {
                return crearToken("EOF", "", pos_actual + 1);
            }
        }

        while (pos_actual < linea_actual.size() && isspace(linea_actual[pos_actual])) {
            pos_actual++;
        }

        if (pos_actual >= linea_actual.size()) continue;

        int col_inicio = pos_actual + 1;
        char actual = linea_actual[pos_actual];

        // Ignorar comentarios entre ?> y <?
        if (!dentro_comentario && linea_actual.substr(pos_actual, 2) == "?>") {
            dentro_comentario = true;
            pos_actual += 2;
            continue;
        }
        if (dentro_comentario) {
            size_t fin = linea_actual.find("<?", pos_actual);
            if (fin != string::npos) {
                pos_actual = fin + 2;
                dentro_comentario = false;
            } else {
                pos_actual = linea_actual.size();
            }
            continue;
        }

        if (linea_actual.substr(pos_actual, 4) == ".qda") {
            pos_actual += 4;
            return crearToken("FIN_LINEA", ".qda", col_inicio);
        }
        if (linea_actual.substr(pos_actual, 2) == "==" || linea_actual.substr(pos_actual, 2) == "!=" ||
            linea_actual.substr(pos_actual, 2) == "&&" || linea_actual.substr(pos_actual, 2) == "||") {
            string op = linea_actual.substr(pos_actual, 2);
            pos_actual += 2;
            string tipo = (op == "&&" || op == "||") ? "LOGICO" : "COMPARADOR";
            return crearToken(tipo, op, col_inicio);
        }

        if (isalpha(actual) || actual == '_') {
            string lexema;
            while (pos_actual < linea_actual.size() && (isalnum(linea_actual[pos_actual]) || linea_actual[pos_actual] == '_')) {
                lexema += linea_actual[pos_actual++];
            }
            if (es_palabra(lexema, palabras_reservadas)) return crearToken(lexema, lexema, col_inicio);
            if (es_palabra(lexema, tipos_dato)) return crearToken("TIPO_DATO", lexema, col_inicio);
            if (es_palabra(lexema, tipos_funcion)) return crearToken("TIPO_FUNCION", lexema, col_inicio);
            if (es_palabra(lexema, booleanos)) return crearToken("BOOLEANO", lexema, col_inicio);
            return crearToken("IDENTIFICADOR", lexema, col_inicio);
        }

        if (isdigit(actual)) {
            string lexema;
            bool tiene_punto = false;
            while (pos_actual < linea_actual.size() && (isdigit(linea_actual[pos_actual]) || (linea_actual[pos_actual] == '.' && !tiene_punto))) {
                if (linea_actual[pos_actual] == '.') {
                    if (pos_actual + 1 < linea_actual.size() && !isdigit(linea_actual[pos_actual+1])) break;
                    tiene_punto = true;
                }
                lexema += linea_actual[pos_actual++];
            }
            return crearToken("NUMERO", lexema, col_inicio);
        }

        if (actual == '"') {
            string lexema;
            pos_actual++; 
            while (pos_actual < linea_actual.size() && linea_actual[pos_actual] != '"') {
                lexema += linea_actual[pos_actual++];
            }
            if (pos_actual < linea_actual.size()) {
                pos_actual++;
                return crearToken("CADENA", lexema, col_inicio);
            } else {
                return crearToken("DESCONOCIDO", lexema, col_inicio);
            }
        }

        string s(1, actual);
        pos_actual++;
        if (s == "=") return crearToken("ASIGNACION", s, col_inicio);
        if (s == "<" || s == ">") return crearToken("COMPARADOR", s, col_inicio);
        if (s == "+" || s == "-" || s == "*" || s == "/") return crearToken("OPERADOR", s, col_inicio);
        if (s == "!") return crearToken("LOGICO", s, col_inicio);
        if (s == "(") return crearToken("PARENTESIS_ABRE", s, col_inicio);
        if (s == ")") return crearToken("PARENTESIS_CIERRA", s, col_inicio);
        if (s == "[") return crearToken("LLAVE_ABRE", s, col_inicio);
        if (s == "]") return crearToken("LLAVE_CIERRA", s, col_inicio);
        if (s == ",") return crearToken("COMA", s, col_inicio);
        
        return crearToken("DESCONOCIDO", s, col_inicio);
    }
}