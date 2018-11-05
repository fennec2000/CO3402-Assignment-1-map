#pragma once
using namespace std;

class Iterator;

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
	unsigned int arraySize = 10;		// current array size
	unsigned int currentArraySize = 0;	// number of values stored in the array
	K* mp_Key = nullptr;				// pointer to the key array
	V* mp_Value = nullptr;				// pointer to the value array
	bool* mp_Taken = nullptr;			// pointer to the array that stores if value is used in array

public:

	class Iterator
	{
		K* m_ptr;
	public:
		Iterator(K* ptr) { m_ptr = ptr; }
		Iterator operator++()		// prefix ++
		{
			return *this;
		}
		Iterator operator++(int)	// postfix ++
		{
			return *this;
		}
		const K& operator*() { return *m_ptr; }
		const K* operator->() { return m_ptr; }
		bool operator==(const Iterator& rhs) { return m_ptr == rhs.m_ptr; }
	};

	Map();													// constructor for setup
	virtual ~Map();											// deconstructor to clean up memory
	bool Add(const K &key, const V &value);					// adds a new key pair value
	unsigned int DataSize() { return currentArraySize; }	// returns the number of data inserted into the array
	unsigned int Size() { return arraySize; }				// returns the size of the pre allocated array
	bool SetSize(unsigned int newSize);						// set size of the array
	bool ForceSetSize(unsigned int newSize);				// unsafe does not check for data loss
	V* Get(const K &key);								// Gets the Value from the key
};

template <typename K, typename V>
inline Map<K, V>::Map()
{
	mp_Key = new K[arraySize];
	mp_Value = new V[arraySize];
	mp_Taken = new bool[arraySize];
	for (int i = 0; i < arraySize; ++i)
		mp_Taken[i] = false;
}

template<typename K, typename V>
inline Map<K, V>::~Map()
{
	delete[] mp_Key;
	delete[] mp_Value;
	delete[] mp_Taken;
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
	while (mp_Taken[insertPoint])
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
	mp_Taken[insertPoint] = true;
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
	bool* newTaken = new bool[newSize];
	for (int i = 0; i < newSize; ++i)
		newTaken[i] = false;

	// go through the old array and get all data
	int loopCheck = (arraySize < newSize ? arraySize : newSize);

	for (int i = 0, j = 0; i < loopCheck && j < currentArraySize; ++i)
	{
		if (mp_Taken[i])
		{
			// rehash and re-enter into new arrays
			unsigned int newHash = MapHash(mp_Key[i]) % newSize;

			// find next space
			while (newTaken[newHash])
			{
				if (newHash >= newSize)
					newHash = 0;
				else
					++newHash;
			}

			newKey[newHash] = mp_Key[i];
			newValue[newHash] = mp_Value[i];
			newTaken[newHash] = true;
			++j;
		}
	}

	// delete old arrays
	delete[] mp_Key;
	delete[] mp_Value;
	delete[] mp_Taken;

	// set new data
	mp_Key = newKey;
	mp_Value = newValue;
	mp_Taken = newTaken;
	arraySize = newSize;

	return true;
}

template<typename K, typename V>
inline V* Map<K, V>::Get(const K &key)
{
	unsigned int findHash = MapHash(key) % arraySize;

	// check each value in array - do not check more than the array size
	for(int i = 0; i < arraySize; ++i)
	{
		// if its there
		if (mp_Key[findHash] == key)
			return &mp_Value[findHash];
		// if its not there
		if (!mp_Taken[findHash])
			return nullptr;
		// something else go to next
		++findHash;
		if (findHash >= arraySize)
			findHash = 0;
	}

	return nullptr;
}