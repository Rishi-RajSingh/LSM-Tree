#include<iostream>
#include<string>
#include<sstream>
#include<fstream>
#include<climits>
#include "Bplustree.h"

using namespace std;

//to create a fresh tree, do not give any command line argument
int main()
{
	BPTree bpt;//B+ tree object that carries out all the operations
	string command;
	int x;
	bool close = false;
	//create tree

	cout<<"Enter the max degree\n";
	cin>>command;
	stringstream max(command);
	max>>MAX;
	cin.clear();
	cin.ignore(1);

	//command line menu
	cout<<"Commands:\nsearch <value> to search\n";
	cout<<"insert <value> to insert\n";
	cout<<"delete <value> to delete\n";
	cout<<"display to display\n";
	cout<<"exit to exit\n";
	do
	{
		cout<<"Enter command: ";
		getline(cin,command);
		if(!command.substr(0,6).compare("search"))
		{
			stringstream argument(command.substr(7));
			argument>>x;
			bpt.search(x);
		}
		else if(!command.substr(0,6).compare("insert"))
		{
			stringstream argument(command.substr(7));
			argument>>x;
			bpt.insert(x);
      bpt.display(bpt.getRoot());
		}
		else if(!command.substr(0,6).compare("delete"))
		{
			stringstream argument(command.substr(7));
			argument>>x;
			bpt.remove(x);
		}
		else if(!command.compare("display"))
		{
			bpt.display(bpt.getRoot());
		}
		else if(!command.compare("exit"))
		{
			close = true;
		}
		else
		{
			cout<<"Invalid command\n";
		}
	}while(!close);
	return 0;
}
