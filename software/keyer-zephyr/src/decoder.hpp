#pragma once
#include <array>
#include <string_view>
#include <optional>

namespace morse {

class Decoder {
public:
  constexpr Decoder() noexcept = default;

  void InsertSymbol(bool is_dash) noexcept;

  [[nodiscard]] std::optional<char> Flush() noexcept;

  void Reset() noexcept { currentIndex_ = 0; }

private:
  size_t currentIndex_{0};
  // Reprezentacja drzewa binarnego w tablicy (indeksowanie od 0)
  // Lewe dziecko = indeks * 2 + 1 (kropka)
  // Prawe dziecko = indeks * 2 + 2 (kreska)
  // '#' oznacza brak znaku na danej pozycji
  static constexpr std::array<char, 127> tree_{
    '#',                                             // 0: Root (pusty)

    // Poziom 1 (1 symbol)
    'E', 'T',                                        // 1-2

    // Poziom 2 (2 symbole)
    'I', 'A', 'N', 'M',                              // 3-6

    // Poziom 3 (3 symbole)
    'S', 'U', 'R', 'W', 'D', 'K', 'G', 'O',          // 7-14

    // Poziom 4 (4 symbole)
    'H', 'V', 'F', '#', 'L', '#', 'P', 'J',          // 15-22
    'B', 'X', 'C', 'Y', 'Z', 'Q', '#', '#',          // 23-30

    // Poziom 5 (5 symboli - tutaj lądują wszystkie cyfry)
    '5', '4', '#', '3', '#', '#', '#', '2',          // 31-38
    '#', '#', '#', '#', '#', '#', '#', '1',          // 39-46
    '6', '=', '/', '#', '#', '#', '#', '#',          // 47-54
    '7', '#', '#', '#', '8', '#', '9', '0',          // 55-62

    // Poziom 6 (Pominięty/Wypełniony '#' dla zachowania struktury do indeksu 126)
    '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#',
    '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#',
    '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#',
    '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'
};
};

}  // namespace morse