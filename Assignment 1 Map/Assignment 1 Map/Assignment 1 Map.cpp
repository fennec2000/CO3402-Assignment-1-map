//	G20363714

#include "pch.h"
#include "map.h"
#include <iostream>
#include <string>
// memory leaks
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

using namespace std;

struct SCustomStruct
{
	int m_int;
	string m_string;
};

bool operator< (const SCustomStruct& a, const SCustomStruct& b) { return a.m_int < b.m_int || (a.m_int == b.m_int && a.m_string < b.m_string); }
bool operator== (const SCustomStruct& a, const SCustomStruct& b) { return a.m_int == b.m_int && a.m_string == b.m_string; }

class CCustomClass
{
private:
	int m_int;
	string m_string;
public:
	void Speak() { cout << m_string << "\n"; }
	const int GetInt() { return m_int; }
	void SetInt(int a) { m_int = a; }
	const string* GetString() { return &m_string; }
	void SetString(const string& a) { m_string = a; }
	bool operator< (const CCustomClass& rhs) const { return m_int < rhs.m_int || (m_int == rhs.m_int && m_string < rhs.m_string); }
	bool operator== (const CCustomClass& rhs) const { return m_int == rhs.m_int && m_string == rhs.m_string; }
};

int main()
{
	// variables used for output
	bool testingBool;
	string testingString;
	string* testingStringPtr;
	int testingInt[2];
	int* testingIntPtr;

	// Tests
	cout << "Tests:\n";
	Map<int, int>* dataII = new Map<int, int>();
	cout << "PASS: Created dataII, an map<int,int>\n";
	Map<string, string>* dataSS = new Map<string, string>();
	cout << "PASS: Created dataSS, an map<string,string>\n";

	// test adding 1,1 to <int,int>
	testingBool = dataII->Add(1, 1);
	cout << (testingBool ? "PASS" : "FAIL*") << ": Added 1, 1 to dataII result: " << testingBool << ", expected: 1\n";
	testingBool = dataII->Add(1, 1);
	cout << (!testingBool ? "PASS" : "FAIL*") << ": Added duplicate 1, 1 to dataII result: " << testingBool << ", expected: 0\n";

	// test adding "Test","Test" to <string, string>
	testingBool = dataSS->Add("TestString", "TestString");
	cout << (testingBool ? "PASS" : "FAIL*") << ": Added \"TestString\", \"TestString\" to dataSS result: " << testingBool << ", expected: 1\n";
	testingBool = dataSS->Add("TestString", "TestString");
	cout << (!testingBool ? "PASS" : "FAIL*") << ": Added duplicate \"TestString\", \"TestString\" to dataSS result: " << testingBool << ", expected: 0\n";

	// test retreving data
	testingString = (dataSS->Get("TestString") != nullptr ? *dataSS->Get("TestString") : "");
	cout << (!strcmp(testingString.c_str(), "TestString") ? "PASS" : "FAIL*") << ": Retrieved \"TestString\" from dataSS result: \"" << testingString << "\", expected: \"TestString\"\n";
	testingString = (dataSS->Get("NotInDataSS") != nullptr ? *dataSS->Get("NotInDataSS") : "");
	cout << (!strcmp(testingString.c_str(), "") ? "PASS" : "FAIL*") << ": Retrieved \"NotInDataSS\" from dataSS result: \"" << testingString << "\", expected: \"\"\n";

	// Test adding more data than map size
	testingInt[0] = dataII->Size();
	for (int i = 0; i < 15; ++i)
		dataII->Add(i, i);
	testingInt[1] = dataII->Size();
	cout << (testingInt[0] != testingInt[1] ? "PASS" : "FAIL*") << ": Added 15 values to dataII, forcing the size of array to increase before: "
		<< testingInt[0] << ", after: " << testingInt[1] << "\n";

	// custom class as key value
	SCustomStruct testingSCustomStruct;
	testingSCustomStruct.m_int = 5;
	testingSCustomStruct.m_string = "This is a string";
	cout << "    : Created testingSCustomStruct\n";
	Map<SCustomStruct, SCustomStruct>* dataCSCS = new Map<SCustomStruct, SCustomStruct>();
	cout << "PASS: Created dataCSCS, an map<SCustomStruct,SCustomStruct>\n";
	testingBool = dataCSCS->Add(testingSCustomStruct, testingSCustomStruct);
	cout << (testingBool ? "PASS" : "FAIL*") << ": Added testingSCustomStruct, testingSCustomStruct to dataCSCS result: " << testingBool << ", expected: 1\n";
	testingBool = dataCSCS->Add(testingSCustomStruct, testingSCustomStruct);
	cout << (!testingBool ? "PASS" : "FAIL*") << ": Added duplicate testingSCustomStruct, testingSCustomStruct to dataCSCS result: " << testingBool << ", expected: 0\n";

	//custom struct as key and value
	CCustomClass testingCCustomClass;
	CCustomClass* testingCCustomClassPtr;
	testingCCustomClass.SetInt(5);
	testingString = "This is a string";
	testingCCustomClass.SetString(testingString);
	cout << "    : Created testingCCustomClass\n";
	Map<CCustomClass, CCustomClass>* dataCCCC = new Map<CCustomClass, CCustomClass>();
	cout << "PASS: Created dataCCCC, an map<CCustomClass,CCustomClass>\n";
	testingBool = dataCCCC->Add(testingCCustomClass, testingCCustomClass);
	cout << (testingBool ? "PASS" : "FAIL*") << ": Added testingCCustomClass, testingCCustomClass to dataCCCC result: " << testingBool << ", expected: 1\n";
	testingBool = dataCCCC->Add(testingCCustomClass, testingCCustomClass);
	cout << (!testingBool ? "PASS" : "FAIL*") << ": Added duplicate testingCCustomClass, testingCCustomClass to dataCCCC result: " << testingBool << ", expected: 0\n";

	testingCCustomClassPtr = dataCCCC->Get(testingCCustomClass);
	testingBool = (!strcmp(testingCCustomClassPtr->GetString()->c_str(), testingString.c_str())) ? true : false;
	cout << (testingBool ? "PASS" : "FAIL*") << ": Retrieved CCustomClass from dataCCCC result: "
		<< *testingCCustomClassPtr->GetString() <<", Address: " << testingCCustomClassPtr << "\n"
		<< "    :                                    expected: " << testingString << ", Address: " << &testingString << "\n";

	// data integraty check
	cout << "    : Data integraty check: checking inserting and checking 10000 values\n    : ";
	testingBool = true;
	for (int i = 0; i < 10000; ++i)
	{
		dataII->Add(i, i);
		testingIntPtr = dataII->Get(i);
		testingInt[0] = (testingIntPtr == nullptr) ? -1 : *dataII->Get(i);
		if (testingInt[0] != i)
		{
			testingBool = false;
			cout << "FAULT: number: " << i << ", actual: " << testingInt[0] << "\n";
			break;
		}
		if (!(i % 1000))
			cout << i << ", ";
	}
	cout << "done 10000\n";
	cout << (testingBool ? "PASS" : "FAIL*") << ": dataII checked\n";
	testingBool = true;
	dataSS->SetSize(20480);
	cout << "    : Set dataSS to size 20,480, as size need to be 13,334 to not call size increase for 10,000\n    : ";
	for (int i = 0; i < 10000; ++i)
	{
		string search = "test" + to_string(i);
		dataSS->Add(search, search);
		testingStringPtr = dataSS->Get(search);
		testingString = (testingStringPtr != nullptr ? *testingStringPtr : "");
		if (testingString != search)
		{
			testingBool = false;
			cout << "FAULT: term: " << search << ", actual: " << testingString << "\n";
		}
		if (!(i % 1000))
			cout << i << ", ";
	}
	cout << "done 10000\n";
	cout << (testingBool ? "PASS" : "FAIL*") << ": dataSS checked\n";
	
	// iterator test
	cout << "    : Iterating through dataII\n";
	testingBool = true;
	cout << "    : ";
	for (auto it = dataII->Begin(); it != dataII->End(); ++it)
	{
		if (it->first < 0 || it->second < 0)
		{
			testingBool = false;
			cout << "Uninitialised data found, " << it->first << ", " << it->second << "\n";
		}
		if (!(it->first % 1000))
			cout << it->first << ", ";
	}
	cout << "done 10000\n";
	if (testingBool)
		cout << "PASS: Iterated through dataII\n";
	else
		cout << "FAIL*: Iterated through dataII\n";


	// cleanup
	cout << "\nCleanup:\n";
	delete dataCCCC;
	cout << "deleted dataCCCC\n";
	delete dataCSCS;
	cout << "deleted dataCSCS\n";
	delete dataSS;
	cout << "deleted dataSS\n";
	delete dataII;
	cout << "deleted dataII\n";

	// pause
	cout << "Paused.\n";
	system("pause");
	_CrtDumpMemoryLeaks();
	return 0;
}
