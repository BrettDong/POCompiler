# POCompiler
![C++](https://img.shields.io/badge/language-C++17-green)
![GPLv3](https://img.shields.io/badge/license-GPLv3-blue)

A tool to transform UTF-8 encoded GNU gettext translation data from `.po` source format to `.mo` binary format, that can
be used as a hacky replacement for `msgfmt` to speed-up translation compilation.

## Why

The `msgfmt` command is slow, spending time on character set encoding conversions back-and-forth, correctness checking, and other GNU gettext features. Compiling translations using `msgfmt` in a big project with two-digit number of target languages can easily take half a minute or so.

In use cases where file encoding is always UTF-8 and sanity check is unneeded, and no patience to wait for so long to compile translations, the time spent in `msgfmt` on encoding conversion and other stuff is undesirable. So, _to not pay for what you don't use_, this tool was written for that use case to speed up translation compilation. It's 10x-20x faster than `msgfmt`, at the expense of no character set encoding awareness, no input sanity check, and no full gettext feature support. But reduces waiting time from half a minute to 1-2 seconds.

## License

GPLv3.

<img src="https://upload.wikimedia.org/wikipedia/commons/9/93/GPLv3_Logo.svg" alt="GPLv3" width="150"/>

## Build

Requires a modern version of CMake and a C++-17 compiler. Dependencies are managed by CMake `FetchContent`:

* [`fmtlib`](https://github.com/fmtlib/fmt)
* [`doctest`](https://github.com/doctest/doctest)

Known to compile on MSVC 19.33, GCC 9, AppleClang 14.

## Usage

Example command line to compile a `.po` file and write to a `.mo` file in a different directory:

```
POCompiler -o $PROJECT_BUILD/i18n/ja/LC_MESSAGES/main.mo $PROJECT_SRC/i18n/ja.po
```

Output directory needs to exist before invoking the command. This program won't create output directories for you.
