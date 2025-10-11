# 🗜️ ByteShrink – Huffman-Based File Compression Tool

A **C++-based lossless file compressor and decompressor** built using **Huffman Coding**, designed to demonstrate strong **Object-Oriented Programming principles**, **binary file handling**, and **algorithmic efficiency**.

Developed as an advanced Systems Programming project at **Nirma University**.

---

## 🧠 Overview

**ByteShrink** efficiently compresses and decompresses any type of file (text, image, or binary) using **Huffman Coding**, one of the most widely used algorithms in data compression.  
It applies a frequency-based variable-length encoding technique to achieve significant size reduction while maintaining data integrity.

---

## ⚙️ Core Features

- ✅ **Lossless Compression** – Original data is fully restored after decompression  
- ⚙️ **Huffman Tree Generation** – Frequency-based binary tree implementation  
- 🧠 **OOP Architecture** – Encapsulation, abstraction, and polymorphism applied throughout  
- 💾 **Binary File Support** – Works with both text and non-text files  
- 🧩 **Custom File Format (.bshk)** – Stores encoded data and frequency map  
- 🧰 **Reusable Components** – Independent classes for compression, decompression, and bit-level I/O  
- 🚀 **CMake Build System** – Cross-platform compilation and modular linking  

---

## 🏗️ Project Architecture

ByteShrink/
├── src/
│   ├── main.cpp
│   ├── HuffmanNode.cpp
│   ├── HuffmanCompressor.cpp
│   ├── HuffmanDecompressor.cpp
│   ├── FrequencyTable.cpp
│   └── BitStream.cpp
│
├── include/
│   ├── HuffmanNode.h
│   ├── HuffmanCompressor.h
│   ├── HuffmanDecompressor.h
│   ├── FrequencyTable.h
│   └── BitStream.h
│
├── data/
│   ├── input/
│   └── output/
│
├── docs/
│   ├── architecture.png
│   └── demo.gif
│
├── CMakeLists.txt
├── .gitignore
├── README.md
└── LICENSE

---

## 🔍 Technical Workflow

1. **Frequency Analysis** – Calculate frequency of each unique byte in the input file.  
2. **Tree Construction** – Build a Huffman Tree using a priority queue.  
3. **Encoding Generation** – Assign shorter codes to more frequent symbols.  
4. **Bitstream Writing** – Write encoded bits to a binary file (`.bshk`).  
5. **Header Embedding** – Save frequency table for decompression.  
6. **Decoding** – Reconstruct the Huffman tree and decode the bitstream back to original content.

---

## 🧱 `.bshk` File Structure

+----------------------+
| Header (metadata)    | → contains frequency map
+----------------------+
| Encoded Bitstream    | → compressed binary data
+----------------------+

The header ensures **accurate decompression** by storing the frequency of each symbol, allowing the Huffman tree to be rebuilt identically.

---

## 🧰 Build & Run Instructions

### 1️⃣ Clone the Repository

```bash
git clone https://github.com/dpshah23/ByteShrink.git
cd ByteShrink
````

### 2️⃣ Build the Project

```bash
mkdir build && cd build
cmake ..
make
```

### 3️⃣ Run the Program

To compress:

```bash
./ByteShrink compress ../data/input/sample.txt ../data/output/sample.bshk
```

To decompress:

```bash
./ByteShrink decompress ../data/output/sample.bshk ../data/output/sample_out.txt
```

---

## 🧩 Sample Output

| Input File   | Input Size | Compressed File | Compressed Size | Reduction |
| ------------ | ---------- | --------------- | --------------- | --------- |
| `sample.txt` | 51 KB      | `sample.bshk`   | 29 KB           | 🔻 43%    |
| `image.bmp`  | 120 KB     | `image.bshk`    | 91 KB           | 🔻 24%    |

---

## 🧭 Future Enhancements

 🔹 Adaptive Huffman Coding
 🔹 Multi-threaded Compression
 🔹 Progress Visualization in CLI
 🔹 GUI Front-end (Qt / Electron)
 🔹 File Format Detection for Smart Encoding

---

## 🧑‍💻 Authors

| Name                      | Role                                                                | GitHub                                   |
| ------------------------- | ------------------------------------------------------------------- | ---------------------------------------- |
| **Deep Shah** | System Architecture, Compression Algorithm, Binary Encoding         | [@dpshah23](https://github.com/dpshah23) |
| **Nakshi Shah**                | Decompression Engine, Testing, Documentation & Project Coordination | [@nakshishah31](https://github.com/nakshishah31)                                  |

## 📸 Demo Preview

*(Add GIF or screenshots inside `/docs/demo.gif` and `/docs/architecture.png`)*

---

## 💡 Summary

ByteShrink demonstrates the intersection of **data structures, file handling, and object-oriented design**,
delivering a practical, high-performance implementation of **Huffman-based compression** using **modern C++ practices**.
