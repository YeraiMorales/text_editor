# Text Editor Simulator

This repository contains a C++ solution for a technical challenge to simulate a text editor with an integrated clipboard manager. The project is developed using Object-Oriented Programming (OOP) to ensure a predictable state and maintainable code.

## Compilation and Execution

The project is designed to be easily compiled using GCC (g++). It does not require any external dependencies beyond the C++ Standard Library.

**1. Compile the source code:**
```bash
g++ editor.cpp -o editor
```

**2. Run via standard input (interactive mode):**
```bash
./editor
```
*(You can type commands manually and finish by typing `EXIT`).*

**3. Run using test files:**
```bash
./editor < tests/test1.txt
```

---

## Functional Overview

The `TextEditor` class serves as the core engine of this simulation. It manages a dynamic string of text, a precise cursor position, and a multi-level clipboard history. The editor follows a "state-heavy" logic where certain operations (like typing or pasting) change their behavior based on whether text is currently selected.

### Supported Operations

* **`TYPE <text>`**: Inserts the specified text at the current cursor position. If a selection is active, it is deleted first, and the new text is inserted in its place. The cursor then moves to the end of the inserted string.
* **`SELECT <start_index> <end_index>`**: Selects a range of characters (0-based, inclusive). The cursor is automatically relocated to the end of the selected area.
* **`MOVE_CURSOR <offset>`**: Moves the cursor position relative to its current spot (negative for left, positive for right). The cursor is "clamped" to ensure it never leaves the text boundaries. Performing this operation automatically clears any active selection.
* **`COPY`**: Takes the text within the current selection and pushes it onto the clipboard history. If nothing is selected, the operation is ignored.
* **`PASTE <steps_back>`**: Retrieves a string from the clipboard history. `PASTE 1` (or a simple `PASTE`) fetches the most recent copy, while `PASTE 2` fetches the one before that. If a selection exists, it is replaced by the pasted text.
* **`EXIT`**: Signals the end of the operation sequence and terminates the program.

---

## Important Design Decisions

### 1. Single-File Architecture
While traditional production environments separate declarations (`.h`) from implementations (`.cpp`), a single `editor.cpp` file was chosen due to the reduced scale of the project.

### 2. Encapsulation and Centralized State
All logic is encapsulated within the `TextEditor` class to isolate the document's state:
* **`text`**: The main string that is dynamically mutated.
* **`cursor_pos`**: The numerical position of the cursor, strictly protected by the `clampCursor` method to ensure it stays within document bounds.
* **`clipboard`**: A `std::vector<string>` acting as a history stack, allowing direct access by calculating `clipboard.size() - steps_back`.
* **Selection State**: Dedicated variables (`has_selection`, `select_start`, `select_end`) manage selected areas.

### 3. Logic Reuse
The requirements specify that if a non-empty selection exists, both `TYPE` and `PASTE` operations must delete the selected text, insert the new content in its place, and move the cursor to the end of the insertion. To avoid redundancy, the `paste()` method internally invokes `type()`, ensuring identical behavior and reducing potential bugs.

---

## Test Cases and Expected Results

Multiple `.txt` files have been designed to validate edge cases and the behaviour of the application.

| Test File | Test Objective | Expected Output |
| :--- | :--- | :--- |
| `test1.txt` | The basic example provided in the challenge. | `"We love pointers"`  |
| `test2.txt` | Extended example from the document. Tests clipboard history and multiple selections. | `"Cactus is located in Barcelona!"`  |
| `test3.txt` | Verifies cursor boundary containment and rejection of invalid `PASTE` steps. | `"Oh,Oh, Hola!"` |
| `test4.txt` | Validates strict deletion when performing `TYPE` or `PASTE` over active selections. | `"bcdef"` |
| `test5.txt` | Ensures `COPY` without a prior selection is ignored without polluting the clipboard. | `"test"` |

## Complexity Analysis

The following table outlines the **Time Complexity** for each operation. In the context of this simulation, $n$ represents the total length of the text, and $k$ represents the length of the string being inserted or copied.

| Operation | Time Complexity | Logic / Rationale |
| :--- | :--- | :--- |
| **`TYPE`** | $O(n)$ | In the worst case, `std::string::insert` and `std::string::erase` require shifting all subsequent characters in the string. |
| **`SELECT`** | $O(1)$ | Simply updates integer boundaries and a boolean flag. |
| **`MOVE_CURSOR`** | $O(1)$ | Performs a basic arithmetic operation and a boundary check (clamping). |
| **`COPY`** | $O(k)$ | Where $k$ is the length of the selected text; `std::string::substr` creates a new string copy. |
| **`PASTE`** | $O(n + m)$ | Where $n$ is the current text length and $m$ is the length of the clipboard content. It reuses the `TYPE` logic. |

---