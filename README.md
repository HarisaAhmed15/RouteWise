# RouteWise – Smart Delivery and Route Optimization System

Project Team
Harisa Ahmed
Zaib-Un-Nisa
Maneha Faisal

## 1. Project Overview
RouteWise is a C++-based delivery management system designed to manage customers, parcels, delivery priorities, and routes between locations. It uses different data structures and algorithms to organize deliveries efficiently and find optimal routes.

## 2. Major Features
- Register and manage customers.
- Add, search, and track parcels.
- Manage normal deliveries using a Queue and urgent deliveries using a Max Heap.
- Maintain delivery history and undo previous actions.
- Search parcels using BST and AVL Tree.
- Represent locations and roads using a Graph.
- Find reachable locations using BFS and DFS.
- Calculate shortest routes using Dijkstra's algorithm.
- Find a Minimum Spanning Tree using Prim's algorithm.

## 3. Data Structures Used

| Data Structure | Purpose |
|---|---|
| Singly Linked List | Store customer records |
| Queue | Manage normal deliveries (FIFO) |
| Max Heap | Prioritize urgent deliveries |
| Hash Table | Search parcels efficiently |
| Doubly Linked List | Store delivery history |
| Stack | Undo previous actions |
| BST | Organize and search parcel records |
| AVL Tree | Maintain a balanced search tree |
| Graph | Represent locations and roads |
| Adjacency List and Matrix | Store graph connections |

## 4. Algorithms Used
- **BFS:** Traverses reachable locations level by level.
- **DFS:** Explores connected locations deeply.
- **Dijkstra's Algorithm:** Finds the shortest route between two locations.
- **Prim's Algorithm:** Finds a Minimum Spanning Tree.
- **BST and AVL Operations:** Support insertion, searching, and traversal.

## 5. How to Build and Run

### Requirements
- C++ compiler (G++ recommended).
- VS Code or another C++ IDE.

### Compilation
Keep `main.cpp`, `routewise.h`, and `routewise.cpp` in the same folder.

Run this command in the terminal:

```bash
g++ -std=c++11 main.cpp routewise.cpp -o RouteWise
```

### Execution

On Windows:

```bash
RouteWise.exe
```

On Linux:

```bash
./RouteWise
```

Follow the menu displayed by the program to use its features.

## 6. Time Complexity Analysis

Let:
- `n` = number of records
- `V` = number of graph vertices
- `E` = number of graph edges
- `h` = tree height

| Operation | Time Complexity |
|---|---|
| Customer insertion/search | O(n) |
| Queue insertion/removal | O(1) |
| Heap insertion/removal | O(log n) |
| Hash Table search/insertion | Average O(1), worst O(n) |
| Stack push/pop | O(1) |
| History insertion | O(1) |
| BST insertion/search | O(h) |
| AVL insertion/search | O(log n) |
| BFS and DFS | O(V + E) |
| Dijkstra (matrix implementation) | O(V²) |
| Prim (matrix implementation) | O(V²) |

## 7. Limitations
- The system is console-based.
- Data is stored in memory and is not permanently saved.
- Routes are based on the locations and distances entered into the system.

## 8. Conclusion
RouteWise demonstrates how data structures and algorithms can be combined to build a delivery management system. It provides parcel management, delivery prioritization, history tracking, and route optimization through graph algorithms.
