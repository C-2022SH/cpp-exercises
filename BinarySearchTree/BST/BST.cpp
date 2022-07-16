#include <iostream>

#include "BST.h"

using namespace std;


int main()
{
	CBST<int, int> BSTInt;

	BSTInt.insert(make_bstpair(48, 200));
	BSTInt.insert(make_bstpair(27, 100));
	BSTInt.insert(make_bstpair(74, 300));
	BSTInt.insert(make_bstpair(15, 200));
	BSTInt.insert(make_bstpair(88, 100));
	BSTInt.insert(make_bstpair(9, 300));
	BSTInt.insert(make_bstpair(64, 200));
	BSTInt.insert(make_bstpair(11, 100));
	BSTInt.insert(make_bstpair(10, 300));
	BSTInt.insert(make_bstpair(39, 200));
	BSTInt.insert(make_bstpair(42, 100));
	BSTInt.insert(make_bstpair(45, 300));
	BSTInt.insert(make_bstpair(63, 200));
	BSTInt.insert(make_bstpair(17, 100));
	BSTInt.insert(make_bstpair(16, 300));
	BSTInt.insert(make_bstpair(18, 200));
	BSTInt.insert(make_bstpair(22, 100));
	BSTInt.insert(make_bstpair(20, 300));
	BSTInt.insert(make_bstpair(24, 200));
	BSTInt.insert(make_bstpair(81, 100));
	BSTInt.insert(make_bstpair(92, 300));
	BSTInt.insert(make_bstpair(77, 200));
	BSTInt.insert(make_bstpair(83, 100));
	BSTInt.insert(make_bstpair(100, 300));

	CBST<int, int>::iterator bstiter = BSTInt.begin();
	bstiter = BSTInt.find(20);

	//cout << bstiter->first << endl;

	for (bstiter = BSTInt.begin(); bstiter != BSTInt.end(); ++bstiter)
	{
		cout << bstiter->first << endl;
	}

	return 0;
}