#include "intent/diagnostic.h"
#include "intent/parser.h"
#include "intent/token.h"

#include <fstream>
#include <iostream>
#include <string>

using namespace std::string_literals;

namespace {

struct CommandLine {
    enum class Mode {
        Help,
        ParseOnly,
    };

    Mode mode{Mode::Help};
    std::string input_path;
    bool json_output{false};
};

void print_help() {
    std::cout << "intent - Intent language compiler (bootstrap)\\n";
    std::cout << \"Usage:\\n\";
    std::cout << \"  intent parse <file> [--json]\\n\";
}

std::optional<CommandLine> parse_args(int argc, char** argv, intent::DiagnosticSink& diags) {
    if (argc < 2) {
        return CommandLine{CommandLine::Mode::Help, {}, false};
    }

    CommandLine cmd;
    std::string mode = argv[1];
    if (mode == \"parse\") {
        cmd.mode = CommandLine::Mode::ParseOnly;
    } else if (mode == \"-h\" || mode == \"--help\") {
        cmd.mode = CommandLine::Mode::Help;
        return cmd;
    } else {
        intent::Diagnostic d;
        d.code = \"CLI001\";
        d.message = \"unknown command: \" + mode;
        d.severity = intent::Severity::Error;
        d.stage = \"cli\";
        diags.add(std::move(d));
        return std::nullopt;
    }

    if (cmd.mode == CommandLine::Mode::ParseOnly) {
        if (argc < 3) {
            intent::Diagnostic d;
            d.code = \"CLI002\";
            d.message = \"missing input file for parse mode\";
            d.severity = intent::Severity::Error;
            d.stage = \"cli\";
            diags.add(std::move(d));
            return std::nullopt;
        }
        cmd.input_path = argv[2];
        for (int i = 3; i < argc; ++i) {
            std::string_view flag{argv[i]};
            if (flag == \"--json\") {
                cmd.json_output = true;
            }
        }
    }

    return cmd;
}

std::string read_file(const std::string& path, intent::DiagnosticSink& diags) {
    std::ifstream in(path, std::ios::binary);
    if (!in) {
        intent::Diagnostic d;
        d.code = \"IO001\";
        d.message = \"failed to open input file\";
        d.severity = intent::Severity::Error;
        d.file = path;
        d.stage = \"cli\";
        diags.add(std::move(d));
        return {};
    }
    std::string contents;
    in.seekg(0, std::ios::end);
    contents.resize(static_cast<std::size_t>(in.tellg()));
    in.seekg(0, std::ios::beg);
    in.read(contents.data(), static_cast<std::streamsize>(contents.size()));
    return contents;
}

}  // namespace

int main(int argc, char** argv) {
    intent::DiagnosticSink diagnostics;

    auto cmd_opt = parse_args(argc, argv, diagnostics);
    if (!cmd_opt.has_value()) {
        if (!diagnostics.all().empty()) {
            std::cerr << intent::to_json(diagnostics.all()) << \"\\n\";
        } else {
            print_help();
        }
        return 1;
    }

    const CommandLine cmd = *cmd_opt;
    if (cmd.mode == CommandLine::Mode::Help) {
        print_help();
        return 0;
    }

    const std::string source = read_file(cmd.input_path, diagnostics);
    if (diagnostics.has_errors()) {
        if (cmd.json_output) {
            std::cout << intent::to_json(diagnostics.all()) << \"\\n\";
        } else {
            std::cerr << \"error: failed to read input file\\n\";
        }
        return 1;
    }

    Lexer lexer(cmd.input_path, source, diagnostics);
    std::vector<Token> tokens = lexer.tokenize();

    Parser parser(cmd.input_path, tokens, diagnostics);
    auto module = parser.parse_module();
    (void)module;

    if (cmd.json_output) {
        std::cout << intent::to_json(diagnostics.all()) << \"\\n\";
    }

    return diagnostics.has_errors() ? 1 : 0;
}

