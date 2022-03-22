// Algorithms_Assignment4.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <stdlib.h>
#include <string>
#include <cstdint>
#include <set>
#include <stack>

using namespace std;


#define LIST_SIZE 10

class Node {
public:
    Node* next;
    string task;
    int index;

    Node() {
        next = nullptr;
        task = "";
        index = -1;
    }

    Node(int _index) : index(_index) {
        next = nullptr;
        task = "";
    }

};


class Diagraph {
public:

    // Adjacency list (array of header nodes)
    Node* adjList[LIST_SIZE];
    int numNodes;

    // DFS related params
    bool visited[LIST_SIZE];
    set<int> set;
    stack<int> stack;

    Diagraph() {

        for (int i = 0; i < LIST_SIZE; i++) {
			adjList[i] = nullptr;
            visited[i] = false;
        }
        
        numNodes = 0;

    }

    bool AddNode(int num, string task) {
        if (num >= LIST_SIZE) return false;

        Node* node = new Node(num);
        node->task = task;
        adjList[num] = node;
        numNodes++;

        return true;
    }

    bool AddEdge(string edge)
    {
        if (edge.length() < 3) return false;

        int count = 0;

        string token1 = edge.substr(0, edge.find(" "));
        string token2 = edge.substr(edge.find(" "), edge.length());

        // Failed to find integers
        if (token1.empty() || token2.empty()) return false;

        int index1 = stoi(token1);
        int index2 = stoi(token2);

        // Check if input is valid
        if (index1 > numNodes || index2 > numNodes) return false;

        // Check if nodes exist
        if (adjList[index1] == nullptr || adjList[index2] == nullptr) return false;

        // Check if nodes are the same
        if (index1 == index2) return false;
        
        // Check for cycle, check if exists already in header
        // Is only 1 node deep check
        Node* cur = adjList[index2];
        Node* prev = nullptr;
        do {
            prev = cur;
			cur = cur->next;
        } while (cur != nullptr && adjList[index1]->task != cur->task);

        if (cur != nullptr) return false; // cycle

        // Navigate through linked list till open next is found, then populate it
        cur = adjList[index1];
        prev = nullptr;
        do {
            prev = cur;
			cur = cur->next;
        } while (cur != nullptr);

        // Have to create copy
        Node* copy = new Node(index2);
        copy->task = adjList[index2]->task;
        prev->next = copy;

        return true;

    }

    bool DeleteNode(int num) {
        if (num >= LIST_SIZE) return false;

        delete adjList[num];
        adjList[num] = nullptr;

    }
    
    bool DeleteEdge(string edge) {
        
        if (edge.length() < 3) return false;

        int count = 0;

        string token1 = edge.substr(0, edge.find(" "));
        string token2 = edge.substr(edge.find(" "), edge.length());

        // Failed to find integers
        if (token1.empty() || token2.empty()) return false;

        int index1 = stoi(token1);
        int index2 = stoi(token2);

        // Check if input is valid
        if (index1 > numNodes || index2 > numNodes) return false;

        // Check if nodes exist
        if (adjList[index1] == nullptr || adjList[index2] == nullptr) return false;

        // Navigate through linked list till matching task is found
        string task = adjList[index2]->task;
        Node* cur = adjList[index1];
        Node* prev = nullptr;
        
        do {
            prev = cur;
			cur = cur->next;
        } while (cur != nullptr && cur->task != task);

        // Found node, free memory and remove reference
        if (cur != nullptr) {
            delete cur;
            prev->next = nullptr;
        }

        return true;

    }

    void Display() {

        for (int i = 0; i < numNodes; i++) {
            if (adjList[i] != nullptr) {
                Node* cur = adjList[i]->next;
                cout << i << ": ";
                while (cur != nullptr){
                    cout << cur->task << ", ";
                    cur = cur->next;
                } while (cur != nullptr);
            
                cout << endl;
            }
        }

    }

    bool Acyclical_Check() {
        for (int i = 0; i < LIST_SIZE; i++) {
            visited[i] = false;
        }

        for (int i = 0; i < numNodes; i++) {
			if (!visited[i]) {
                if (!_DFS(i)) {
                    return false;
                }
				
			}
        }
        return true;
    }

    void Topological_Sort() {
        for (int i = 0; i < LIST_SIZE; i++) {
            visited[i] = false;
        }

        while (!stack.empty()) {
            stack.pop();
        }

        for (int i = 0; i < numNodes; i++) {
			if (!visited[i]) {
				_Topological_Sort(i);
			}
        }
        
        cout << "Sorted DAG: \n";
        while (!stack.empty()) {
            cout << stack.top() << endl;
            stack.pop();
        }

    }

private:

    bool _DFS(int num) {
        if (num >= LIST_SIZE) return false;

        // Mark as discovered and push to set
        visited[num] = true;
        set.insert(num);

        // DFS on children
        Node* cur = adjList[num]->next;
        
        while (cur != nullptr){

            if (set.find(cur->index) != set.end()) {
                return false;
            }

            if (!visited[cur->index]) {
                if (!_DFS(cur->index)) {
                    return false;
                }
            }

            cur = cur->next;

        }
        
        // Remove set[num]
        set.erase(num);

        return true;

    }

    void _Topological_Sort(int num) {
        if (num >= LIST_SIZE) return;

        // Mark as discovered and push to set
        visited[num] = true;

        // DFS on children
        Node* cur = adjList[num]->next;
       
        while (cur != nullptr){

            if (!visited[cur->index]) {
                _Topological_Sort(cur->index);
            }

            cur = cur->next;

        }
        
        stack.push(num);

    }


};

int main()
{
    Diagraph* dag = new Diagraph();
    dag->AddNode(0, "zero");
    dag->AddNode(1, "one");
    dag->AddNode(2, "two");
    dag->AddNode(3, "three");
    dag->AddNode(4, "four");
    dag->AddNode(5, "five");
    dag->AddNode(6, "six");
    dag->AddNode(7, "seven");
    dag->AddNode(8, "eight");

    dag->AddEdge("1 7");
    dag->AddEdge("7 5");

    // 1D Cycle prevention test
    //dag->AddEdge("6 5");
    // acycle check
    //dag->AddEdge("4 1");

    cout << "is acyclical: " << dag->Acyclical_Check() << endl;

    dag->Display();

    dag->Topological_Sort();
}

