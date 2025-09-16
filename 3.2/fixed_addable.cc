#include <concepts>

template <typename... Ts> concept Addable =
    requires(Ts... p) { (... + p); requires sizeof...(Ts) > 1; };


/**
 * The error in original code is that the `template <Addable... Ts>` syntax checks
 * if *each type individually* satisfies the `Addable` concept.
 *
 * This means that inside the concept check, `sizeof...(Ts)` is always 1,
 * and the `sizeof...(Ts) > 1` condition always fails.
 *
 * The corrected code below uses an explicit `requires` clause to apply the concept
 * to the entire parameter pack at once
 */
template <typename... Ts>
    requires Addable<Ts...>
auto sum_all(Ts... p) {
    return (... + p);
}

int main() {
    return sum_all(1, 2); // SUCCESS
}
