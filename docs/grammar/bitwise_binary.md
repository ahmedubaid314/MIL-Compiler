# Bitwise Binary Operators

Bitwise AND, OR, XOR, and shifts.

$$
\text{\_BOR} \to \text{\_BXOR \_BOR\_R} \\
\text{\_BOR\_R} \to \text{ | \_BXOR \_BOR\_R  | } \epsilon \\
\text{\_BXOR} \to \text{\_BAND \_BXOR\_R} \\
\text{\_BXOR\_R} \to \text{ \^ \_BAND \_BXOR\_R | } \epsilon \\
\text{\_BAND} \to \text{\_SHIFT \_BAND\_R} \\
\text{\_BAND\_R} \to \text{ \& \_SHIFT \_BAND\_R | } \epsilon \\
\text{\_SHIFT} \to \text{\_AEXPR \_SHIFT\_R} \\
\text{\_SHIFT\_R} \to \text{ << \_AEXPR \_SHIFT\_R | >> \_AEXPR \_SHIFT\_R | } \epsilon
$$

## Details

- **OR** (`|`): Bitwise OR (lowest precedence of bitwise)
- **XOR** (`^`): Bitwise exclusive OR
- **AND** (`&`): Bitwise AND (highest precedence of bitwise)
- **Shifts** (`<<`, `>>`): Left and right logical shifts
- **Associativity:** Left-associative for all

---

**Next:** [Arithmetic](arithmetic.md)

**Previous:** [Comparisons](comparisons.md)