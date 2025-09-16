#include <concepts>

template <typename... Ts> concept Addable =
    requires(Ts... p) { (... + p); requires sizeof...(Ts) > 1; };

template <typename... Ts>
    requires Addable<Ts...>
auto sum_all(Ts... p) {
    return (... + p);
}

int main() {
    return sum_all(1, 2); // SUCCESS
}
