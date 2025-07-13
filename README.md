# FRIENDS - Console-Based Social Network in C

FRIENDS is a simple, command-line social network simulation program written in C. Users can connect with each other, manage friendships, explore mutual friends, and discover the shortest connection path between two people using Breadth-First Search (BFS).

## 🚀 Features

- Add new users to the network
- Establish or remove friendships
- View a user's list of friends
- Discover mutual friends between two users
- Find the shortest path (connection) between users
- Display the entire network graphically (username and their friends)

## 🛠️ Built With

- C Programming Language
- Standard Library (`stdio.h`, `stdlib.h`, `string.h`)

## 🧠 Concepts Used

- Graphs (users as nodes, friendships as undirected edges)
- Breadth-First Search (for shortest path)
- Linked lists (for dynamic friend relationships)
- Hashless, array-based user storage (up to 100 users)

## 📦 How to Compile

Use any C compiler (e.g., GCC):

```bash
gcc -o rubor rubor.c
