#pragma once

#include "intent/ast.h"
#include "intent/token.h"

#include <memory>
#include <vector>

namespace intent {

class Parser {
public:
    Parser(std::string file_name, const std::vector<Token>& tokens, DiagnosticSink& sink);

    std::unique_ptr<Module> parse_module();

private:
    const Token& current() const;
    const Token& peek_next() const;
    bool at_end() const;
    bool match(TokenKind kind);
    bool check(TokenKind kind) const;
    const Token& advance();

    void synchronize();

    std::unique_ptr<AstNode> parse_item();

    std::string file_name_;
    const std::vector<Token>& tokens_;
    DiagnosticSink& sink_;
    std::size_t index_{0};
};

}  // namespace intent

