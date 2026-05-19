#pragma once

namespace utl {

/**
 * A struct to represent a plane - such as a screen
 */
struct Box {
    Box() : w{}, h{} {}
    Box(int width, int height) : w{width}, h{height} {}

    int w;
    int h;
};

}  // namespace utl
