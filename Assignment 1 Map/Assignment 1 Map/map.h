#pragma once
#include <utility>
using namespace std;

template <typename K, typename V>
class Map
{
private:
	struct SData
	{
		K key;
		V value;
	};

	unsigned int arraySize = 10;		// current array size
	unsigned int currentArraySize = 0;	// number of values stored in the array
	SData* mp_Data;

	pair<bool, unsigned int> FindInArray(const K& key, unsigned int start, unsigned int end);

public:

	class Iterator
	{
	private:
		SData* data;
	public:
		Iterator(SData* data) : data(data) {}
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
		
		auto operator*() { return make_pair(data->key, data->value); }
		auto operator->() { return &make_pair(data->key, data->value); }
		bool operator==(const Iterator& rhs) { return data == rhs.data; }
		bool operator!=(const Iterator& rhs) { return data != rhs.data; }
	};

	Map();															// constructor
	~Map();															// deconstructor
	bool Add(const K &key, const V &value);							// adds a new key pair value
	unsigned int DataSize() { return currentArraySize; }			// returns the number of data inserted into the array
	unsigned int Size() { return arraySize; }						// returns the size of the pre allocated array
	bool SetSize(unsigned int newSize);								// set size of the array
	bool ForceSetSize(unsigned int newSize);						// unsafe does not check for data loss
	V* Get(const K &key);											// Gets the Value from the key
	Iterator Begin() { return Iterator(mp_Data); }					// Iterator start
	Iterator End() { return Iterator(mp_Data + currentArraySize); }	// iterator end
};

template<typename K, typename V>
inline pair<bool, unsigned int> Map<K, V>::FindInArray(const K& key, unsigned int start, unsigned int end)
{
	unsigned int mid = 0;
	if (end >= start && end != -1)
	{
		// check mid
		mid = start + (end - start) / 2;
		if (mp_Data[mid].key == key)
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
	mp_Data = new SData[arraySize];
}

template<typename K, typename V>
inline Map<K, V>::~Map()
{
	delete[] mp_Data;
}

template<typename K, typename V>
inline bool Map<K, V>::Add(const K &key, const V &value)
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
		// find position again
		result = FindInArray(key, 0, currentArraySize-1);
	}

	// move data over 1
	memmove(&mp_Data[result.second + 1], &mp_Data[result.second], sizeof(SData) * (currentArraySize - result.second));

	// insert
	mp_Data[result.second].key = key;
	mp_Data[result.second].value = value;
	++currentArraySize;
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
	memcpy(newData, mp_Data, sizeof(SData) * arraySize);

	// delete old arrays
	delete[] mp_Data;

	// set new data
	mp_Data = newData;
	arraySize = newSize;

	return true;
}

template<typename K, typename V>
inline V* Map<K, V>::Get(const K &key)
{
	auto result = FindInArray(key, 0, currentArraySize-1);
	if (result.first)
		return &mp_Data[result.second].value;
	else
		return nullptr;
}
