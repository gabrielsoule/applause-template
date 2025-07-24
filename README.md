# Applause Plugin Template

A minimal template for creating audio plugins using the [Applause](https://github.com/free-audio/applause) framework.

## Quick Start

```bash
# Clone with submodules
git clone --recursive https://github.com/yourusername/applause-template.git
cd applause-template

# Build
cmake -B build
cmake --build build
```

Your plugins will be in the `build/` directory and automatically copied to your system plugin folders.

## Customization

Edit `CMakeLists.txt` to set your plugin's:
- Name, ID, vendor info
- Plugin type (instrument/effect)
- AU manufacturer codes (macOS)

## Requirements

- C++20 compiler
- CMake 3.21+
- Git

## License

MIT