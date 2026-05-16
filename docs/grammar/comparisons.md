# Comparisons

Comparison operators.

$$
\text{\_CEXPR} \to \text{\_AEXPR \_CEXPR\_R} \\
\text{\_CEXPR\_R} \to \text{ <comp\_op> \_AEXPR \_CEXPR\_R | } \epsilon \\
\text{<comp\_op>} \to \text{< | > | <= | >= | == | !=}
$$

## Details

- **Returns:** 1 (true) or 0 (false)
- **Associativity:** Left-associative. `a < b < c` parses as `(a < b) < c`
- **Operands:** Arithmetic expressions. Compare the *result* of arithmetic, not individual terms.

Example: `1 + 2 < 3 * 4` evaluates `(1 + 2)` and `(3 * 4)`, then compares: `3 < 12` → `1`

---

**Next:** [Arithmetic](arithmetic.md) — See what operands look like (arithmetic expressions)

**Previous:** [Logical](logical.md)

**See Also:** [Factors](factors.md) for base values