#pragma once

#include "intent/token.h"

#include <memory>
#include <string>
#include <vector>

namespace intent {

struct AstNode {
    Span span{};
    virtual ~AstNode() = default;
};

struct Expr : AstNode {
};

struct Statement : AstNode {
};

struct Block : AstNode {
    std::vector<std::unique_ptr<Statement>> statements;
};

struct Function : AstNode {
    std::string name;
    Block body;
};

struct Module : AstNode {
    std::vector<std::unique_ptr<AstNode>> items;
};

}  // namespace intent

