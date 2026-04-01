### Intent Language Compiler (Bootstrap)

This repository contains the **bootstrap implementation** of the Intent language compiler, targeting native binaries on Windows and macOS using LLVM.

The goal of this project is to implement a **small, strict, and coherent** systems language that is safe by default and friendly to both humans and AI tooling.

#### Layout

- `docs/` – Language and compiler design documents (foundation, scope, diagnostics, ownership, etc.).
- `examples/` – Small example programs written in the Intent language.
- `include/` – Public compiler headers.
- `src/` – Compiler implementation, organized by stage:
  - `lexer/` – Tokenization.
  - `parser/` – Parsing and AST construction.
  - `ast/` – AST node definitions.
  - `sema/` – Name resolution, type checking, ownership, and effects.
  - `ir/` – Typed intermediate representation.
  - `codegen/` – LLVM lowering and native code generation.
  - `diagnostics/` – Shared diagnostic infrastructure.
- `tests/` – Unit and integration tests by compiler stage.

#### Building

```bash
cmake -S . -B build
cmake --build build
```

The `intent` CLI will be built in the `build` directory.

