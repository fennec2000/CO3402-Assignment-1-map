#pragma once
#include <utility>
using namespace std;

template <typename K, typename V>
class Map
{
private:
	struct SData // struct to hold each key value pair
	{
		K key;
		V value;
	};

	// private variables
	unsigned int arraySize = 10;		// current array size
	unsigned int currentArraySize = 0;	// number of values stored in the array
	SData* mp_Data;						// pointer to struct array to store data
	bool m_is_trivially_copyable;		// stores bool wether is data trivially copyable for performace boost

	// private functions
	pair<bool, unsigned int> FindInArray(const K& key, unsigned int start, unsigned int end);	// binary search find value first = found, second = position

public:

	// iterator class
	class Iterator
	{
	private:
		SData* data;
	public:
		Iterator(SData* data) : data(data) {}	// constructor
		Iterator operator++()		// prefix ++
		{
			Iterator x = *this;
			++data;
			return x;
		}
		Iterator operator++(int)	// postfix ++
		{
			++data;
			return *this;
		}
		Iterator operator--()		// prefix --
		{
			Iterator x = *this;
			--data;
			return x;
		}
		Iterator operator--(int)	// postfix --
		{
			--data;
			return *this;
		}
		
		// operator overloads
		auto operator*() { return make_pair(data->key, data->value); }
		auto operator->() { return &make_pair(data->key, data->value); }
		bool operator==(const Iterator& rhs) { return data == rhs.data; }
		bool operator!=(const Iterator& rhs) { return data != rhs.data; }
	};

	Map();								// constructor
	Map(const Map& m);					// copy constructor
	virtual ~Map();						// deconstructor
	Map& operator=(const Map& m)		// = operator overloading
	{
		swap(m);
		return *this;
	}

	// Iterators
	Iterator Begin() { return Iterator(mp_Data); }					// Iterator start
	Iterator End() { return Iterator(mp_Data + currentArraySize); }	// iterator end

	// Capacity
	unsigned int Size() { return currentArraySize; }	// returns the number of data inserted into the array
	unsigned int MaxSize() { return arraySize; }		// returns the size of the pre allocated array
	bool Empty() { return !currentArraySize; }			// returne if the array has no data in it
	bool SetSize(unsigned int newSize);					// set size of the array
	bool ForceSetSize(unsigned int newSize);			// unsafe does not check for data loss

	// Modifiers
	bool Insert(const K &key, const V &value);	// adds a new key pair value
	bool Erase(const K &key);					// removes a key value pair
	bool Clear();								// removes all data from map and sets array size to default
	
	// Operations
	V* Find(const K &key);	// Gets the Value from the key

};

template<typename K, typename V>
inline pair<bool, unsigned int> Map<K, V>::FindInArray(const K& key, unsigned int start, unsigned int end)
{
	unsigned int mid = 0;
	if (end >= start && end != -1)
	{
		// check mid
		mid = start + (end - start) / 2;
		if (!(mp_Data[mid].key < key || key < mp_Data[mid].key))
			return pair<bool, unsigned int>(true, mid);

		// recursion
		if (key < mp_Data[mid].key)
			return FindInArray(key, start, mid - 1);
		else
			return FindInArray(key, mid + 1, end);
	}

	return pair<bool, unsigned int>(false, start);
}

template <typename K, typename V>
inline Map<K, V>::Map()
{
	// setup
	mp_Data = new SData[arraySize];
	m_is_trivially_copyable = (is_trivially_copyable<K>::value && is_trivially_copyable<V>::value) ? true : false;
}

template<typename K, typename V>
inline Map<K, V>::Map(const Map & m)
{
	// set array size
	arraySize = m.arraySize;
	mp_Data = new SData[arraySize];

	// check and copy
	currentArraySize = m.currentArraySize;
	m_is_trivially_copyable = m.m_is_trivially_copyable;
	if (m_is_trivially_copyable)
		memcpy(mp_Data, m.mp_Data, sizeof(SData) * currentArraySize);
	else
	{
		// must copy via loop instead of memcpy for not TriviallyCopyable data
		// slower but must be used to prevent leaks
		for (unsigned int i = 0; i < currentArraySize; ++i)
			mp_Data[i] = m.mp_Data[i];
	}
}

template<typename K, typename V>
inline Map<K, V>::~Map()
{
	delete[] mp_Data;
}

template<typename K, typename V>
inline bool Map<K, V>::Insert(const K &key, const V &value)
{
	// check if key exists
 	auto result = FindInArray(key, 0, currentArraySize-1);
	if (result.first)
		return false;

	// check if array is big enough
	if (currentArraySize >= arraySize)
	{
		// resize
		ForceSetSize(2 * arraySize);
		// may need to find position again
	}

	// move data over 1
	if (m_is_trivially_copyable)
		memmove(&mp_Data[result.second + 1], &mp_Data[result.second], sizeof(SData) * (currentArraySize - result.second));
	else
	{
		// must copy via loop instead of memmove for not TriviallyCopyable data
		// slower but must be used to prevent leaks
		for (unsigned int i = currentArraySize; i > result.second; --i)
			mp_Data[i] = mp_Data[i - 1];
	}

	// insert
	mp_Data[result.second].key = key;
	mp_Data[result.second].value = value;
	++currentArraySize;
	return true;
}

template<typename K, typename V>
inline bool Map<K, V>::Erase(const K & key)
{
	// check if key exists
	auto result = FindInArray(key, 0, currentArraySize - 1);
	if (!result.first)
		return false;

	// move data over 1
	if (m_is_trivially_copyable)
		memmove(&mp_Data[result.second], &mp_Data[result.second + 1], sizeof(SData) * (currentArraySize - result.second));
	else
	{
		// must copy via loop instead of memmove for not TriviallyCopyable data
		// slower but must be used to prevent leaks
		for (unsigned int i = result.second; i < currentArraySize; ++i)
			mp_Data[i] = mp_Data[i + 1];
	}

	// reduce the array size
	--currentArraySize;

	// duplicate data at the end of array but due to array size it is not checked
	// this means it does not need to be deleted
	return true;
}

template<typename K, typename V>
inline bool Map<K, V>::Clear()
{
	// delete
	delete[] mp_Data;
	// reset vaiables
	currentArraySize = 0;
	arraySize = 10;
	// make new array
	mp_Data = new SData[arraySize];
	return true;
}

template<typename K, typename V>
inline bool Map<K, V>::SetSize(unsigned int newSize)
{
	if (newSize <= arraySize)
		return false;

	return ForceSetSize(newSize);
}

template<typename K, typename V>
inline bool Map<K, V>::ForceSetSize(unsigned int newSize)
{
	// make new arrays
	SData* newData = new SData[newSize];

	// copy data to new array
	if (m_is_trivially_copyable)
		memcpy(newData, mp_Data, sizeof(SData) * arraySize);
	else
	{
		// must copy via loop instead of memcpy for not TriviallyCopyable data
		// slower but must be used to prevent leaks
		for (unsigned int i = 0; i < arraySize; ++i)
			newData[i] = mp_Data[i];
	}

	// delete old arrays
	delete[] mp_Data;

	// set new data
	mp_Data = newData;
	arraySize = newSize;

	return true;
}

template<typename K, typename V>
inline V* Map<K, V>::Find(const K &key)
{
	auto result = FindInArray(key, 0, currentArraySize-1);
	if (result.first)
		return &mp_Data[result.second].value;
	else
		return nullptr;
}
