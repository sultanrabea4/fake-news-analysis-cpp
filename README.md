# Fake News Analysis with Custom Data Structures (C++)

A C++ console application that loads large real and fake news datasets (two CSV files, about 115 MB in total) and analyses them using a **hand-built dynamic array**, with no STL containers for the core data. It compares the performance of sorting algorithms on real data.

> Data Structures group project. This repository contains **my part: the dynamic array implementation** (`array.cpp`). Teammates built the linked-list version and handled data cleaning.

---

## Features
- **Custom dynamic array** with automatic resizing (`push_back`, `resize`)
- Load articles from CSV (title, content, category, date)
- **Sort and display** articles by publication year
- **Search** articles by year
- **Fake political news analysis**: monthly distribution in 2016 with a text-based bar chart
- **Word-frequency analysis** of government-related fake news using a custom **hash map** (separate chaining)
- **Sorting benchmark**: Bubble Sort vs Merge Sort timed with `std::chrono`

## Concepts Demonstrated
Dynamic memory management · custom hash table · Merge Sort, Bubble Sort, Quick Sort · time-complexity comparison (O(n²) vs O(n log n)) · CSV parsing · regex date extraction

## Build & Run
```bash
g++ -std=c++17 -O2 array.cpp -o array
./array
```
Put the cleaned dataset files in `data/`:
- `data/cleaned_true.csv`
- `data/cleaned_fake.csv`

The CSV files (true and fake news articles, cleaned of empty and malformed rows) are not included because of their size.

## Menu
```
1. Sort and display all articles by publication year
2. Display total number of articles
3. Search articles (by year)
4. Analyze fake political news by month in 2016
5. Compare sorting performance
6. Analyze most common words in fake government-related news
```

## Author
**Sultan Mohammed Rabea** · [LinkedIn](https://www.linkedin.com/in/sultan-rabea/) · [GitHub](https://github.com/sultanrabea4)
