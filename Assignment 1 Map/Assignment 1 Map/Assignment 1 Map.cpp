//	G20363714

#include "pch.h"
#include "map.h"
#include <iostream>
#include <string>
// memory leak checks
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else
#define DBG_NEW new
#endif

using namespace std;

struct SCustomStruct
{
	int m_int;
	string m_string;
	~SCustomStruct() {}
};

bool operator< (const SCustomStruct& a, const SCustomStruct& b) { return a.m_int < b.m_int || (a.m_int == b.m_int && a.m_string < b.m_string); }

class CCustomClass
{
private:
	int m_int;
	string m_string;
public:
	CCustomClass() {}
	CCustomClass(int i, string s) : m_int(i), m_string(s) {}
	CCustomClass(const CCustomClass& c) : m_int(c.m_int), m_string(c.m_string) {}
	CCustomClass& operator=(const CCustomClass& c);
	virtual ~CCustomClass() {}
	void Speak() { cout << m_string << "\n"; }
	const int GetInt() { return m_int; }
	void SetInt(int a) { m_int = a; }
	const string* GetString() { return &m_string; }
	void SetString(const string& a) { m_string = a; }
	bool operator< (const CCustomClass& rhs) const { return m_int < rhs.m_int || (m_int == rhs.m_int && m_string < rhs.m_string); }
};

CCustomClass & CCustomClass::operator=(const CCustomClass & c)
{
	m_int = c.m_int;
	m_string = c.m_string;
	return *this;
}

