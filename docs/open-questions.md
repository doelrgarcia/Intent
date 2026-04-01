### Intent Language – Open Questions (V1 Track)

This file tracks the intentional open questions from the foundation spec. Each item should be answered explicitly before a 1.0 language freeze, and any interim choices in the compiler must be documented here.

#### 1. Language name and file extension
- **Question**: What is the final language name and canonical source-file extension?
- **Current working assumption**: Use the working name **Intent** with the provisional extension `.intent`.
- **Rationale**: Keeps the toolchain concrete while leaving room to rename prior to 1.0.
- **Action to close**: Choose final branding and update CLI help, docs, and examples.

#### 2. Field-level mutability in structs
- **Question**: Should v1 support field-level mutability (immutable struct binding with selectively mutable fields)?
- **Current working assumption**: Start with **uniform mutability** for v1:
  - A `let` binding is fully immutable.
  - A `let mut` binding allows mutation of the entire value, including all fields.
- **Rationale**: This keeps the ownership and borrow rules simpler for both humans and AI while still allowing practical mutation patterns.
- **Action to close**: If field-level mutability proves necessary, add a targeted design and update the ownership checker rules.

#### 3. Effects declaration syntax
- **Question**: What is the exact surface syntax for declaring effects on functions and methods?
- **Current working assumption**: A **post-signature effects clause**, for example:

  ```text
  fn save_report(path: string, data: Report) -> Result<(), FileError>
      effects(IO, FileWrite)
  {
      ...
  }
  ```

- **Rationale**: Keeps the function signature readable and groups effects in a single, easily parsed list.
- **Action to close**: Lock the grammar once the parser is implemented and we have at least one real-world example using effects.

#### 4. Minimum generics support for v1
- **Question**: How much generic capability is required in v1 before it becomes a distraction?
- **Current working assumption**:
  - Treat `Result<T, E>` and `Option<T>` as **built-in, special-cased generic forms** in the type system.
  - Defer general-purpose user-defined generics (generic functions, structs, and enums) to a post-v1 milestone.
- **Rationale**: This covers the most important error-handling and optionality patterns without committing to a full trait system or complex type features.
- **Action to close**: Revisit after the core compiler pipeline (through LLVM) is stable and we have concrete ergonomics feedback.

#### 5. Strings, slices, and collections under ownership
- **Question**: What are the concrete v1 representations for strings, slices, and basic collections under the ownership model?
- **Current working assumption**:
  - **Strings**: A single owned string type (e.g., `string`) with move semantics; borrowing exposes a read-only view.
  - **Slices**: Read-only borrowed views over contiguous data (e.g., `&[T]`–style semantics) without full lifetime expressiveness.
  - **Collections**: Keep v1 minimal (e.g., a small, standard growable buffer type) and prefer examples that do not require a rich collection library.
- **Rationale**: This avoids over-designing the standard library while keeping the ownership rules demonstrable and testable.
- **Action to close**: Lock concrete type names and memory layout once the IR and codegen layer are defined.

#### 6. Method receiver forms
- **Question**: Which receiver forms should be supported for methods in v1?
- **Current working assumption**:
  - Support three receiver forms:
    - By value (consuming `self`).
    - Immutable borrow (`&self`-style).
    - Mutable borrow (`&mut self`-style).
- **Rationale**: This is the minimum needed to model ownership transfers, read-only access, and in-place mutation without introducing inheritance.
- **Action to close**: Confirm syntax and defaulting rules once the method and protocol syntax are implemented in the parser.

#### 7. Protocol/interface surface and minimum polymorphism
- **Question**: What is the simplest protocol/interface syntax and feature set that still provides useful polymorphism?
- **Current working assumption**:
  - A protocol is a named set of required methods with no default implementations and no inheritance between protocols.
  - Types opt in explicitly by implementing the protocol.
- **Rationale**: This aligns with the spec’s emphasis on composition over inheritance and keeps dynamic behavior explicit.
- **Action to close**: Finalize syntax and semantics once we have at least one concrete protocol in the examples (e.g., `Writer`).

