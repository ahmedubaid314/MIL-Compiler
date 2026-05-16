# Factors

Base expressions. Highest precedence.

$$
\text{\_FACTOR} \to \text{IDENT | INT\_LIT | (\_EXPR)}
$$

## Details

- **IDENT**: Variable name (previously declared with `let`)
- **INT_LIT**: Integer literal (32-bit signed, no leading zeros)
- **(EXPR)**: Parenthesized expression (recursively can be any expression)

Parentheses let you override precedence:
- `2 + 3 * 4` → `2 + (3 * 4)` → `14`
- `(2 + 3) * 4` → `5 * 4` → `20`

---

**Previous:** [Arithmetic](arithmetic.md)

**Back to:** [Expressions](expressions.md) for full hierarchy | [Statements](statements.md) for context