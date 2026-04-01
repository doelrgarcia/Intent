### Intent Language – Lexical Grammar (Draft)

This document defines the **lexical layer** for Intent v1: how source text is broken into a stream of tokens.

The lexer is **whitespace-insensitive** except where whitespace is required to avoid ambiguity (e.g., separating identifiers), and it tracks byte offsets for diagnostics.

#### Comments

- Line comments start with `//` and continue to the end of the line.
- Block comments are not included in v1.

#### Whitespace

- Spaces, tabs, carriage returns, and newlines are treated as separators.
- Whitespace is otherwise ignored by the parser.

#### Identifiers

- Pattern: `[A-Za-z_][A-Za-z0-9_]*`
- Examples: `x`, `User`, `my_var1`

#### Keywords (reserved identifiers)

The following identifiers are reserved as keywords in v1:

- `let`
- `mut`
- `fn`
- `struct`
- `enum`
- `impl`
- `protocol`
- `match`
- `if`
- `else`
- `return`
- `unsafe`
- `effects`

#### Literals

- **Integer literals**
  - Pattern: `[0-9]+`
  - Only decimal literals are required for v1.
- **String literals**
  - Delimited by double quotes: `"..."`.
  - Support basic escape sequences: `\\n`, `\\t`, `\\\"`, `\\\\`.

#### Punctuation and operators

The lexer recognizes the following single- and multi-character tokens:

- Single-character: `(`, `)`, `{`, `}`, `[`, `]`, `,`, `:`, `;`, `=`, `+`, `-`, `*`, `/`, `&`, `|`, `?`
- Multi-character:
  - `->`
  - `=>`

The lexer prefers the **longest matching token** when ambiguity exists (e.g., `->` instead of `-` and `>`).

