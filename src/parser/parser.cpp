#include "intent/parser.h"

namespace intent {

Parser::Parser(std::string file_name, const std::vector<Token>& tokens, DiagnosticSink& sink)
    : file_name_(std::move(file_name)), tokens_(tokens), sink_(sink) {}

const Token& Parser::current() const {
    if (index_ >= tokens_.size()) {
        return tokens_.back();
    }
    return tokens_[index_];
}

const Token& Parser::peek_next() const {
    if (index_ + 1 >= tokens_.size()) {
        return tokens_.back();
    }
    return tokens_[index_ + 1];
}

bool Parser::at_end() const {
    return current().kind == TokenKind::EndOfFile;
}

const Token& Parser::advance() {
    if (!at_end()) {
        ++index_;
    }
    return tokens_[index_ - 1];
}

bool Parser::check(TokenKind kind) const {
    if (at_end()) return false;
    return current().kind == kind;
}

bool Parser::match(TokenKind kind) {
    if (!check(kind)) return false;
    advance();
    return true;
}

void Parser::synchronize() {
    // Simple error recovery: advance until we reach a likely item boundary.
    while (!at_end()) {
        if (current().kind == TokenKind::Semicolon ||
            current().kind == TokenKind::KwFn ||
            current().kind == TokenKind::KwStruct ||
            current().kind == TokenKind::KwEnum) {
            return;
        }
        advance();
    }
}

std::unique_ptr<AstNode> Parser::parse_item() {
    // For now, we accept any token sequence as an opaque item until EndOfFile,
    // and let later passes evolve richer structure.
    auto node = std::make_unique<AstNode>();
    if (!at_end()) {
        node->span = current().span;
        advance();
    }
    return node;
}

std::unique_ptr<Module> Parser::parse_module() {
    auto module = std::make_unique<Module>();
    module->span.start = 0;
    if (!tokens_.empty()) {
        module->span.end = tokens_.back().span.end;
    }

    while (!at_end()) {
        if (current().kind == TokenKind::EndOfFile) {
            break;
        }
        auto item = parse_item();
        if (item) {
            module->items.push_back(std::move(item));
        } else {
            synchronize();
        }
    }

    return module;
}

}  // namespace intent

