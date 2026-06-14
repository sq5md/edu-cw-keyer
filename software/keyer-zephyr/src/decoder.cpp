#include "decoder.hpp"
void morse::Decoder::InsertSymbol(bool is_dash) noexcept
{
    if (is_dash) {
        currentIndex_ = (currentIndex_ * 2) + 2;
    }
    else {
        currentIndex_ = (currentIndex_ * 2) + 1;
    }

    if (currentIndex_ >= tree_.size()) {
        Reset();
    }
}
std::optional<char> morse::Decoder::Flush() noexcept
{
    if (currentIndex_ == 0 || currentIndex_ >= tree_.size()) {
        return std::nullopt;
    }
    char decoded = tree_[currentIndex_];
    Reset();
    return decoded != '#' ? std::make_optional(decoded) : std::nullopt;
}