#pragma once
#include <iostream>
#include <string>
using namespace std;
const int INF = 1000000000;
struct Customer { 
	int id; 
	string name; 
	string phone; 
	string address; 
	Customer* next; };
struct Parcel { 
	int tracking; 
	int customerId; 
	string source; 
	string destination; 
	double weight; 
	int priority; 
	string status; 
	int arrival; 
	bool urgent; };
string readLine(const string& prompt);
int readInt(const string& prompt);
double readDouble(const string& prompt);
void printParcel(const Parcel* p);
class CustomerList {
	Customer* head;
	int count;
public:
	CustomerList();
	~CustomerList();
	bool add(int id, const string& name, const string& phone, const string& address);
	Customer* find(int id)const;
	void display()const;
	int size()const;
};
struct ParcelNode { 
	Parcel* data; 
	ParcelNode* next; };
class ParcelQueue {
	ParcelNode* front;
	ParcelNode* rear;
	int count;
public:
	ParcelQueue();
	~ParcelQueue();
	void enqueue(Parcel* p);
	void pushFront(Parcel* p);
	Parcel* dequeue();
	bool empty()const;
	int size()const;
	void display()const;
};
class ParcelHeap {
	Parcel** data;
	int capacity;
	int count;
	bool higher(Parcel* a, Parcel* b)const;
	void siftUp(int i);
	void siftDown(int i);
	void grow();
public:
	ParcelHeap();
	~ParcelHeap();
	void push(Parcel* p);
	Parcel* pop();
	bool empty()const;
	int size()const;
	void display()const;
};
class HashTable {
	Parcel** table;
	int capacity;
	int count;
	int hashKey(int key, int cap)const;
	void rehash();
public:
	HashTable();
	~HashTable();
	bool insert(Parcel* p);
	Parcel* find(int key, int& probes)const;
	int size()const;
	int cap()const;
};
struct HistoryNode { 
	int tracking; 
	string event; 
	HistoryNode* prev; 
	HistoryNode* next; };
class HistoryList {
	HistoryNode* head;
	HistoryNode* tail;
	int count;
public:
	HistoryList();
	~HistoryList();
	void add(int tracking, const string& event);
	void removeLast();
	void display()const;
	int size()const;
};
struct UndoAction { 
	Parcel* parcel; 
	string previousStatus; 
	int type; };
struct UndoNode { 
	UndoAction action; 
	UndoNode* next; };
class UndoStack {
	UndoNode* top;
public:
	UndoStack();
	~UndoStack();
	void push(const UndoAction& a);
	bool pop(UndoAction& a);
	bool empty()const;
};
struct IntNode { 
	int value; 
	IntNode* next; };
class IntQueue {
	IntNode* front;
	IntNode* rear;
public:
	IntQueue();
	~IntQueue();
	void enqueue(int v);
	int dequeue();
	bool empty()const;
};
class IntStack {
	IntNode* top;
public:
	IntStack();
	~IntStack();
	void push(int v);
	int pop();
	bool empty()const;
};
struct TreeNode { 
	int key; 
	Parcel* parcel; 
	TreeNode* left; 
	TreeNode* right; 
	int height; };
class BST {
	TreeNode* root;
	TreeNode* insertNode(TreeNode* n, int key, Parcel* p);
	void destroy(TreeNode* n);
	int heightOf(TreeNode* n)const;
	void inorder(TreeNode* n)const;
public:
	BST();
	~BST();
	void insert(int key, Parcel* p);
	Parcel* search(int key, int& steps)const;
	void display()const;
	int height()const;
};
class AVL {
	TreeNode* root;
	int h(TreeNode* n)const;
	int balance(TreeNode* n)const;
	TreeNode* rotateRight(TreeNode* y);
	TreeNode* rotateLeft(TreeNode* x);
	TreeNode* insertNode(TreeNode* n, int key, Parcel* p);
	void destroy(TreeNode* n);
	void inorder(TreeNode* n)const;
public:
	AVL();
	~AVL();
	void insert(int key, Parcel* p);
	Parcel* search(int key, int& steps)const;
	void display()const;
	int height()const;
};
struct EdgeNode { int to; int weight; EdgeNode* next; };
class Graph {
	string* names;
	EdgeNode** adj;
	int** matrix;
	int capacity;
	int count;
	int roads;
	void grow();
	void addEdgeList(int u, int v, int w);
public:
	Graph();
	~Graph();
	int indexOf(const string& name)const;
	bool addLocation(const string& name);
	bool addRoad(const string& a, const string& b, int w);
	int size()const;
	int roadCount()const;
	void displayList()const;
	void displayMatrix()const;
	void bfs(const string& start, const string& target)const;
	void dfs(const string& start, const string& target)const;
	void shortestPath(const string& src, const string& dst)const;
	void minimumSpanningTree()const;
};
class RouteWise {
	CustomerList customers;
	HashTable parcels;
	ParcelQueue normalQueue;
	ParcelHeap urgentHeap;
	HistoryList history;
	UndoStack undoStack;
	BST bst;
	AVL avl;
	Graph city;
	int arrivalCounter;
	int deliveredCount;
	string createParcel(int tracking, int cid, const string& src, const string& dst, double w, int pr);
	void printMenu()const;
	void registerCustomer();
	void registerParcel();
	void searchParcel();
	void addRequest(bool urgent);
	void processNext();
	void markDelivered();
	void undoLast();
	void displayPending()const;
	void displayParcels()const;
	void cityMenu();
	void bfsMenu()const;
	void dfsMenu()const;
	void shortestMenu()const;
	void compareTrees()const;
	void stats()const;
	void loadSample();
public:
	RouteWise();
	void run();
};



