#include "intent/diagnostic.h"

#include <sstream>

namespace intent {

void DiagnosticSink::add(Diagnostic diag) {
    diagnostics_.push_back(std::move(diag));
}

bool DiagnosticSink::has_errors() const noexcept {
    for (const auto& d : diagnostics_) {
        if (d.severity == Severity::Error) {
            return true;
        }
    }
    return false;
}

std::string_view to_string(Severity severity) noexcept {
    switch (severity) {
        case Severity::Error: return "error";
        case Severity::Warning: return "warning";
        case Severity::Note: return "note";
        case Severity::Info: return "info";
    }
    return "error";
}

static void append_escaped(std::ostringstream& os, const std::string& value) {
    for (char ch : value) {
        if (ch == '\\\\') {
            os << "\\\\\\\\";
        } else if (ch == '\"') {
            os << "\\\\\"";
        } else if (ch == '\\n') {
            os << "\\\\n";
        } else {
            os << ch;
        }
    }
}

std::string to_json(const std::vector<Diagnostic>& diagnostics) {
    std::ostringstream os;
    os << \"[\";
    bool first_diag = true;
    for (const auto& d : diagnostics) {
        if (!first_diag) {
            os << \",\";
        }
        first_diag = false;
        os << \"{\\\"code\\\":\\\"\";
        append_escaped(os, d.code);
        os << \"\\\",\\\"message\\\":\\\"\";
        append_escaped(os, d.message);
        os << \"\\\",\\\"severity\\\":\\\"\";
        os << to_string(d.severity);
        os << \"\\\",\\\"file\\\":\\\"\";
        append_escaped(os, d.file);
        os << \"\\\",\\\"span\\\":{\\\"start\\\":\" << d.span.start << \",\\\"end\\\":\" << d.span.end << \"}\";

        if (!d.related.empty()) {
            os << \",\\\"relatedSpans\\\":[\";
            bool first_rel = true;
            for (const auto& r : d.related) {
                if (!first_rel) {
                    os << \",\";
                }
                first_rel = false;
                os << \"{\\\"label\\\":\\\"\";
                append_escaped(os, r.label);
                os << \"\\\",\\\"start\\\":\" << r.span.start << \",\\\"end\\\":\" << r.span.end << \"}\";
            }
            os << \"]\";
        }

        if (!d.hints.empty()) {
            os << \",\\\"hints\\\":[\";
            bool first_hint = true;
            for (const auto& h : d.hints) {
                if (!first_hint) {
                    os << \",\";
                }
                first_hint = false;
                os << \"\\\"\";
                append_escaped(os, h);
                os << \"\\\"\";
            }
            os << \"]\";
        }

        if (!d.stage.empty()) {
            os << \",\\\"stage\\\":\\\"\";
            append_escaped(os, d.stage);
            os << \"\\\"\";
        }

        os << \"}\";
    }
    os << \"]\";
    return os.str();
}

}  // namespace intent

