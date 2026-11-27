#include "routewise.h"
static int maxOf(int a, int b) { return a > b ? a : b; }
string readLine(const string& prompt) {
	string s;
	cout << prompt;
	if (!getline(cin, s))return "";
	return s;
}
int readInt(const string& prompt) {
	while (true) {
		string s = readLine(prompt);
		if (cin.fail())return 0;
		int i = 0;
		bool neg = false;
		if (!s.empty() && s[0] == '-') { neg = true; i = 1; }
		if (i >= (int)s.size() || (int)s.size() - i > 9) { cout << "Invalid number\n"; 
		continue; 
		}
		long v = 0;
		bool ok = true;
		for (; i < (int)s.size(); i++) {
			if (s[i] < '0' || s[i]>'9') { ok = false; break; }
			v = v * 10 + (s[i] - '0');
		}
		if (!ok) { cout << "Invalid number\n"; continue; }
		return neg ? -(int)v : (int)v;
	}
}
double readDouble(const string& prompt) {
	while (true) {
		string s = readLine(prompt);
		if (cin.fail())return 0;
		double v = 0, f = 0.1;
		bool dot = false, ok = !s.empty();
		int digits = 0;
		for (size_t i = 0; i < s.size() && ok; i++) {
			if (s[i] == '.' && !dot)dot = true;
			else if (s[i] >= '0' && s[i] <= '9') {
				digits++;
				if (!dot)v = v * 10 + (s[i] - '0');
				else { v += (s[i] - '0') * f; f /= 10; }
			}
			else ok = false;
		}
		if (ok && digits > 0)return v;
		cout << "Invalid number\n";
	}
}
void printParcel(const Parcel* p) {
	cout << "Tracking: " << p->tracking << " | Customer: " << p->customerId << " | " << p->source << " -> " << p->destination << " | Weight: " << p->weight << " kg | Priority: " << p->priority << " | Status: " << p->status << "\n";
}
CustomerList::CustomerList() :head(nullptr), count(0) {}
CustomerList::~CustomerList() {
	while (head) {
		Customer* t = head;
		head = head->next;
		delete t;
	}
}
bool CustomerList::add(int id, const string& name, const string& phone, const string& address) {
	if (find(id))return false;
	Customer* c = new Customer{ id,name,phone,address,nullptr };
	if (!head)head = c;
	else {
		Customer* t = head;
		while (t->next)t = t->next;
		t->next = c;
	}
	count++;
	return true;
}
Customer* CustomerList::find(int id)const {
	for (Customer* c = head; c; c = c->next)if (c->id == id)return c;
	return nullptr;
}
void CustomerList::display()const {
	if (!head) { cout << "(no customers)\n"; return; }
	for (Customer* c = head; c; c = c->next)cout << "ID: " << c->id << " | " << c->name << " | " << c->phone << " | " << c->address << "\n";
}
int CustomerList::size()const { return count; }
ParcelQueue::ParcelQueue() :front(nullptr), rear(nullptr), count(0) {}
ParcelQueue::~ParcelQueue() {
	while (front) {
		ParcelNode* t = front;
		front = front->next;
		delete t;
	}
}
void ParcelQueue::enqueue(Parcel* p) {
	ParcelNode* n = new ParcelNode{ p,nullptr };
	if (rear)rear->next = n;
	else front = n;
	rear = n;
	count++;
}
void ParcelQueue::pushFront(Parcel* p) {
	ParcelNode* n = new ParcelNode{ p,front };
	front = n;
	if (!rear)rear = n;
	count++;
}
Parcel* ParcelQueue::dequeue() {
	if (!front)return nullptr;
	ParcelNode* t = front;
	Parcel* p = t->data;
	front = front->next;
	if (!front)rear = nullptr;
	delete t;
	count--;
	return p;
}
bool ParcelQueue::empty()const { return front == nullptr; }
int ParcelQueue::size()const { return count; }
void ParcelQueue::display()const {
	if (!front) { cout << "(none)\n"; return; }
	for (ParcelNode* n = front; n; n = n->next)printParcel(n->data);
}
ParcelHeap::ParcelHeap() :capacity(16), count(0) { data = new Parcel * [capacity]; }
ParcelHeap::~ParcelHeap() { delete[]data; }
bool ParcelHeap::higher(Parcel* a, Parcel* b)const {
	if (a->priority != b->priority)return a->priority > b->priority;
	return a->arrival < b->arrival;
}
void ParcelHeap::siftUp(int i) {
	while (i > 0) {
		int parent = (i - 1) / 2;
		if (higher(data[i], data[parent])) {
			Parcel* t = data[i];
			data[i] = data[parent];
			data[parent] = t;
			i = parent;
		}
		else break;
	}
}
void ParcelHeap::siftDown(int i) {
	while (true) {
		int l = 2 * i + 1, r = l + 1, best = i;
		if (l < count && higher(data[l], data[best]))best = l;
		if (r < count && higher(data[r], data[best]))best = r;
		if (best == i)break;
		Parcel* t = data[i];
		data[i] = data[best];
		data[best] = t;
		i = best;
	}
}
void ParcelHeap::grow() {
	Parcel** nd = new Parcel * [capacity * 2];
	for (int i = 0; i < count; i++)nd[i] = data[i];
	delete[]data;
	data = nd;
	capacity *= 2;
}
void ParcelHeap::push(Parcel* p) {
	if (count == capacity)grow();
	data[count] = p;
	siftUp(count);
	count++;
}
Parcel* ParcelHeap::pop() {
	if (count == 0)return nullptr;
	Parcel* top = data[0];
	count--;
	data[0] = data[count];
	if (count > 0)siftDown(0);
	return top;
}
bool ParcelHeap::empty()const { return count == 0; }
int ParcelHeap::size()const { return count; }
void ParcelHeap::display()const {
	if (count == 0) { cout << "(none)\n"; return; }
	ParcelHeap tmp;
	for (int i = 0; i < count; i++)tmp.push(data[i]);
	while (!tmp.empty())printParcel(tmp.pop());
}
HashTable::HashTable() :capacity(11), count(0) {
	table = new Parcel * [capacity];
	for (int i = 0; i < capacity; i++)table[i] = nullptr;
}
HashTable::~HashTable() {
	for (int i = 0; i < capacity; i++)delete table[i];
	delete[]table;
}
int HashTable::hashKey(int key, int cap)const {
	int k = key < 0 ? -key : key;
	return k % cap;
}
void HashTable::rehash() {
	int newCap = capacity * 2 + 1;
	Parcel** nt = new Parcel * [newCap];
	for (int i = 0; i < newCap; i++)nt[i] = nullptr;
	for (int i = 0; i < capacity; i++) {
		if (table[i]) {
			int idx = hashKey(table[i]->tracking, newCap);
			while (nt[idx])idx = (idx + 1) % newCap;
			nt[idx] = table[i];
		}
	}
	delete[]table;
	table = nt;
	capacity = newCap;
}
bool HashTable::insert(Parcel* p) {
	int probes;
	if (find(p->tracking, probes))return false;
	if ((count + 1) * 10 > capacity * 7)rehash();
	int idx = hashKey(p->tracking, capacity);
	while (table[idx])idx = (idx + 1) % capacity;
	table[idx] = p;
	count++;
	return true;
}
Parcel* HashTable::find(int key, int& probes)const {
	probes = 0;
	int idx = hashKey(key, capacity);
	for (int i = 0; i < capacity; i++) {
		probes++;
		if (!table[idx])return nullptr;
		if (table[idx]->tracking == key)return table[idx];
		idx = (idx + 1) % capacity;
	}
	return nullptr;
}
int HashTable::size()const { return count; }
int HashTable::cap()const { return capacity; }
HistoryList::HistoryList() :head(nullptr), tail(nullptr), count(0) {}
HistoryList::~HistoryList() {
	while (head) {
		HistoryNode* t = head;
		head = head->next;
		delete t;
	}
}
void HistoryList::add(int tracking, const string& event) {
	HistoryNode* n = new HistoryNode{ tracking,event,tail,nullptr };
	if (tail)tail->next = n;
	else head = n;
	tail = n;
	count++;
}
void HistoryList::removeLast() {
	if (!tail)return;
	HistoryNode* t = tail;
	tail = tail->prev;
	if (tail)tail->next = nullptr;
	else head = nullptr;
	delete t;
	count--;
}
void HistoryList::display()const {
	if (!head) { cout << "(no history)\n"; return; }
	int i = 1;
	for (HistoryNode* n = head; n; n = n->next)cout << i++ << ". Parcel " << n->tracking << " - " << n->event << "\n";
}
int HistoryList::size()const { return count; }
UndoStack::UndoStack() :top(nullptr) {}
UndoStack::~UndoStack() {
	while (top) {
		UndoNode* t = top;
		top = top->next;
		delete t;
	}
}
void UndoStack::push(const UndoAction& a) {
	top = new UndoNode{ a,top };
}
bool UndoStack::pop(UndoAction& a) {
	if (!top)return false;
	UndoNode* t = top;
	a = t->action;
	top = top->next;
	delete t;
	return true;
}
bool UndoStack::empty()const { return top == nullptr; }
IntQueue::IntQueue() :front(nullptr), rear(nullptr) {}
IntQueue::~IntQueue() {
	while (front) {
		IntNode* t = front;
		front = front->next;
		delete t;
	}
}
void IntQueue::enqueue(int v) {
	IntNode* n = new IntNode{ v,nullptr };
	if (rear)rear->next = n;
	else front = n;
	rear = n;
}
int IntQueue::dequeue() {
	IntNode* t = front;
	int v = t->value;
	front = front->next;
	if (!front)rear = nullptr;
	delete t;
	return v;
}
bool IntQueue::empty()const { return front == nullptr; }
IntStack::IntStack() :top(nullptr) {}
IntStack::~IntStack() {
	while (top) {
		IntNode* t = top;
		top = top->next;
		delete t;
	}
}
void IntStack::push(int v) { top = new IntNode{ v,top }; }
int IntStack::pop() {
	IntNode* t = top;
	int v = t->value;
	top = top->next;
	delete t;
	return v;
}
bool IntStack::empty()const { return top == nullptr; }
BST::BST() :root(nullptr) {}
BST::~BST() { destroy(root); }
void BST::destroy(TreeNode* n) {
	if (!n)return;
	destroy(n->left);
	destroy(n->right);
	delete n;
}
TreeNode* BST::insertNode(TreeNode* n, int key, Parcel* p) {
	if (!n)return new TreeNode{ key,p,nullptr,nullptr,1 };
	if (key < n->key)n->left = insertNode(n->left, key, p);
	else if (key > n->key)n->right = insertNode(n->right, key, p);
	return n;
}
void BST::insert(int key, Parcel* p) { root = insertNode(root, key, p); }
Parcel* BST::search(int key, int& steps)const {
	steps = 0;
	TreeNode* cur = root;
	while (cur) {
		steps++;
		if (key == cur->key)return cur->parcel;
		cur = key < cur->key ? cur->left : cur->right;
	}
	return nullptr;
}
int BST::heightOf(TreeNode* n)const {
	if (!n)return 0;
	return 1 + maxOf(heightOf(n->left), heightOf(n->right));
}
void BST::inorder(TreeNode* n)const {
	if (!n)return;
	inorder(n->left);
	if (n->parcel)printParcel(n->parcel);
	inorder(n->right);
}
void BST::display()const { inorder(root); }
int BST::height()const { return heightOf(root); }
AVL::AVL() :root(nullptr) {}
AVL::~AVL() { destroy(root); }
void AVL::destroy(TreeNode* n) {
	if (!n)return;
	destroy(n->left);
	destroy(n->right);
	delete n;
}
int AVL::h(TreeNode* n)const { return n ? n->height : 0; }
int AVL::balance(TreeNode* n)const { return n ? h(n->left) - h(n->right) : 0; }
TreeNode* AVL::rotateRight(TreeNode* y) {
	TreeNode* x = y->left;
	TreeNode* t = x->right;
	x->right = y;
	y->left = t;
	y->height = 1 + maxOf(h(y->left), h(y->right));
	x->height = 1 + maxOf(h(x->left), h(x->right));
	return x;
}
TreeNode* AVL::rotateLeft(TreeNode* x) {
	TreeNode* y = x->right;
	TreeNode* t = y->left;
	y->left = x;
	x->right = t;
	x->height = 1 + maxOf(h(x->left), h(x->right));
	y->height = 1 + maxOf(h(y->left), h(y->right));
	return y;
}
TreeNode* AVL::insertNode(TreeNode* n, int key, Parcel* p) {
	if (!n)return new TreeNode{ key,p,nullptr,nullptr,1 };
	if (key < n->key)n->left = insertNode(n->left, key, p);
	else if (key > n->key)n->right = insertNode(n->right, key, p);
	else return n;
	n->height = 1 + maxOf(h(n->left), h(n->right));
	int b = balance(n);
	if (b > 1 && key < n->left->key)return rotateRight(n);
	if (b<-1 && key>n->right->key)return rotateLeft(n);
	if (b > 1 && key > n->left->key) {
		n->left = rotateLeft(n->left);
		return rotateRight(n);
	}
	if (b < -1 && key < n->right->key) {
		n->right = rotateRight(n->right);
		return rotateLeft(n);
	}
	return n;
}
void AVL::insert(int key, Parcel* p) { root = insertNode(root, key, p); }
Parcel* AVL::search(int key, int& steps)const {
	steps = 0;
	TreeNode* cur = root;
	while (cur) {
		steps++;
		if (key == cur->key)return cur->parcel;
		cur = key < cur->key ? cur->left : cur->right;
	}
	return nullptr;
}
void AVL::inorder(TreeNode* n)const {
	if (!n)return;
	inorder(n->left);
	if (n->parcel)printParcel(n->parcel);
	inorder(n->right);
}
void AVL::display()const { inorder(root); }
int AVL::height()const { return h(root); }
Graph::Graph() :capacity(8), count(0), roads(0) {
	names = new string[capacity];
	adj = new EdgeNode * [capacity];
	matrix = new int* [capacity];
	for (int i = 0; i < capacity; i++) {
		adj[i] = nullptr;
		matrix[i] = new int[capacity]();
	}
}
Graph::~Graph() {
	for (int i = 0; i < count; i++) {
		EdgeNode* e = adj[i];
		while (e) {
			EdgeNode* t = e;
			e = e->next;
			delete t;
		}
	}
	for (int i = 0; i < capacity; i++)delete[]matrix[i];
	delete[]matrix;
	delete[]adj;
	delete[]names;
}
void Graph::grow() {
	int nc = capacity * 2;
	string* nn = new string[nc];
	EdgeNode** na = new EdgeNode * [nc];
	int** nm = new int* [nc];
	for (int i = 0; i < nc; i++) {
		nm[i] = new int[nc]();
		na[i] = nullptr;
		if (i < capacity) {
			nn[i] = names[i];
			na[i] = adj[i];
			for (int j = 0; j < capacity; j++)nm[i][j] = matrix[i][j];
		}
	}
	for (int i = 0; i < capacity; i++)delete[]matrix[i];
	delete[]matrix;
	delete[]adj;
	delete[]names;
	names = nn;
	adj = na;
	matrix = nm;
	capacity = nc;
}
void Graph::addEdgeList(int u, int v, int w) {
	EdgeNode* e = new EdgeNode{ v,w,nullptr };
	if (!adj[u])adj[u] = e;
	else {
		EdgeNode* t = adj[u];
		while (t->next)t = t->next;
		t->next = e;
	}
}
int Graph::indexOf(const string& name)const {
	for (int i = 0; i < count; i++)if (names[i] == name)return i;
	return -1;
}
bool Graph::addLocation(const string& name) {
	if (name.empty() || indexOf(name) >= 0)return false;
	if (count == capacity)grow();
	names[count] = name;
	count++;
	return true;
}
bool Graph::addRoad(const string& a, const string& b, int w) {
	int u = indexOf(a), v = indexOf(b);
	if (u < 0 || v < 0 || u == v || w <= 0 || matrix[u][v] != 0)return false;
	matrix[u][v] = w;
	matrix[v][u] = w;
	addEdgeList(u, v, w);
	addEdgeList(v, u, w);
	roads++;
	return true;
}
int Graph::size()const { return count; }
int Graph::roadCount()const { return roads; }
void Graph::displayList()const {
	if (count == 0) { cout << "(network is empty)\n"; return; }
	cout << "Adjacency List:\n";
	for (int i = 0; i < count; i++) {
		cout << names[i] << " ->";
		if (!adj[i])cout << " (no roads)";
		for (EdgeNode* e = adj[i]; e; e = e->next)cout << " " << names[e->to] << "(" << e->weight << ")";
		cout << "\n";
	}
}
void Graph::displayMatrix()const {
	if (count == 0)return;
	cout << "Adjacency Matrix:\n";
	for (int i = 0; i < count; i++)cout << "[" << i << "] " << names[i] << "\n";
	cout << "\t";
	for (int j = 0; j < count; j++)cout << "[" << j << "]\t";
	cout << "\n";
	for (int i = 0; i < count; i++) {
		cout << "[" << i << "]\t";
		for (int j = 0; j < count; j++)cout << matrix[i][j] << "\t";
		cout << "\n";
	}
}
void Graph::bfs(const string& start, const string& target)const {
	int s = indexOf(start);
	if (s < 0) { cout << "Location not found\n"; return; }
	int t = indexOf(target);
	bool* vis = new bool[count]();
	bool found = (s == t);
	bool first = true;
	IntQueue q;
	q.enqueue(s);
	vis[s] = true;
	cout << "BFS order: ";
	while (!q.empty()) {
		int u = q.dequeue();
		cout << (first ? "" : " -> ") << names[u];
		first = false;
		if (u == t)found = true;
		for (EdgeNode* e = adj[u]; e; e = e->next) {
			if (!vis[e->to]) {
				vis[e->to] = true;
				q.enqueue(e->to);
			}
		}
	}
	cout << "\n";
	if (t >= 0)cout << target << (found ? " is reachable from " : " is NOT reachable from ") << start << "\n";
	else if (!target.empty())cout << "Destination not found\n";
	delete[]vis;
}
void Graph::dfs(const string& start, const string& target)const {
	int s = indexOf(start);
	if (s < 0) { cout << "Location not found\n"; return; }
	int t = indexOf(target);
	bool* vis = new bool[count]();
	bool found = (s == t);
	bool first = true;
	IntStack st;
	st.push(s);
	cout << "DFS order: ";
	while (!st.empty()) {
		int u = st.pop();
		if (vis[u])continue;
		vis[u] = true;
		cout << (first ? "" : " -> ") << names[u];
		first = false;
		if (u == t)found = true;
		IntStack tmp;
		for (EdgeNode* e = adj[u]; e; e = e->next)if (!vis[e->to])tmp.push(e->to);
		while (!tmp.empty())st.push(tmp.pop());
	}
	cout << "\n";
	if (t >= 0)cout << target << (found ? " is reachable from " : " is NOT reachable from ") << start << "\n";
	else if (!target.empty())cout << "Destination not found\n";
	delete[]vis;
}
void Graph::shortestPath(const string& src, const string& dst)const {
	int s = indexOf(src), t = indexOf(dst);
	if (s < 0 || t < 0) { cout << "Invalid location\n"; return; }
	int* dist = new int[count];
	int* par = new int[count];
	bool* done = new bool[count];
	for (int i = 0; i < count; i++) {
		dist[i] = INF;
		par[i] = -1;
		done[i] = false;
	}
	dist[s] = 0;
	for (int k = 0; k < count; k++) {
		int u = -1;
		for (int i = 0; i < count; i++)if (!done[i] && dist[i] < INF && (u < 0 || dist[i] < dist[u]))u = i;
		if (u < 0)break;
		done[u] = true;
		for (int v = 0; v < count; v++) {
			if (matrix[u][v] > 0 && !done[v] && dist[u] + matrix[u][v] < dist[v]) {
				dist[v] = dist[u] + matrix[u][v];
				par[v] = u;
			}
		}
	}
	if (dist[t] >= INF)cout << "No route exists between " << src << " and " << dst << "\n";
	else {
		IntStack st;
		for (int v = t; v != -1; v = par[v])st.push(v);
		bool first = true;
		cout << "Source: " << src << "\nDestination: " << dst << "\nRecommended Route:\n";
		while (!st.empty()) {
			cout << (first ? "" : " -> ") << names[st.pop()];
			first = false;
		}
		cout << "\nTotal Distance: " << dist[t] << " km\n";
	}
	delete[]dist;
	delete[]par;
	delete[]done;
}
void Graph::minimumSpanningTree()const {
	if (count == 0) { cout << "Network is empty\n"; return; }
	int* key = new int[count];
	int* par = new int[count];
	bool* used = new bool[count];
	for (int i = 0; i < count; i++) {
		key[i] = INF;
		par[i] = -1;
		used[i] = false;
	}
	key[0] = 0;
	int total = 0, added = 0;
	cout << "Selected connections:\n";
	for (int k = 0; k < count; k++) {
		int u = -1;
		for (int i = 0; i < count; i++)if (!used[i] && key[i] < INF && (u < 0 || key[i] < key[u]))u = i;
		if (u < 0)break;
		used[u] = true;
		added++;
		if (par[u] != -1) {
			cout << names[par[u]] << " -- " << names[u] << " (" << key[u] << " km)\n";
			total += key[u];
		}
		for (int v = 0; v < count; v++) {
			if (matrix[u][v] > 0 && !used[v] && matrix[u][v] < key[v]) {
				key[v] = matrix[u][v];
				par[v] = u;
			}
		}
	}
	cout << "Total network cost: " << total << " km\n";
	if (added < count)cout << "Warning: network is not fully connected, only " << added << " of " << count << " locations are linked from " << names[0] << "\n";
	delete[]key;
	delete[]par;
	delete[]used;
}
RouteWise::RouteWise() :arrivalCounter(0), deliveredCount(0) {}
string RouteWise::createParcel(int tracking, int cid, const string& src, const string& dst, double w, int pr) {
	if (!customers.find(cid))return "Customer not found";
	if (city.indexOf(src) < 0 || city.indexOf(dst) < 0)return "Source or destination is not in the city network";
	if (pr < 1 || pr>10)return "Priority must be between 1 and 10";
	if (w <= 0)return "Weight must be positive";
	int probes;
	if (parcels.find(tracking, probes))return "Tracking ID already exists";
	Parcel* p = new Parcel{ tracking,cid,src,dst,w,pr,"Registered",0,false };
	parcels.insert(p);
	bst.insert(tracking, p);
	avl.insert(tracking, p);
	return "";
}
void RouteWise::printMenu()const {
	cout << "\n=========================================\n";
	cout << "ROUTEWISE DELIVERY SYSTEM\n";
	cout << "=========================================\n";
	cout << "1. Register Customer\n";
	cout << "2. Register Parcel\n";
	cout << "3. Search Parcel\n";
	cout << "4. Add Normal Delivery Request\n";
	cout << "5. Add Urgent Delivery Request\n";
	cout << "6. Process Next Delivery\n";
	cout << "7. Mark Parcel as Delivered\n";
	cout << "8. Undo Last Operation\n";
	cout << "9. Display Pending Deliveries\n";
	cout << "10. Display Parcel Records\n";
	cout << "11. Display City Network\n";
	cout << "12. BFS Network Traversal\n";
	cout << "13. DFS Network Traversal\n";
	cout << "14. Find Shortest Delivery Route\n";
	cout << "15. Generate Minimum Delivery Network\n";
	cout << "16. Compare BST and AVL Performance\n";
	cout << "17. Display System Statistics\n";
	cout << "18. Exit\n";
}
void RouteWise::registerCustomer() {
	int id = readInt("Customer ID: ");
	string name = readLine("Name: ");
	string phone = readLine("Phone: ");
	string address = readLine("Address: ");
	if (customers.add(id, name, phone, address))cout << "Customer registered\n";
	else cout << "Customer ID already exists\n";
}
void RouteWise::registerParcel() {
	int tracking = readInt("Tracking ID: ");
	int cid = readInt("Customer ID: ");
	string src = readLine("Source: ");
	string dst = readLine("Destination: ");
	double w = readDouble("Weight (kg): ");
	int pr = readInt("Priority (1-10): ");
	string err = createParcel(tracking, cid, src, dst, w, pr);
	if (err.empty())cout << "Parcel registered\n";
	else cout << err << "\n";
}
void RouteWise::searchParcel() {
	int t = readInt("Tracking ID: ");
	int probes = 0, s1 = 0, s2 = 0;
	Parcel* p = parcels.find(t, probes);
	if (!p) { cout << "Parcel not found (hash probes: " << probes << ")\n"; return; }
	printParcel(p);
	Customer* c = customers.find(p->customerId);
	if (c)cout << "Customer: " << c->name << " | " << c->phone << " | " << c->address << "\n";
	bst.search(t, s1);
	avl.search(t, s2);
	cout << "Hash probes: " << probes << " | BST steps: " << s1 << " | AVL steps: " << s2 << "\n";
}
void RouteWise::addRequest(bool urgent) {
	int t = readInt("Tracking ID: ");
	int probes;
	Parcel* p = parcels.find(t, probes);
	if (!p) { cout << "Parcel not found\n"; return; }
	if (p->status != "Registered") { cout << "Parcel already requested (status: " << p->status << ")\n"; return; }
	p->arrival = ++arrivalCounter;
	p->urgent = urgent;
	p->status = "Pending";
	if (urgent)urgentHeap.push(p);
	else normalQueue.enqueue(p);
	cout << (urgent ? "Urgent" : "Normal") << " delivery request added\n";
}
void RouteWise::processNext() {
	Parcel* p = nullptr;
	if (!urgentHeap.empty())p = urgentHeap.pop();
	else if (!normalQueue.empty())p = normalQueue.dequeue();
	if (!p) { cout << "No pending deliveries\n"; return; }
	UndoAction a{ p,p->status,1 };
	undoStack.push(a);
	p->status = "Processing";
	history.add(p->tracking, "Processed");
	cout << "Processing " << (p->urgent ? "urgent" : "normal") << " delivery:\n";
	printParcel(p);
}
void RouteWise::markDelivered() {
	int t = readInt("Tracking ID: ");
	int probes;
	Parcel* p = parcels.find(t, probes);
	if (!p) { cout << "Parcel not found\n"; return; }
	if (p->status != "Processing") { cout << "Parcel must be in Processing status (current: " << p->status << ")\n"; return; }
	UndoAction a{ p,p->status,2 };
	undoStack.push(a);
	p->status = "Delivered";
	deliveredCount++;
	history.add(p->tracking, "Delivered");
	cout << "Parcel marked as delivered\n";
}
void RouteWise::undoLast() {
	UndoAction a;
	if (!undoStack.pop(a)) { cout << "Nothing to undo\n"; return; }
	a.parcel->status = a.previousStatus;
	history.removeLast();
	if (a.type == 1) {
		if (a.parcel->urgent)urgentHeap.push(a.parcel);
		else normalQueue.pushFront(a.parcel);
	}
	else deliveredCount--;
	cout << "Undo successful. Parcel " << a.parcel->tracking << " restored to status " << a.previousStatus << "\n";
}
void RouteWise::displayPending()const {
	cout << "Urgent deliveries (highest priority first):\n";
	urgentHeap.display();
	cout << "Normal deliveries (arrival order):\n";
	normalQueue.display();
	cout << "Delivery history:\n";
	history.display();
}
void RouteWise::displayParcels()const {
	if (parcels.size() == 0) { cout << "(no parcels)\n"; return; }
	avl.display();
}
void RouteWise::cityMenu() {
	cout << "1. Display network\n2. Add location\n3. Add road\n4. Back\n";
	int c = readInt("Enter choice: ");
	if (c == 1) {
		city.displayList();
		city.displayMatrix();
	}
	else if (c == 2) {
		string n = readLine("Location name: ");
		if (city.addLocation(n))cout << "Location added\n";
		else cout << "Invalid or duplicate location\n";
	}
	else if (c == 3) {
		string a = readLine("First location: ");
		string b = readLine("Second location: ");
		int w = readInt("Distance (km): ");
		if (city.addRoad(a, b, w))cout << "Road added\n";
		else cout << "Could not add road\n";
	}
}
void RouteWise::bfsMenu()const {
	string s = readLine("Start location: ");
	string t = readLine("Destination (leave empty to skip): ");
	city.bfs(s, t);
}
void RouteWise::dfsMenu()const {
	string s = readLine("Start location: ");
	string t = readLine("Destination (leave empty to skip): ");
	city.dfs(s, t);
}
void RouteWise::shortestMenu()const {
	string s = readLine("Source: ");
	string d = readLine("Destination: ");
	city.shortestPath(s, d);
}
void RouteWise::compareTrees()const {
	cout << "Live parcel index -> BST height: " << bst.height() << " | AVL height: " << avl.height() << "\n";
	int n = readInt("Sequential test size (1-2000): ");
	if (n < 1 || n>2000) { cout << "Invalid size\n"; return; }
	BST tb;
	AVL ta;
	for (int i = 1; i <= n; i++) {
		tb.insert(i, nullptr);
		ta.insert(i, nullptr);
	}
	cout << "Sequential keys 1.." << n << " -> BST height: " << tb.height() << " | AVL height: " << ta.height() << "\n";
}
void RouteWise::stats()const {
	cout << "Customers: " << customers.size() << "\n";
	cout << "Parcels: " << parcels.size() << " (hash capacity " << parcels.cap() << ")\n";
	cout << "Pending urgent: " << urgentHeap.size() << "\n";
	cout << "Pending normal: " << normalQueue.size() << "\n";
	cout << "Delivered: " << deliveredCount << "\n";
	cout << "History entries: " << history.size() << "\n";
	cout << "Locations: " << city.size() << " | Roads: " << city.roadCount() << "\n";
	cout << "BST height: " << bst.height() << " | AVL height: " << avl.height() << "\n";
}
void RouteWise::loadSample() {
	const char* locs[] = { "Warehouse","City Center","University","Hospital","Airport","Railway Station","Industrial Area","Residential Area" };
	for (int i = 0; i < 8; i++)city.addLocation(locs[i]);
	city.addRoad("Warehouse", "City Center", 5);
	city.addRoad("Warehouse", "Industrial Area", 7);
	city.addRoad("City Center", "University", 6);
	city.addRoad("City Center", "Hospital", 4);
	city.addRoad("City Center", "Railway Station", 8);
	city.addRoad("University", "Airport", 7);
	city.addRoad("Hospital", "Residential Area", 5);
	city.addRoad("Railway Station", "Airport", 10);
	city.addRoad("Industrial Area", "Railway Station", 6);
	city.addRoad("Residential Area", "University", 9);
	customers.add(1, "Ali Khan", "03001234567", "Gulberg");
	customers.add(2, "Sara Ahmed", "03217654321", "Model Town");
	customers.add(3, "Hamza Malik", "03451112233", "Johar Town");
	createParcel(1001, 1, "Warehouse", "Airport", 2.5, 3);
	createParcel(1002, 2, "Warehouse", "Hospital", 1.0, 9);
	createParcel(1003, 3, "Warehouse", "University", 5.2, 5);
	createParcel(1004, 1, "Warehouse", "Railway Station", 3.3, 1);
	createParcel(1005, 2, "Warehouse", "Residential Area", 0.8, 7);
	createParcel(1006, 3, "Warehouse", "Industrial Area", 4.1, 9);
	createParcel(1007, 1, "Warehouse", "City Center", 2.0, 2);
	createParcel(1008, 2, "Warehouse", "Airport", 6.0, 4);
	cout << "Sample data loaded\n";
}
void RouteWise::run() {
	string a = readLine("Load sample data? (y/n): ");
	if (!a.empty() && (a[0] == 'y' || a[0] == 'Y'))loadSample();
	int choice = 0;
	while (choice != 18) {
		printMenu();
		choice = readInt("Enter choice: ");
		if (cin.fail())break;
		switch (choice) {
		case 1:registerCustomer(); break;
		case 2:registerParcel(); break;
		case 3:searchParcel(); break;
		case 4:addRequest(false); break;
		case 5:addRequest(true); break;
		case 6:processNext(); break;
		case 7:markDelivered(); break;
		case 8:undoLast(); break;
		case 9:displayPending(); break;
		case 10:displayParcels(); break;
		case 11:cityMenu(); break;
		case 12:bfsMenu(); break;
		case 13:dfsMenu(); break;
		case 14:shortestMenu(); break;
		case 15:city.minimumSpanningTree(); break;
		case 16:compareTrees(); break;
		case 17:stats(); break;
		case 18:cout << "Goodbye\n"; break;
		default:cout << "Invalid choice\n";
		}
	}
}