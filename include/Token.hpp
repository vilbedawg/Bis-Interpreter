#ifndef TOKEN_HPP
#define TOKEN_HPP
#include <string>

enum class TokenType
{
    // Single-character tokens
    LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
    COMMA, DOT, MINUS, PLUS, SLASH, SEMICOLON, STAR,

    // One or two character tokens
    RIGHT_BRACKET, LEFT_BRACKET, EXCLAMATION, EXCLAMATION_EQUAL, EQUAL,
    EQUAL_EQUAL, GREATER, GREATER_EQUAL, LESS, LESS_EQUAL, MINUS_MINUS, PLUS_PLUS,

    // Literal
    IDENTIFIER, STRING, NUMBER,

    // Keyword
    AND, OR, CLASS, IF, ELSE, ELIF, _FALSE, _TRUE, FN, FOR, 
    WHILE, NIL, PRINT, RETURN, SUPER, THIS, VAR, LAMBDA, BREAK, CONTINUE,

    _EOF
};

struct Token
{
    Token(TokenType type, std::string lexeme, unsigned int line);
    Token(Token&& token) noexcept;
    Token(const Token& token);

    const TokenType type;
    std::string lexeme;
    const unsigned int line;
};

std::ostream& operator<<(std::ostream& os, const Token& token);
std::ostream& operator<<(std::ostream& os, const TokenType type);

#endif // TOKEN_HPP
