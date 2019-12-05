#include "HashTable.h"
#include "RBTree.h"
#include <iostream>
#include <cmath>

HashTable::HashTable()
{

}

HashTable::HashTable(size_t s)
{
	size = s;
	data = new RBTree<Hashable*>[size+1];
	//data = new vector<Hashable*>[size+1];
	for(size_t i = 0; i <= size; i++)
		//data[i] = vector<Hashable*>();
		data[i] = RBTree<Hashable*>();

}

long HashTable::modHash(long n)
{
	return n % size;
}

long HashTable::multHash(long n)
{
	long w = 31;
	//double A = 0.618033989;
	//a * 2^w // a = s/2^31
	double s = 1327217653;	

	long temp = n * s;
	temp = temp & ((1<<w) -1);
	temp = temp >> (w-13);

	return temp;
}

long HashTable::insert(Hashable* h)
{
	long index = multHash(h->strToNum());

	data[index].rbtInsert(h);
	//data[index].push_back(h);

	if(data[index].size() > data[maxFilledIndex].size())
		maxFilledIndex = index;
	if(data[index].size() == 1)
		filledBuckets++;
	numElements++;

	return index;
}

bool HashTable::contains(Hashable* h)
{
	long index = multHash(h->strToNum());
	bool isThere = data[index].rbtSearch(h);
	//bool isThere = false;
	//cout << "checking chain " << endl;
	//for(Hashable* word: data[index])
	//	if(*word == *h)
	//		isThere = true;
	return isThere;
}

void HashTable::stats()
{
	double averageBucketSize =  0;
	long minFilledIndex = -1;
	cout << "Total number of words: " << numElements << endl;
	cout << "Total number of Buckets: "<< size << endl;
	cout << "Buckets in use: " << filledBuckets << endl;
	cout << "Time to load: " << loadTime << "s" << endl << endl;
	for(int i = 1; i < size; i++)
	{
		if(data[i].size() > 0)
		{
			long s = data[i].size();
			averageBucketSize += s;
			if(minFilledIndex == -1 || s < data[minFilledIndex].size())
				minFilledIndex = i;

		}

	}
	averageBucketSize /= filledBuckets;

	cout << "Largest bucket size: " << data[maxFilledIndex].size() << endl;
	cout << "Average bucket size: " << averageBucketSize << endl;
	cout << "Smallest bucket size: " << data[minFilledIndex].size() << endl;
}
