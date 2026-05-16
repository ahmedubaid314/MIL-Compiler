# Expressions

Expressions form a precedence hierarchy. Lower-precedence rules delegate to higher-precedence ones.

$$
\text{\_EXPR} \to \text{\_CEXPR}
$$

## Precedence (lowest to highest)

| Level | Rule | Operators |
|-------|------|-----------|
| 1 | CEXPR | `<`, `>`, `<=`, `>=`, `==`, `!=` |
| 2 | AEXPR | `+`, `-` |
| 3 | TERM | `*`, `/` |
| 4 | FACTOR | literals, identifiers, `(expr)` |

Higher precedence binds tighter. So `1 + 2 * 3` parses as `1 + (2 * 3)`.

---

**Next by level:**
- [Comparisons](comparisons.md) — Comparison operators (level 1)
- [Arithmetic](arithmetic.md) — Addition and subtraction (level 2)
- [Factors](factors.md) — Base expressions (level 4)

**Previous:** [Statements](statements.md)