//
//  List.h
//  Ball 404
//
//  Created by Julien on 10/03/13.
//
//

#ifndef __Ball_404__List__
#define __Ball_404__List__

#include <stdlib.h>

//#include <iostream>
//using namespace std;

class Node
{
    void * data;
    Node * next;
	
public:

				Node();
				~Node();
    
	void		SetData(void * aData) { data = aData; };
    void		SetNext(Node* aNext) { next = aNext; };
    void	*	Data() { return data; };
    Node	*	Next() { return next; };
};

class List
{
    Node *head;
	
	int		_count;

public:

    List() { _count = 0; head = NULL; };
    
	int			count();
	void	*	get(int index);
	void		Print();
    void		Append(void * data);
    int		    Delete(void * data);
};


#endif
