# ToolsLibrary

<p align="left">
    <a href="https://github.com/sebanisu/ToolsLibrary/issues" alt="contributions welcome">
        <img src="https://img.shields.io/badge/contributions-welcome-brightgreen.svg?style=flat" /></a>
    <a href="https://www.boost.org/LICENSE_1_0.txt" alt="Boost License">
        <img src="https://img.shields.io/badge/License-Boost-blue.svg" /></a>    
    <a href="https://en.cppreference.com/w/cpp/20">
        <img src="https://img.shields.io/badge/C++%20-20-ff69b4.svg"/></a>
    <a href="https://github.com/sebanisu?tab=followers" alt="GitHub followers">
        <img src="https://img.shields.io/github/followers/sebanisu.svg?style=social&label=Follow" /></a>
    <a href="https://GitHub.com/sebanisu/ToolsLibrary/stargazers/" alt="GitHub stars">
        <img src="https://img.shields.io/github/stars/sebanisu/ToolsLibrary.svg?style=social&label=Star" /></a>
</p>
I'm mirroring what [codereport](https://github.com/codereport/An-Algorithm-Library) is doing. I will be using this code in my other projects. I'm putting it into an open source library that everyone can use.

My tests are wrote using [ut](https://github.com/boost-ext/ut). This is a neat library. It doesn't require the use of macros.

1. `tl/input.hpp`
    * This struct takes a std::span, or a std::istream. This lets you output to a trivial types, or contiguous ranges of
      trivial types.
2. `tl/read.hpp`
    * This file contains free functions that use `input` to read from files.
3. `tl/concepts.hpp`
    * This file contains some concepts used by the rest of the library.
4. `tl/utility.hpp`
    * This file has some free functions that the library depends on.
    * Example: Detecting the remaining bytes in a stream. 
