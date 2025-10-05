# Assignment 1 - DSA

## Approach

### Task 1 - Polynomial ADT
- Implemented a polynomial ADT using a linked list.
- Supported insertion, addition, multiplication, and derivative operations.
- Followed the order of terms by exponent (highest first).
- Handled zero coefficients and merging of same exponent terms.
- Used `virtual` destructor as required.

### Task 2 - Text Editor Simulation
- Implemented a simple text editor using a doubly linked list.
- Cursor can move left/right; insert/delete characters efficiently.
- Display shows cursor position using `|`.
- No STL list used; everything implemented manually.

### Task 3 - UNO Game Simulation
- Simulated a simplified UNO game for 2-4 players.
- Managed deck, shuffling (fixed seed), discard pile, player hands.
- Handled card matching by color/value and action cards (Skip, Reverse, Draw Two).
- Turn direction and player order handled.
- State displayed in required format.

## GitHub Link
[https://github.com/yourusername/assignment1_dsa](https://github.com/yourusername/assignment1_dsa)

## Challenges Faced
- Managing linked lists without STL for both Polynomial and TextEditor tasks.
- Correctly implementing cursor behavior for insert/delete in TextEditor.
- Handling turn order and action cards in UNO, including direction changes and draw effects.
- Ensuring reproducible shuffling using fixed seed.
