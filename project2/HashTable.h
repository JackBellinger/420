#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "RBTree.h"
#include <vector>
#include <cstddef>
#include <string>

using namespace std;

class Hashable
{
	public:
		Hashable();
		virtual long int strToNum() = 0;

		virtual bool equal_to(Hashable& h) const = 0;
		
		bool operator==(Hashable& h) const
		{
			return equal_to(h);

		}
		bool operator!=(Hashable& h) const
		{
			return !equal_to(h);

		}
		
		virtual string to_string() = 0;

		friend ostream& operator << (ostream& stream, Hashable& h)
		{
			return stream << h.to_string();
		}
		/*
		   virtual bool operator == (const Hashable&)const = 0;
		   virtual bool operator != (const Hashable&)const = 0;
		   virtual bool operator >  (const Hashable&)const = 0;
		   virtual bool operator >= (const Hashable&)const = 0;
		   virtual bool operator <  (const Hashable&)const = 0;
		   virtual bool operator <= (const Hashable&)const = 0;
		   */
};

class Word : public Hashable
{	
	public:
		string str;
		int size;
		static double hashValue;

		Word();
		Word(string);	

		long strToNum();
		
		bool equal_to(Hashable& s) const;
		
		bool operator==(Hashable& s) const
		{
			return equal_to(s);

		}
		
		bool operator!=(Hashable& s) const
		{
			return !equal_to(s);

		}
		
		string to_string();

		/*bool operator == ( Hashable&)const;
		bool operator != (const Hashable&)const;
		bool operator >  (const Hashable&)const;
		bool operator >= (const Hashable&)const;
		bool operator <  (const Hashable&)const;
		bool operator <= (const Hashable&)const;
		*/
};

class HashTable
{
	public:
		HashTable();
		HashTable(size_t);
		long modHash(long);
		long multHash(long);
		long insert(Hashable*);
		bool contains(Hashable*);
		void stats();

		size_t size = 0;
		size_t numElements = 0;
		size_t filledBuckets = 0;
		long maxFilledIndex = 0;
		double loadTime = 0;
		RBTree<Hashable*>* data;// can't use rbt atm because inorder prints the pointers and not the words
		//vector<Hashable*>* data;
};
#endif
