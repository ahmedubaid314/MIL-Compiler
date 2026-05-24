# Statements

Top-level program structure.

$$
\text{\_PROG} \to \text{\_STMT\_LIST}  \\
\text{\_STMT\_LIST} \to \text{\_STMT \_STMT\_LIST | } \epsilon \\
\text{\_STMT} \to \text{kill(\_EXPR);} \\
\text{\_STMT} \to \text{let IDENT = \_EXPR;} \\
\text{\_STMT} \to \text{IDENT = \_EXPR;} \\
\text{\_STMT} \to \text{print(\_EXPR);} \\ 
\text{\_STMT} \to \text{\{\_STMT\_LIST\}}
$$

## Details

- **PROG**: Entry point. Matches a list of statements.
- **STMT_LIST**: Zero or more statements (epsilon allows empty programs).
- **STMT**: Four statement types:
  - `kill(expr)` — terminate with exit code
  - `let ident = expr` — declare and initialize variable
  - `ident = expr` — assign to existing variable
  - `print(expr)` — output integer to stdout

All statements end with `;`.

---

**Next:** [Expressions](expressions.md) — How expressions work inside statements

**Learn More:** [Comparisons](comparisons.md) if you want to skip to comparison operators