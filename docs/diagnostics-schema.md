### Intent Compiler – Diagnostics Schema

Diagnostics are a first-class feature of the Intent compiler. They must be stable, structured, and easy for both humans and tools to consume.

This document defines the **machine-readable JSON shape** and high-level conventions for error codes and severities.

#### JSON Envelope

The compiler emits diagnostics as a JSON array at the top level:

```json
[
  {
    "code": "BORROW001",
    "message": "cannot move value while borrowed",
    "severity": "error",
    "file": "main.intent",
    "span": { "start": 120, "end": 148 },
    "relatedSpans": [
      { "label": "borrow begins here", "start": 88, "end": 97 }
    ],
    "hints": [
      "clone the value before moving it",
      "end the borrow before the move"
    ],
    "stage": "ownership"
  }
]
```

#### Field Definitions

- **`code`** (string, required)
  - Stable identifier for this diagnostic.
  - Format: `SUBSYSTEMNNN`, where:
    - `SUBSYSTEM` is an uppercase mnemonic (e.g., `PARSE`, `TYPE`, `BORROW`, `IR`, `CODEGEN`).
    - `NNN` is a zero-padded numeric identifier (`001`, `002`, ...).

- **`message`** (string, required)
  - Human-oriented description of the problem.
  - Short, direct, and suitable for display in a terminal or editor.

- **`severity`** (string, required)
  - One of: `"error"`, `"warning"`, `"note"`, `"info"`.
  - The compiler treats `"error"` as blocking for successful builds.

- **`file`** (string, required)
  - Path to the source file (as seen by the user), e.g., `src/main.intent`.

- **`span`** (object, required)
  - Primary span associated with this diagnostic.
  - Fields:
    - `start` (integer, byte or character offset from file start).
    - `end` (integer, exclusive).

- **`relatedSpans`** (array of objects, optional)
  - Additional locations that help explain the diagnostic.
  - Each object has:
    - `label` (string): Short description (e.g., `"borrow begins here"`).
    - `start` (integer).
    - `end` (integer).

- **`hints`** (array of strings, optional)
  - Concrete suggestions for fixing the issue.
  - Each entry should be a direct, actionable sentence.

- **`stage`** (string, optional)
  - Name of the compiler stage that produced the diagnostic.
  - Suggested values: `"lexer"`, `"parser"`, `"name_resolution"`, `"type_check"`, `"ownership"`, `"effects"`, `"ir"`, `"codegen"`.

#### Text Output Mode

In text mode, the compiler is free to format diagnostics in a human-friendly way (e.g., with file paths, line/column numbers, and annotated snippets). The **semantic content** of those diagnostics must still be representable in the JSON schema above.

#### Error Code Ranges (Initial Proposal)

These ranges are reserved for early v1:

- `PARSE001`–`PARSE099`: Syntax and lexing errors.
- `NAME001`–`NAME099`: Name resolution (unresolved names, duplicates).
- `TYPE001`–`TYPE099`: Type mismatches and inference failures.
- `BORROW001`–`BORROW099`: Ownership and borrow checker errors.
- `EFFECT001`–`EFFECT099`: Effects declaration and usage mismatches.
- `IR001`–`IR099`: Typed IR construction issues.
- `CODEGEN001`–`CODEGEN099`: LLVM lowering and backend problems.

#### Stability Expectations

- Once an error code is used in a release, it should **not change meaning**.
- New diagnostics should prefer adding new codes rather than overloading existing ones.
- Tooling (including AI agents) can rely on `code` + `stage` as a stable contract for automated repair loops.

