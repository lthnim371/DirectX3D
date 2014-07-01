
#include <iostream>
#include <vector>
#include <string>
using namespace std;

struct sNode
{
	string name;
	sNode* left;
	sNode* right;

	sNode() {left=NULL, right=NULL;}
	sNode(const string &n) : name(n) {left=NULL, right=NULL;}
	sNode(const string &n, sNode *_l, sNode *_r) :
		name(n), left(_l), right(_r) {}
};

void print(sNode *node)
{
	if (!node)
		return;

	print(node->left);
	print(node->right);
	cout << node->name << endl;
}

sNode* find( const string& name, sNode* const node )
{	
	sNode* temp1 = NULL;
	sNode* temp2 = NULL;
	sNode* result = NULL;

	if(!node)
		return NULL;

	temp1 = find( name, node->left );
	temp2 = find( name, node->right );

	if( node->name == name )
		return node;

	if(temp1)
		result = temp1;
	else if(temp2)
		result = temp2;

	return result;
}

void main()
{
	sNode *root = new sNode( "1",
		new sNode("2", 
			new sNode("4", NULL, NULL),
			new sNode("5", NULL, NULL)
			),
		new sNode("3", 
			new sNode("6", NULL, NULL),
			new sNode("7", NULL, NULL)
			)
		);

	sNode* print = find( "7", root );
	if(!print)
		cout << "NULL" << endl;
	else if(print)
		cout << print->name << endl;
//	print(root);
}
