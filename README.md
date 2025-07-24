# Applause Plugin Template

A minimal template for creating audio plugins using the [Applause](https://github.com/gabrielsoule/applause) framework.

## Quick Start

```bash
# Clone with submodules
git clone --recursive https://github.com/yourusername/applause-template.git
cd applause-template

# Build
cmake -B build
cmake --build build
```

Your CLAP, VST3, and AU plugin builds will be in the `build/` directory and automatically copied to your system plugin folders.

## Customization

Edit `CMakeLists.txt` to set your plugin's:
- Name, ID, vendor info
- Plugin type (instrument/effect)
- AU manufacturer codes
- Miscellaneous errata

## Requirements

- C++20 compiler
- CMake 3.21+

## License

MIT
