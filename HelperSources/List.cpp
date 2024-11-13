//
//  List.cpp
//


#include "List.h"


Node::Node()
{
	data = NULL;
}

Node::~Node()
{
	if(data)
	{
        delete (Node*)data;
		data = NULL;
	}
}



int List::count()
{
	return _count;
}

void * List::get(int index)
{
	Node *tmp = head;
    if ( tmp == NULL ) {
		return NULL;
	}
	if(index == 0) {
		return head->Data();
	}
	if ( tmp->Next() == NULL ) {
		return NULL;
	}
	do {
		tmp = tmp->Next();
		index --;
	}
	while ( tmp != NULL && index > 0 );
	if(tmp == NULL) {
		return NULL;
	}
	return tmp->Data();
}

void List::Print()
{	
    // Temp pointer
    Node *tmp = head;
	
    // No nodes
    if ( tmp == NULL ) {
		//cout << "EMPTY" << endl;
		return;
    }
	
    // One node in the list
    if ( tmp->Next() == NULL ) {
		//cout << tmp->Data();
		//cout << " --> ";
		//cout << "NULL" << endl;
    }
    else {
		// Parse and print the list
		do {
			//cout << tmp->Data();
			//cout << " --> ";
			tmp = tmp->Next();
		}
		while ( tmp != NULL );
		
		//cout << "NULL" << endl;
    }
}

void List::Append(void * data)
{
	if(data == NULL) {
		return;
	}
    // Create a new node
    Node* newNode = new Node();
    newNode->SetData(data);
    newNode->SetNext(NULL);
	
	_count ++;
	
    // Create a temp pointer
    Node *tmp = head;
	
    if ( tmp != NULL ) {
		// Nodes already present in the list
		// Parse to end of list
		while ( tmp->Next() != NULL ) {
			tmp = tmp->Next();
		}
		
		// Point the last node to the new node
		tmp->SetNext(newNode);
    }
    else {
		// First node in the list
		head = newNode;
    }
}

int List::Delete(void * data)
{
	if(data == NULL) {
		return 0;
	}

    // Create a temp pointer
    Node * tmp = head;
	
    // No nodes
    if ( tmp == NULL )
		return 0;
	
    // Last node of the list
    if ( tmp->Next() == NULL ) {
		delete tmp;
		head = NULL;
    }
    else
	{		
		// Parse thru the nodes
		Node *prev = NULL;
		do {
			if ( tmp->Data() == data ) break;
			prev = tmp;
			tmp = tmp->Next();
		} while ( tmp != NULL );
		
		if(tmp == NULL)
		{
			//printf("suppression liste element inconnu\n");
			return 0;
		}
		
		if(prev == NULL)
		{
			head = tmp->Next();
		}
		else
		{
			// Adjust the pointers
			prev->SetNext(tmp->Next());
		}
		// Delete the current node
		delete tmp;
    }
	_count --;
    return 1;
}
