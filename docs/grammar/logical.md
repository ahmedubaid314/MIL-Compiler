# Logical
Logical Operators. Lowest Precedence in an expression statement.

$$
\text{\_LEXPR} \to \text{\_CEXPR \_LEXPR\_R} \\
\text{\_LEXPR\_R} \to \text{ <logical\_op> \_CEXPR \_LEXPR\_R | } \epsilon \\ 
\text{<logical\_op>} \to \text{\&\& | ||} 
$$

# Details
- **Returns:** 1 or 0
- **Uses** short-circuiting. In `a < b && c < d` if `a < b` is false, rest of the expression is not parsed

---

**Next:** [Comparisons](comparisons.md) 

**Previous:** [Expressions](expressions.md)