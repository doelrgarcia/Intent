#pragma once

#include <string>
#include <string_view>
#include <vector>

namespace intent {

struct Span {
    std::size_t start{};
    std::size_t end{};
};

struct RelatedSpan {
    std::string label;
    Span span{};
};

enum class Severity {
    Error,
    Warning,
    Note,
    Info,
};

struct Diagnostic {
    std::string code;
    std::string message;
    Severity severity{Severity::Error};
    std::string file;
    Span span{};
    std::vector<RelatedSpan> related;
    std::vector<std::string> hints;
    std::string stage;
};

class DiagnosticSink {
public:
    void add(Diagnostic diag);

    const std::vector<Diagnostic>& all() const noexcept { return diagnostics_; }

    bool has_errors() const noexcept;

private:
    std::vector<Diagnostic> diagnostics_;
};

std::string to_json(const std::vector<Diagnostic>& diagnostics);

std::string_view to_string(Severity severity) noexcept;

}  // namespace intent

