#pragma once
using namespace std;

// custom hashing algorithm, converts any data type to a long
template<typename T>
inline long MapHash(T given)
{
	const long result = reinterpret_cast<long&>(given);
	return result * result;		// squared to decrease cashe misses
}

template <typename K, typename V>
class Map
{
private:
	K* emptyKey = new K;				// value of an empty key
	V* emptyValue = new V;				// value of an empty value
	unsigned int arraySize = 10;		// current array size
	unsigned int currentArraySize = 0;	// number of values stored in the array
	K* mp_Key = nullptr;				// pointer to the key array
	V* mp_Value = nullptr;				// pointer to the value array

public:

	class Iterator
	{
		K* m_ptr;
	public:
		Iterator(K* ptr) { m_ptr = ptr; }
		Iterator operator++();		// prefix ++
		Iterator operator++(int);	// postfix ++
		const K& operator*() { return *m_ptr; }
		const K* operator->() { return m_ptr; }
		bool operator==(const Iterator& rhs) { return m_ptr == rhs.m_ptr; }
	};

	inline Iterator Iterator::operator++()
	{
		return *this;
	}

	inline Iterator Iterator::operator++(int)
	{
		return *this;
	}

	Map();													// constructor for setup
	virtual ~Map();											// deconstructor to clean up memory
	bool Add(const K &key, const V &value);					// adds a new key pair value
	unsigned int DataSize() { return currentArraySize; }	// returns the number of data inserted into the array
	unsigned int Size() { return arraySize; }				// returns the size of the pre allocated array
	bool SetSize(unsigned int newSize);						// set size of the array
	bool ForceSetSize(unsigned int newSize);				// unsafe does not check for data loss
	const V &Get(const K &key);								// Gets the Value from the key
};

template <typename K, typename V>
inline Map<K, V>::Map()
{
	mp_Key = new K[arraySize];
	mp_Value = new V[arraySize];
}

template<typename K, typename V>
inline Map<K, V>::~Map()
{
	delete emptyKey;
	delete emptyValue;
	delete[] mp_Key;
	delete[] mp_Value;
}

template<typename K, typename V>
inline bool Map<K, V>::Add(const K &key, const V &value)
{
	// check if key exists
	long insertPoint = MapHash(key) % arraySize;
	if (key == mp_Key[insertPoint])
		return false;

	// check if array is big enough
	if (currentArraySize >= arraySize * 0.75f)
	{
		// resize
		ForceSetSize(2 * arraySize);
		insertPoint = MapHash(key) % arraySize;
	}

	// find next avalable spot
	while (!(mp_Key[insertPoint] == *emptyKey))
	{
		if (insertPoint >= arraySize)
			insertPoint = 0;
		else
			++insertPoint;

		// check again if key exists
		if (key == mp_Key[insertPoint])
			return false;
	}

	// insert
	mp_Key[insertPoint] = key;
	mp_Value[insertPoint] = value;
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
	K* newKey = new K[newSize];
	V* newValue = new V[newSize];

	// go through the old array and get all data
	int loopCheck = (arraySize < newSize ? arraySize : newSize);

	for (int i = 0, j = 0; i < loopCheck && j < currentArraySize; ++i)
	{
		if (!(mp_Key[i] == *emptyKey))
		{
			// rehash and re-enter into new arrays
			unsigned int newHash = MapHash(mp_Key[i]) % newSize;

			// find next space
			while (!(newKey[newHash] == *emptyKey))
			{
				if (newHash >= newSize)
					newHash = 0;
				else
					++newHash;
			}

			newKey[newHash] = mp_Key[i];
			newValue[newHash] = mp_Value[i];
			++j;
		}
	}

	// delete old arrays
	delete[] mp_Key;
	delete[] mp_Value;

	// set new data
	mp_Key = newKey;
	mp_Value = newValue;
	arraySize = newSize;

	return true;
}

template<typename K, typename V>
inline const V &Map<K, V>::Get(const K &key)
{
	unsigned int findHash = MapHash(key) % arraySize;

	// check each value in array - do not check more than the array size
	for(int i = 0; i < arraySize; ++i)
	{
		// if its there
		if (mp_Key[findHash] == key)
			return mp_Value[findHash];
		// if its not there
		if (mp_Key[findHash] == *emptyKey)
			return *emptyValue;
		// something else go to next
		++findHash;
		if (findHash >= arraySize)
			findHash = 0;
	}

	return *emptyValue;
}
