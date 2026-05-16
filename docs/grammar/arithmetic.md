# Arithmetic

Addition and subtraction (and multiplication/division via TERM).

$$
\text{\_AEXPR} \to \text{\_TERM \_AEXPR\_R} \\
\text{\_AEXPR\_R} \to \text{ + \_TERM \_AEXPR\_R | - \_TERM \_AEXPR\_R | } \epsilon \\
\text{\_TERM} \to \text{\_FACTOR \_TERM\_R} \\
\text{\_TERM\_R} \to \text{* \_FACTOR \_TERM\_R | / \_FACTOR \_TERM\_R | } \epsilon
$$

## Details

- **AEXPR**: Addition and subtraction (lower precedence)
- **TERM**: Multiplication and division (higher precedence)
- **Associativity:** Left-associative. `a + b + c` = `(a + b) + c`

Multiplication and division bind tighter than addition/subtraction:
- `1 + 2 * 3` → `1 + (2 * 3)` → `7`
- `6 / 2 + 1` → `(6 / 2) + 1` → `4`

---

**Next:** [Factors](factors.md) — What AEXPR operands are made of

**Previous:** [Comparisons](comparisons.md)