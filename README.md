# 📖 Slang Dictionary CLI with Trie (C Language)

A command-line based dictionary application to **store**, **search**, and **manage slang words** using the Trie data structure. This project was built in C as a practice of data structure implementation and text-based UI interaction.

---

## 🚀 Features

- 🔤 Add (Release) new slang words with descriptions
- 🔍 Search for slang words by exact match
- 📚 List all slang words by a given **prefix**
- 📋 Display all stored slang words
- ✅ Overwrite existing slang word description if re-added

---

## 🧱 Data Structure Used

- **Trie (Prefix Tree)**:
  - Each node represents a character
  - `is_end_of_word` flag to mark full words
  - Stores `description` on word-end node
- **Custom Structs** with dynamic memory allocation

---

## 💡 How It Works

User interacts with a simple menu interface to perform the following:

```text
1. Release a new slang word
2. Search a slang word
3. View all slang words starting with a certain prefix
4. View all slang words
5. Exit
