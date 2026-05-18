# Factors

Base expressions. 

$$
\text{\_FACTOR} \to \text{\_UNARY} \\ 
\text{\_UNARY} \to \text{<unary\_op> \_UNARY | IDENT | INT\_LIT | (\_EXPR)} \\ 
\text{<unary\_op>} \to \text{- | !}
$$

## Details

- **UNARY**: Unary expressions (negation, logical NOT)
- **(EXPR)**: Parenthesized expression (recursively can be any expression)

Parentheses override precedence:
- `2 + 3 * 4` → `2 + (3 * 4)` → `14`
- `(2 + 3) * 4` → `5 * 4` → `20`

---

**Previous:** [Unary](unary.md)

**Back to:** [Expressions](expressions.md) for full hierarchy | [Statements](statements.md) for context