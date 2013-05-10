// BTree.cpp : main project file.

#include <iostream>
#include <cstdlib>
#include <cstddef>
#include <cstring>
#include <fstream>
#include "BTree.h"


using namespace std;
bool readCommands(const char* commandFile, BTree<int, 5>& aBTree);
bool readBulkInput(const char* bulkLoadFile, BTree<int, 5>& aBTree);

int main(int argc, char* argv[])
{
	char file[256];
	bool inputOK; //ensures comand line arguments are okay
    BTree<int, 5> myBTree; //create a BTree of type int and with M=5 for the purposes fo the assignment

	if(argc >=2)
		inputOK=true;
	else
	{
		inputOK=false;
		cout<<"No command line argument input files"<<endl;
		cout<<"Please run program again with a commandFile."<<endl;
		return 1;
	}
	if(readCommands(argv[1], myBTree))//something went wrong with the commandFile
		 //myBTree.print();
		 cout<<"file reads!\n";
	else
	{
		cout<<"Something went wrong with the commandFile.\nPlease check file and restart program.\n";
		return 2;
	}
	return 0;
}

bool readCommands(const char* commandFile, BTree<int, 5>& aBTree)
{
	char c [256];//stores the commands
	char file[256];//stores the bulk file name
	int number; //stores numbers from commandFile

	ifstream commands;
	commands.open(commandFile);

	if (!commands)
	{
		commands.close();
		cout << "Sorry, I cannot seem to find this file. \nPlease restart the program.";
		return false;
	}

	while (commands.good()) //start reading the file
	{
		commands>>c;
		if(strcmp(c,"L:")==0) //load bulk data file
		{
			cout<<"L\n";
			commands.ignore(1,' ');
			commands>>file;
			cout<<file<<endl;
			if(readBulkInput(file, aBTree))
				cout<<"Read data into tree.\n";
			else
			{
				cout<<"Problem with Bulk Input file....\n";
				return false;
			}
		}
		else if(strcmp(c,"A:")==0) //add a new int to tree
		{
			cout<<"A\n";
			commands>>number;
			cout<< "Adding "<<number<<endl;
			aBTree.insert(number);
		}
		else if(strcmp(c,"D:")==0) //deletes a number from the tree
		{
			cout<<"D\n";
			commands>>number;
			cout<<"Removing "<<number<<endl;
			aBTree.remove(number);
		}
		else if(strcmp(c,"P:")==0)//prints the tree
		{
			cout<<"P\n";
			aBTree.print();
		}
		else if(strcmp(c,"S:")==0) //searches for an item in the tree
		{
			cout<<"S\n";
			commands>>number;
			cout<<number<<endl;
			if(aBTree.find(number))
				cout<<"Found "<<number<<"!\n";
			else
				cout<<number<<" not in tree...\n";
		}
		else if(strcmp(c,"T:")==0) //end program
		{
			cout<<"T\n";
			commands.close();
			return true;
		}
		else
		{
			cout<<"I do not recognize that command.\nPlease check commandFile and restart program.\n";
			commands.close();
			return false;
		}
	}

}

bool readBulkInput(const char* bulkLoadFile, BTree<int, 5>& aBTree)
{
	int number; //stores numbers to be put in the tree

	ifstream bulkData;
	bulkData.open(bulkLoadFile);

	if (!bulkData)
	{
		bulkData.close();
		cout << "Sorry, I cannot seem to find the bulk data file. \nPlease restart the program.";
		return false;
	}

	while(bulkData) //start inputing values into the tree
	{
		bulkData>>number;
		cout<<number<<endl;
		aBTree.insert(number);
	}
	
	bulkData.close();

	return true;
}