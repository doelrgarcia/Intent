### Intent Language – V1 Scope

This document captures the intentionally **small, strict, and coherent** surface for Intent v1, derived from the foundation spec. It is divided into **included** and **explicitly out-of-scope** features.

#### Included in V1

- **Core types**
  - Fixed-width integers (e.g., `i32`, `i64`).
  - Booleans (`bool`).
  - Strings (`string`) with ownership-aware semantics.

- **Bindings and mutation**
  - `let` bindings that are **immutable by default**.
  - `let mut` for explicitly mutable bindings.

- **Functions**
  - Named functions with explicit parameter and return types.
  - A clear distinction between pure and effectful functions via the effects list.

- **Structs and enums**
  - Structs with named fields.
  - Enums with variants, including sum types such as `Result` and `Option`.

- **Control flow**
  - `if` / `else` expressions.
  - Loops (a single, simple loop form is sufficient for v1).
  - `match` expressions that are **exhaustive and non-fallthrough**.

- **Error handling**
  - Typed `Result<T, E>` and `Option<T>`.
  - A propagation operator (similar to `?`) to return early on error.
  - Ignoring a `Result` (or other must-handle types) is a compile-time error or high-severity diagnostic.
  - `panic` reserved for unrecoverable internal faults, not normal control flow.

- **Ownership and safety**
  - Values move by default.
  - Immutable borrows.
  - Exclusive mutable borrows.
  - Compile-time errors for use-after-move and invalid aliasing of mutable state.
  - Explicit `unsafe` blocks as narrow escape hatches.

- **Effects system**
  - Explicit effect lists on functions/methods (e.g., `effects(IO, FileWrite)`).
  - A small, fixed set of effect categories: `IO`, `FileRead`, `FileWrite`, `Network`, `ProcessExec`, `Console`, `Time`, `Database`, `UnsafeMemory`.

- **Object model**
  - Structs with methods.
  - Protocols/interfaces for polymorphism.
  - Composition as the primary mechanism for code reuse.

- **Compiler behavior**
  - Native compilation on Windows and macOS using LLVM.
  - A typed IR between the high-level AST and LLVM.
  - Machine-readable diagnostics (JSON) in addition to human-readable text.
  - CLI workflow: `intent check` and `intent build`.

#### Explicitly Out of Scope for V1

- **Memory management features**
  - No garbage collector in the core language.
  - No automatic reference counting built into the language semantics.

- **Metaprogramming and macros**
  - No general macro system.
  - No compile-time code execution beyond what is required for the compiler itself.

- **Advanced type system features**
  - No full trait system with complex bounds and constraints.
  - No higher-kinded types.
  - No advanced lifetime annotations beyond what is required for the basic ownership model.

- **Object-oriented features**
  - No class-based inheritance hierarchies.
  - No virtual dispatch by default on all method calls.
  - No hidden object lifecycles or mandatory heap allocation.

- **Runtime and ecosystem**
  - No full async runtime.
  - No large or complex package manager.
  - No reflection or runtime type inspection APIs.

- **Tooling beyond the core compiler**
  - No language server protocol (LSP) implementation in v1.
  - No IDE-specific features beyond what can be built on top of the CLI and diagnostics.

#### V1 Exit Criteria (Conceptual)

Intent v1 is considered feature-complete when:

- The compiler can parse, check, and compile representative programs using:
  - Structs, enums, and methods.
  - `Result`-based error handling with the propagation operator.
  - Ownership-aware operations and basic borrowing.
  - Declared effects for visible side effects.
- Diagnostics are stable, machine-readable, and useful to both humans and automated tools.
- The implementation stays within the **non-goals**: no macros, no heavy async, no complex trait system, and no inheritance.

