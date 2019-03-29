//Fadi Ayad
//Stack-Queue-Bookshop
//Bookshop program where buying and selling books takes place

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

using namespace std;

class LinkedList {
public:
	string book;
	LinkedList *next;
};

class Queue : public LinkedList {
private:
	LinkedList *f;	//front
	LinkedList *r;	//rear
public:
	Queue();	//constructor
	void enqueue(string book);	//add book to queue
	void dequeue();	//remove book from queue
	string front();	//return book at front
	bool empty();	//check if queue is empty (return 1 or 0)
	int size();	//return size of queue
};

Queue::Queue() {
	f = NULL;
	r = NULL;
}

void Queue::enqueue(string book) {
	LinkedList *ptr = new LinkedList;
	ptr->book = book;
	ptr->next = NULL;
	if (f == NULL)
	{
		f = r = ptr;
		r->next = NULL;
	}
	else
	{
		r->next = ptr;
		r = ptr;
		r->next = NULL;
	}
}

void Queue::dequeue() {
	LinkedList *ptr = new LinkedList;
	if (empty()) {
		cout << "Queue is empty\n";
	}
	else {
		ptr = f;
		f = f->next;
		delete(ptr);
	}
}

string Queue::front() {
	if (!empty()) {
		return f->book;
	}
}

bool Queue::empty() {
	return (size() == 0);
}

int Queue::size() {
	int i = 0;
	LinkedList *cu = f;
	if (cu == NULL) {
		return i;
	}
	else {
		i++;
		while (cu->next != NULL) {
			cu = cu->next;
			i++;
		}
		return i;
	}
}

class Stack : public LinkedList {
private:
	LinkedList *t;	//top
public:
	Stack();	//constructor
	void push(string book);	//add book to stack
	void pop();	//remove book from stack
	string top();	//return top book
	bool empty();	//check if stack is empty (return 1 or 0)
	int size();	//return size of stack
};

Stack::Stack() {
	t = NULL;
}

void Stack::push(string book) {
	LinkedList *ptr = new LinkedList;
	ptr->book = book;
	ptr->next = t;
	t = ptr;
}

void Stack::pop() {
	LinkedList *ptr = new LinkedList;
	if (empty()) {
		cout << "Stack is empty\n";
	}
	else {
		ptr = t;
		t = t->next;
		delete(ptr);
	}
		
}

string Stack::top() {
	if (!empty()) {
		return t->book;
	}
}

bool Stack::empty() {
	return (size() == 0);
}

int Stack::size() {
	int i = 0;
	LinkedList *cu = t;
	if (cu == NULL) {
		return i;
	}
	else {
		i++;
		while (cu->next != NULL) {
			cu = cu->next;
			i++;
		}
		return i;
	}
}

int main(int argc, char* argv[]) {
	ArgumentManager am(argc, argv);
	string in = am.get("input");
	string out = am.get("output");

	ifstream infile;	// Creating instance
	ofstream outfile;	// Creating instance

	infile.open(in);
	outfile.open(out);

	if (!infile.is_open())	//print message if couldn't open file
	{
		cout << "Couldn't open input file.";
		return 0;
	}

	Queue queue;	//creating Queue object	
	Stack stack;	//creating Stack object
	Queue tempQ;	//creating Queue object

	string command = "";

	while (getline(infile, command)) {	//reading input file
		bool found = true;
		if (command.empty()) {
			continue;
		}
		else if (command[0] == 'b') {	//if buying book
			stringstream ss(command);
			string getWord;
			string book;
			string dummy;
			ss >> dummy;

			while (ss >> getWord) {	//getting book name
				if (book.empty()) {
					book = getWord;
				}
				else {
					book = book + ' ' + getWord;
				}
			}

			queue.enqueue(book);	//add book to queue
			stack.push(book);	//add book to stack
			ss.str("");	//clear ss
		}
		else if (command[0] == 's') {	//if selling
			stringstream ss(command);
			string getWord;
			string book;
			string dummy;
			ss >> dummy;

			while (ss >> getWord) {	//getting book name
				if (book.empty()) {
					book = getWord;
				}
				else {
					book = book + ' ' + getWord;
				}
			}

			int stackSize = stack.size();

			if (stack.size() == 0) {
				outfile << book << " not found\n";
			}
			else {
				for (int i = 1; i <= stackSize; i++) {	//keep iterating through the stack until we find the book we are going to sell
					if (i == stackSize && stack.top() != book) {	//if not found, inform user
						outfile << book << " not found\n";
						tempQ.enqueue(stack.top());	//store book getting popped from stack into temporary queue
						stack.pop();	//remove from stack
						found = false;
					}
					else if (stack.top() == book) {
						outfile << book << " finding cost at stack: " << i << ", ";
						stack.pop();	//once found, we will pop (sell) the book
						break;
					}
					else {
						tempQ.enqueue(stack.top());	//store book getting popped from stack into temporary queue
						stack.pop();	//remove from stack
					}
				}
				while (!tempQ.empty()) {	//return popped books that were not sold back to the stack
					stack.push(tempQ.front());	//add book from temporary queue to stack
					tempQ.dequeue();	//remove book from temporary queue
				}

				if (found) {
					int queueSize = queue.size();
					for (int i = 1; i <= queueSize; i++) {	//keep iterating through the queue until we find the book we are going to sell
						if (queue.front() == book) {
							outfile << "at queue: " << i << endl;
							queue.dequeue();	//once found, we will dequeue (sell) the book
							break;
						}
						else {	//move book from front to back of queue
							queue.enqueue(queue.front());	
							queue.dequeue();
						}
					}
				}
				ss.str("");	//clear ss
			}
		}
	}

	infile.close();
	outfile.close();
	return 0;
}
