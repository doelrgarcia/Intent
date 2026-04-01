### Intent Language – Core Syntax (Draft)

This document sketches the **minimal v1 concrete syntax** that the parser supports.

The grammar below is informal but aims to be unambiguous and easy for tooling to generate.

#### Modules

An Intent source file is a sequence of **items**:

```text
module    ::= item*
item      ::= function_item
           |  struct_item
           |  enum_item
```

#### Functions

```text
function_item ::=
    "fn" IDENT "(" param_list? ")" "->" type
        effects_clause?
        block

param_list   ::= param ("," param)*
param        ::= IDENT ":" type

effects_clause ::=
    "effects" "(" IDENT ("," IDENT)* ")"
```

#### Structs and enums

```text
struct_item ::=
    "struct" IDENT "{" struct_field_list? "}"

struct_field_list ::=
    struct_field ("," struct_field)* ","?

struct_field ::=
    IDENT ":" type

enum_item ::=
    "enum" IDENT "{" enum_variant_list? "}"

enum_variant_list ::=
    enum_variant ("," enum_variant)* ","?

enum_variant ::=
    IDENT
  | IDENT "(" type ("," type)* ")"
```

#### Types (minimal v1)

```text
type ::=
    IDENT                       # named type, e.g. i32, string, User
  | IDENT "<" type ">"          # simple single-parameter forms like Result<T, E> may be special-cased later
```

The initial implementation is free to treat `Result` and `Option` as **special cases** in the type checker rather than fully general generics.

#### Statements and expressions

```text
block      ::= "{" statement* "}"

statement  ::= let_stmt
            |  expr_stmt
            |  return_stmt

let_stmt   ::= "let" ("mut")? IDENT ":" type "=" expr ";"

return_stmt ::=
    "return" expr? ";"

expr_stmt  ::= expr ";"
```

#### Expressions (core subset)

```text
expr        ::= match_expr

match_expr  ::= "match" expr "{" match_arm+ "}"

match_arm   ::= pattern "=>" block

pattern     ::= IDENT                      # simple name binding
             | IDENT "(" pattern_list? ")" # enum variant destructuring

pattern_list ::= pattern ("," pattern)* ","?
```

Additional expression forms (binary operators, function calls, field access) can be introduced incrementally as the type checker and ownership model mature.

