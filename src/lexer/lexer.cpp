#include "intent/token.h"

#include <cctype>

namespace intent {

Lexer::Lexer(std::string file_name, std::string_view source, DiagnosticSink& sink)
    : file_name_(std::move(file_name)), source_(source), sink_(sink) {}

char Lexer::peek() const noexcept {
    if (pos_ >= source_.size()) {
        return '\0';
    }
    return source_[pos_];
}

char Lexer::peek_next() const noexcept {
    if (pos_ + 1 >= source_.size()) {
        return '\0';
    }
    return source_[pos_ + 1];
}

char Lexer::advance() noexcept {
    if (pos_ >= source_.size()) {
        return '\0';
    }
    return source_[pos_++];
}

bool Lexer::match(char expected) noexcept {
    if (peek() != expected) {
        return false;
    }
    ++pos_;
    return true;
}

void Lexer::skip_whitespace_and_comments() {
    for (;;) {
        char c = peek();
        if (c == ' ' || c == '\t' || c == '\r' || c == '\n') {
            advance();
            continue;
        }
        if (c == '/' && peek_next() == '/') {
            while (peek() != '\n' && peek() != '\0') {
                advance();
            }
            continue;
        }
        break;
    }
}

void Lexer::add_token(TokenKind kind, std::string text, std::size_t start_offset, std::size_t end_offset) {
    Token t;
    t.kind = kind;
    t.text = std::move(text);
    t.span.start = start_offset;
    t.span.end = end_offset;
    tokens_.push_back(std::move(t));
}

void Lexer::lex_identifier_or_keyword() {
    const std::size_t start = pos_;
    while (std::isalnum(static_cast<unsigned char>(peek())) || peek() == '_') {
        advance();
    }
    const std::size_t end = pos_;
    std::string text{source_.substr(start, end - start)};

    TokenKind kind = TokenKind::Identifier;
    if (text == "let") kind = TokenKind::KwLet;
    else if (text == "mut") kind = TokenKind::KwMut;
    else if (text == "fn") kind = TokenKind::KwFn;
    else if (text == "struct") kind = TokenKind::KwStruct;
    else if (text == "enum") kind = TokenKind::KwEnum;
    else if (text == "impl") kind = TokenKind::KwImpl;
    else if (text == "protocol") kind = TokenKind::KwProtocol;
    else if (text == "match") kind = TokenKind::KwMatch;
    else if (text == "if") kind = TokenKind::KwIf;
    else if (text == "else") kind = TokenKind::KwElse;
    else if (text == "return") kind = TokenKind::KwReturn;
    else if (text == "unsafe") kind = TokenKind::KwUnsafe;
    else if (text == "effects") kind = TokenKind::KwEffects;

    add_token(kind, std::move(text), start, end);
}

void Lexer::lex_number() {
    const std::size_t start = pos_;
    while (std::isdigit(static_cast<unsigned char>(peek()))) {
        advance();
    }
    const std::size_t end = pos_;
    std::string text{source_.substr(start, end - start)};
    add_token(TokenKind::IntegerLiteral, std::move(text), start, end);
}

void Lexer::lex_string() {
    const std::size_t start_quote = pos_;
    advance();  // consume opening quote
    const std::size_t content_start = pos_;

    std::string result;
    bool terminated = false;
    while (true) {
        char c = peek();
        if (c == '\0') {
            break;
        }
        if (c == '"') {
            terminated = true;
            advance();
            break;
        }
        if (c == '\\\\') {
            advance();
            char esc = peek();
            if (esc == 'n') {
                result.push_back('\\n');
            } else if (esc == 't') {
                result.push_back('\\t');
            } else if (esc == '"') {
                result.push_back('\"');
            } else if (esc == '\\\\') {
                result.push_back('\\\\');
            } else {
                result.push_back(esc);
            }
            advance();
        } else {
            result.push_back(c);
            advance();
        }
    }

    const std::size_t end = pos_;
    if (!terminated) {
        Diagnostic d;
        d.code = "PARSE001";
        d.message = "unterminated string literal";
        d.severity = Severity::Error;
        d.file = file_name_;
        d.span.start = start_quote;
        d.span.end = end;
        d.stage = "lexer";
        sink_.add(std::move(d));
    }

    add_token(TokenKind::StringLiteral, std::move(result), content_start, end);
}

std::vector<Token> Lexer::tokenize() {
    tokens_.clear();

    while (true) {
        skip_whitespace_and_comments();
        const std::size_t start = pos_;
        char c = peek();
        if (c == '\0') {
            break;
        }

        if (std::isalpha(static_cast<unsigned char>(c)) || c == '_') {
            lex_identifier_or_keyword();
            continue;
        }
        if (std::isdigit(static_cast<unsigned char>(c))) {
            lex_number();
            continue;
        }

        advance();
        switch (c) {
            case '(':
                add_token(TokenKind::LParen, "(", start, pos_);
                break;
            case ')':
                add_token(TokenKind::RParen, ")", start, pos_);
                break;
            case '{':
                add_token(TokenKind::LBrace, "{", start, pos_);
                break;
            case '}':
                add_token(TokenKind::RBrace, "}", start, pos_);
                break;
            case '[':
                add_token(TokenKind::LBracket, "[", start, pos_);
                break;
            case ']':
                add_token(TokenKind::RBracket, "]", start, pos_);
                break;
            case ',':
                add_token(TokenKind::Comma, ",", start, pos_);
                break;
            case ':':
                add_token(TokenKind::Colon, ":", start, pos_);
                break;
            case ';':
                add_token(TokenKind::Semicolon, ";", start, pos_);
                break;
            case '=':
                if (peek() == '>') {
                    advance();
                    add_token(TokenKind::FatArrow, "=>", start, pos_);
                } else {
                    add_token(TokenKind::Equal, "=", start, pos_);
                }
                break;
            case '+':
                add_token(TokenKind::Plus, "+", start, pos_);
                break;
            case '-':
                if (peek() == '>') {
                    advance();
                    add_token(TokenKind::Arrow, "->", start, pos_);
                } else {
                    add_token(TokenKind::Minus, "-", start, pos_);
                }
                break;
            case '*':
                add_token(TokenKind::Star, "*", start, pos_);
                break;
            case '/':
                add_token(TokenKind::Slash, "/", start, pos_);
                break;
            case '&':
                add_token(TokenKind::Ampersand, "&", start, pos_);
                break;
            case '|':
                add_token(TokenKind::Pipe, "|", start, pos_);
                break;
            case '?':
                add_token(TokenKind::Question, "?", start, pos_);
                break;
            case '"':
                --pos_;  // step back so lex_string can see the quote
                lex_string();
                break;
            default: {
                Diagnostic d;
                d.code = "PARSE002";
                d.message = "unrecognized character in input";
                d.severity = Severity::Error;
                d.file = file_name_;
                d.span.start = start;
                d.span.end = pos_;
                d.stage = "lexer";
                sink_.add(std::move(d));
                add_token(TokenKind::Unknown, std::string(1, c), start, pos_);
                break;
            }
        }
    }

    Token eof;
    eof.kind = TokenKind::EndOfFile;
    eof.text = "";
    eof.span.start = pos_;
    eof.span.end = pos_;
    tokens_.push_back(std::move(eof));

    return tokens_;
}

}  // namespace intent

