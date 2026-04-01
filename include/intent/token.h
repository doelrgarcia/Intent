#pragma once

#include "intent/diagnostic.h"

#include <string>
#include <string_view>
#include <vector>

namespace intent {

enum class TokenKind {
    EndOfFile,
    Identifier,
    IntegerLiteral,
    StringLiteral,

    // Keywords
    KwLet,
    KwMut,
    KwFn,
    KwStruct,
    KwEnum,
    KwImpl,
    KwProtocol,
    KwMatch,
    KwIf,
    KwElse,
    KwReturn,
    KwUnsafe,
    KwEffects,

    // Punctuation
    LParen,
    RParen,
    LBrace,
    RBrace,
    LBracket,
    RBracket,
    Comma,
    Colon,
    Semicolon,
    Equal,
    Plus,
    Minus,
    Star,
    Slash,
    Ampersand,
    Pipe,
    Question,

    Arrow,     // ->
    FatArrow,  // =>

    Unknown,
};

struct Token {
    TokenKind kind{TokenKind::Unknown};
    std::string text;
    Span span{};
};

class Lexer {
public:
    explicit Lexer(std::string file_name, std::string_view source, DiagnosticSink& sink);

    std::vector<Token> tokenize();

private:
    char peek() const noexcept;
    char peek_next() const noexcept;
    char advance() noexcept;
    bool match(char expected) noexcept;
    void skip_whitespace_and_comments();

    void lex_identifier_or_keyword();
    void lex_number();
    void lex_string();
    void add_token(TokenKind kind, std::string text, std::size_t start_offset, std::size_t end_offset);

    std::string file_name_;
    std::string_view source_;
    DiagnosticSink& sink_;
    std::size_t pos_{0};
    std::vector<Token> tokens_;
};

}  // namespace intent

