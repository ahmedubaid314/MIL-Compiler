# Expressions

Expressions form a precedence hierarchy. Lower-precedence rules delegate to higher-precedence ones.

$$
\text{\_EXPR} \to \text{\_LEXPR}
$$

## Precedence (lowest to highest)

| Level | Rule | Operators |
|-------|------|-----------|
| 1 | LEXPR | `&&`, `\|\|`
| 2 | CEXPR | `<`, `>`, `<=`, `>=`, `==`, `!=` |
| 3 | AEXPR | `+`, `-` |
| 4 | TERM | `*`, `/` |
| 5 | FACTOR | literals, identifiers, `(expr)` |

Higher precedence binds tighter. So `1 + 2 * 3` parses as `1 + (2 * 3)`.

---

**Next by level:**
- [Logical](logical.md) — Logical operators 

**Previous:** [Statements](statements.md)
