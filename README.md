### Intent Language

Intent is a native, memory-safe, **AI-first** systems programming language.  
This repository contains the bootstrap C++ compiler that targets LLVM and emits native binaries for Windows and macOS.

The goal of v1 is not to be feature-complete; it is to be **small, strict, and predictable** so both humans and AI tools can generate and safely repair code.

#### Goals

- **Native, no-GC core**: memory-safe by default, without depending on a garbage collector.
- **Explicit ownership and effects**: move semantics, borrow checking, and effect lists for I/O and other side effects.
- **Typed error handling**: `Result`/`Option` with a propagation operator, not hidden exceptions.
- **First-class diagnostics**: stable error codes and machine-readable JSON diagnostics designed for automated fix loops.

#### Repository layout

- `docs/` – Language and compiler design docs (foundation spec, v1 scope, diagnostics schema, grammar drafts, etc.).
- `examples/` – Small example programs written in Intent.
- `include/` – Public compiler headers.
- `src/` – Compiler implementation, organized by stage:
  - `lexer/` – Tokenization and source spans.
  - `parser/` – Parsing and AST construction.
  - `ast/` – AST node definitions.
  - `sema/` – Name resolution, type checking, ownership, and effects (planned).
  - `ir/` – Typed intermediate representation (planned).
  - `codegen/` – LLVM lowering and native code generation (planned).
  - `diagnostics/` – Shared diagnostic infrastructure and JSON output.
- `tests/` – Unit and integration tests per compiler stage.

#### Status

- Repository skeleton, docs, diagnostics core, lexer, and a minimal parser/CLI are scaffolded.
- Name resolution, type checking, ownership/effects, IR, and LLVM lowering are planned but not yet fully implemented.

#### Building (once CMake/LLVM are installed)

```bash
cmake -S . -B build
cmake --build build
```

This should produce the `intent` CLI in the `build` directory.