int main()
{
	// flag for memory leaks
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// variables used for output
	bool testingBool;
	string testingString;
	string* testingStringPtr;
	int testingInt[2];

	// Tests
	cout << "Tests:\n";
	Map<int, int>* dataII = new Map<int, int>();
	cout << "PASS: Created dataII, an map<int,int>\n";
	Map<string, string>* dataSS = new Map<string, string>();
	cout << "PASS: Created dataSS, an map<string,string>\n";
	Map<float, float>* dataFF = new Map<float, float>();
	cout << "PASS: Created dataII, an map<int,int>\n";
	

	// test adding 1,1 to <int,int>
	testingBool = dataII->Insert(1, 1);
	cout << (testingBool ? "PASS" : "FAIL*") << ": Added 1, 1 to dataII result: " << testingBool << ", expected: 1\n";
	testingBool = dataII->Insert(1, 1);
	cout << (!testingBool ? "PASS" : "FAIL*") << ": Added duplicate 1, 1 to dataII result: " << testingBool << ", expected: 0\n";

	// test adding "Test","Test" to <string, string>
	testingString = string("TestString");
	testingBool = dataSS->Insert("TestString", "TestString");
	cout << (testingBool ? "PASS" : "FAIL*") << ": Added \"TestString\", \"TestString\" to dataSS result: " << testingBool << ", expected: 1\n";
	testingBool = dataSS->Insert("TestString", "TestString");
	cout << (!testingBool ? "PASS" : "FAIL*") << ": Added duplicate \"TestString\", \"TestString\" to dataSS result: " << testingBool << ", expected: 0\n";

	// test adding 0.1f,0.1f to <float,float>
	testingBool = dataFF->Insert(0.1f, 0.1f);
	cout << (testingBool ? "PASS" : "FAIL*") << ": Added 0.1f, 0.1f to dataFF result: " << testingBool << ", expected: 1\n";
	testingBool = dataFF->Insert(0.1f, 0.1f);
	cout << (!testingBool ? "PASS" : "FAIL*") << ": Added duplicate 0.1f, 0.1f to dataFF result: " << testingBool << ", expected: 0\n";

	// test retreving data
	testingStringPtr = dataSS->Find("TestString");
	if (testingStringPtr == nullptr)
		cout << "FAIL*:Retrieved nullptr from dataSS\n";
	else
	{
		testingString = *testingStringPtr;
		cout << (!strcmp(testingString.c_str(), "TestString") ? "PASS" : "FAIL*") << ": Retrieved \"TestString\" from dataSS result: \"" << testingString << "\", expected: \"TestString\"\n";
	}
		
	testingStringPtr = dataSS->Find("NotInDataSS");
	if (testingStringPtr == nullptr)
		cout << "PASS: Retrieved nullptr from dataSS\n";
	else
	{
		testingString = *testingStringPtr;
		cout << "FAIL: Retrieved '" << testingString << "' from DataSS, expected nullptr\n";
	}

	// Test adding more data than map size
	testingInt[0] = dataII->MaxSize();
	for (int i = 0; i < 15; ++i)
		dataII->Insert(i, i);
	testingInt[1] = dataII->MaxSize();
	cout << (testingInt[0] != testingInt[1] ? "PASS" : "FAIL*") << ": Added 15 values to dataII, forcing the size of array to increase before: "
		<< testingInt[0] << ", after: " << testingInt[1] << "\n";

	// custom class as key value
	SCustomStruct testingSCustomStruct;
	testingSCustomStruct.m_int = 5;
	testingSCustomStruct.m_string = "CS string";
	cout << "    : Created testingSCustomStruct\n";
	Map<SCustomStruct, SCustomStruct>* dataCSCS = new Map<SCustomStruct, SCustomStruct>();
	cout << "PASS: Created dataCSCS, an map<SCustomStruct,SCustomStruct>\n";
	testingBool = dataCSCS->Insert(testingSCustomStruct, testingSCustomStruct);
	cout << (testingBool ? "PASS" : "FAIL*") << ": Added testingSCustomStruct, testingSCustomStruct to dataCSCS result: " << testingBool << ", expected: 1\n";
	testingBool = dataCSCS->Insert(testingSCustomStruct, testingSCustomStruct);
	cout << (!testingBool ? "PASS" : "FAIL*") << ": Added duplicate testingSCustomStruct, testingSCustomStruct to dataCSCS result: " << testingBool << ", expected: 0\n";

	//custom struct as key and value
	CCustomClass testingCCustomClass(5, "CC String");
	cout << "    : Created testingCCustomClass\n";
	Map<CCustomClass, CCustomClass>* dataCCCC = new Map<CCustomClass, CCustomClass>();
	cout << "PASS: Created dataCCCC, an map<CCustomClass,CCustomClass>\n";
	testingBool = dataCCCC->Insert(testingCCustomClass, testingCCustomClass);
	cout << (testingBool ? "PASS" : "FAIL*") << ": Added testingCCustomClass, testingCCustomClass to dataCCCC result: " << testingBool << ", expected: 1\n";
	testingBool = dataCCCC->Insert(testingCCustomClass, testingCCustomClass);
	cout << (!testingBool ? "PASS" : "FAIL*") << ": Added duplicate testingCCustomClass, testingCCustomClass to dataCCCC result: " << testingBool << ", expected: 0\n";
	CCustomClass* CCCReslult = dataCCCC->Find(testingCCustomClass);
	testingBool = (!strcmp(CCCReslult->GetString()->c_str(), "CC String")) ? true : false;
	cout << (testingBool ? "PASS" : "FAIL*") << ": Retrieved CCustomClass from dataCCCC result: " << CCCReslult->GetString()->c_str() << ", Address: " << CCCReslult->GetString() << "\n"
		<< "    :                                    expected: CC String, Address: " << &testingString << "\n";

	// data integraty check
	cout << "    : Data integraty check: checking inserting and checking 10000 values\n    : ";
	testingBool = true;
	for (int i = 0; i < 10000; ++i)
	{
		dataII->Insert(i, i);
		testingInt[0] = *dataII->Find(i);
		if (testingInt[0] != i)
		{
			testingBool = false;
			cout << "FAULT: number: " << i << ", actual: " << testingInt[0] << "\n";
			break;
		}
		if (!(i % 1000))
			cout << i << ", ";
	}
	cout << "10000, done\n";
	cout << (testingBool ? "PASS" : "FAIL*") << ": dataII checked\n";


	testingBool = true;
	dataSS->SetSize(20480);
	cout << "    : Set dataSS to size 20,480, as size need to be 13,334 to not call size increase for 10,000\n    : ";
	for (int i = 0; i < 10000; ++i)
	{
		string search = "test" + to_string(i);
		dataSS->Insert(search, search);
		testingStringPtr = dataSS->Find(search);
		if (testingStringPtr == nullptr)
			cout << "FAIL*:Retrieved nullptr from dataSS\n";
		else
		{
			testingString = *testingStringPtr;
			if (testingString != search)
			{
				testingBool = false;
				cout << "FAULT: term: " << search << ", actual: " << testingString << "\n";
			}
			if (!(i % 1000))
				cout << i << ", ";
		}
	}
	cout << "10000, done\n";
	cout << (testingBool ? "PASS" : "FAIL*") << ": dataSS checked\n";

	
	// iterator testing
	cout << "    : Iterator test\n    : ";
	testingBool = true;
	for (auto it = dataII->Begin(); it != dataII->End(); ++it)
	{
		if (it->first < 0)
			cout << "FAIL: iterator returned: " << it->first << "\n";

		if (!(it->first % 1000))
			cout << it->first << ", ";
	}
	if (testingBool)
		cout << "10000, done\nPASS: iterated through DataII\n";

	// testing copy constructors
	cout << "    : Constructor testing\n";
	Map<int, int> dataII2;
	for(int i = 0; i < 20; ++i)
		dataII2.Insert(i,i);
	cout << "    : Created DataII2\n";

	Map<int, int> dataII3(dataII2);
	cout << "    : Created dataII3 using copy constructor with DataII2\n";
	testingInt[0] = dataII3.MaxSize();
	if (testingInt[0] == 20)
		cout << "PASS: ";
	else
		cout << "FAIL*: ";
	cout << "DataII3's size is: " << testingInt[0] << ", expected: 20 \n";

	Map<int, int> dataII4 = dataII2;
	cout << "    : Created dataII4 using = operator with DataII2\n";
	testingInt[0] = dataII4.MaxSize();
	if (testingInt[0] == 20)
		cout << "PASS: ";
	else
		cout << "FAIL*: ";
	cout << "DataII4's size is: " << testingInt[0] << ", expected: 20 \n";

	// testing erase and clear
	cout << "    : Function testing\n";
	cout << "    : Erase()\n";
	testingBool = dataII2.Erase(1);
	if (testingBool)
		cout << "PASS: ";
	else
		cout << "FAIL*: ";
	cout << "DataII2's errased key 1\n";
	testingBool = dataII2.Erase(1);
	if (!testingBool)
		cout << "PASS: ";
	else
		cout << "FAIL*: ";
	cout << "DataII2's duplicaed errased key 1 which does not exist\n";

	cout << "    : Clear()\n";
	dataII3.Clear();
	if (dataII3.Empty())
		cout << "PASS: ";
	else
		cout << "FAIL*: ";
	cout << "Cleared dataII3 current size: " << dataII3.Size() << "\n";


	// cleanup
	cout << "\nCleanup:\n";
	delete dataCCCC;
	cout << "deleted dataCCCC\n";
	delete dataCSCS;
	cout << "deleted dataCSCS\n";
	delete dataFF;
	cout << "deleted dataFF\n";
	delete dataSS;
	cout << "deleted dataSS\n";
	delete dataII;
	cout << "deleted dataII\n";

	// pause
	cout << "Paused.\n";
	system("pause");
}