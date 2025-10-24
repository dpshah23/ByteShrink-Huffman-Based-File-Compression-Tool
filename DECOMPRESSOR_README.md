# ByteShrink Huffman Decompressor

This is the decompressor tool for the ByteShrink Huffman-based file compression system. It can decompress `.bshk` files created by the ByteShrink compressor.

## Features

- **Lossless Decompression**: Perfectly restores original files from compressed `.bshk` files
- **Huffman Tree Reconstruction**: Rebuilds the Huffman tree from stored frequency data
- **Binary Format Support**: Handles binary files correctly
- **Error Handling**: Comprehensive error checking and reporting

## Building

### Using Makefile (Recommended)
```bash
make decompressor    # Build only the decompressor
make all            # Build both compressor and decompressor
make test           # Build and run tests
```

### Manual Compilation
```bash
g++ -std=c++17 -I include -o decompressor main/decompressor_main.cpp src/HuffmanDecompressor.cpp src/HuffmanTree.cpp
```

## Usage

```bash
./decompressor <input.bshk> <output.txt>
```

### Examples

```bash
# Decompress a file
./decompressor data/output/sample.bshk data/decompressed/sample.txt

# Decompress with different output name
./decompressor compressed_file.bshk restored_file.txt
```

## File Format

The `.bshk` file format contains:
1. **Frequency Table Size**: Number of unique characters
2. **Frequency Table**: Character-frequency pairs
3. **Original Data Size**: Size of the uncompressed data
4. **Encoded Data Length**: Number of bits in the encoded data
5. **Encoded Data**: Huffman-encoded binary data

## Error Handling

The decompressor will report errors for:
- Missing or inaccessible input files
- Corrupted `.bshk` files
- Invalid file format
- Output file creation failures

## Testing

Run the complete compression and decompression test:
```bash
make test
```

This will:
1. Compress `data/input/sample.txt` to `data/output/sample.bshk`
2. Decompress the file back to `data/decompressed/sample.txt`
3. Verify that the original and decompressed files are identical

## Technical Details

- **Algorithm**: Huffman coding with deterministic tree building
- **Tree Building**: Uses character value as tiebreaker for equal frequencies
- **Memory Management**: Automatic cleanup of Huffman tree nodes
- **Bit Handling**: Proper bit-level reading and writing
- **Cross-Platform**: Works on Linux, macOS, and Windows

## Dependencies

- C++17 compatible compiler (g++, clang++, MSVC)
- Standard C++ library
- No external dependencies required
