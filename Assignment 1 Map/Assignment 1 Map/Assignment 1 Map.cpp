//	G20363714

#include "pch.h"
#include "map.h"
#include <iostream>
#include <string>

using namespace std;

struct SCustomStruct
{
	int m_int;
	string m_string;
};

bool operator< (const SCustomStruct& a, const SCustomStruct& b) { return a.m_int < b.m_int || (a.m_int == b.m_int && a.m_string < b.m_string); }
bool operator== (const SCustomStruct& a, const SCustomStruct& b) { return a.m_int == b.m_int && a.m_string == b.m_string; }
bool operator!= (const SCustomStruct& a, const SCustomStruct& b) { return a.m_int != b.m_int || a.m_string != b.m_string; }

class CCustomClass
{
private:
	string m_string = "Default CCustomClass Test String";
public:
	void SetMessage(string s) { m_string = s; }
	void Speak() { cout << m_string << "\n"; }
};

int main()
{
	// variables used for output
	bool testingBool;
	string testingString;
	int testingInt[2];

	// Tests
	cout << "Tests:\n";
	map<int, int>* dataII = new map<int, int>();
	cout << "PASS: Created dataII, an map<int,int>\n";
	map<string, string>* dataSS = new map<string, string>();
	cout << "PASS: Created dataSS, an map<string,string>\n";

	// test adding 1,1 to <int,int>
	testingBool = dataII->Add(1, 1);
	cout << (testingBool ? "PASS" : "*FAIL*") << ": Added 1, 1 to dataII result: " << testingBool << ", expected: 1\n";
	testingBool = dataII->Add(1, 1);
	cout << (!testingBool ? "PASS" : "*FAIL*") << ": Added duplicate 1, 1 to dataII result: " << testingBool << ", expected: 0\n";

	// test adding "Test","Test" to <string, string>
	testingBool = dataSS->Add("Test", "Test");
	cout << (testingBool ? "PASS" : "*FAIL*") << ": Added \"Test\", \"Test\" to dataSS result: " << testingBool << ", expected: 1\n";
	testingBool = dataSS->Add("Test", "Test");
	cout << (!testingBool ? "PASS" : "*FAIL*") << ": Added duplicate \"Test\", \"Test\" to dataSS result: " << testingBool << ", expected: 0\n";

	// test retreving data
	testingString = dataSS->Get("Test");
	cout << (testingString == "Test" ? "PASS" : "*FAIL*") << ": Retrieving \"Test\" from dataSS result: \"" << testingString << "\", expected: \"Test\"\n";
	testingString = dataSS->Get("NotInDataSS");
	cout << (testingString == "" ? "PASS" : "*FAIL*") << ": Retrieving \"NotInDataSS\" from dataSS result: \"" << testingString << "\", expected: \"\"\n";

	// Test adding more data than map size
	testingInt[0] = dataII->Size();
	for (int i = 0; i < 15; ++i)
		dataII->Add(i, i);
	testingInt[1] = dataII->Size();
	cout << (testingInt[0] != testingInt[1] ? "PASS" : "*FAIL*") << ": Added 15 values to dataII, forcing the size of array to increase before: "
		<< testingInt[0] << ", after: " << testingInt[1] << "\n";

	// data integraty check
	cout << "Data integraty check: checking inserting and checking 10000 values\n";
	testingBool = true;
	for (int i = 0; i < 10000; ++i)
	{
		dataII->Add(i, i);
		testingInt[0] = dataII->Get(i);
		if (testingInt[0] != i)
		{
			testingBool = false;
			cout << "*FAULT*: number: " << i << ", actual: " << testingInt[0] << "\n";
			break;
		}
	}
	cout << (testingBool ? "PASS" : "*FAIL*") << ": dataII checked\n";
	testingBool = true;
	for (int i = 0; i < 10000; ++i)
	{
		string search = "test" + to_string(i);
		dataSS->Add(search, search);
		testingString = dataSS->Get(search);
		if (testingString != search)
		{
			testingBool = false;
			cout << "*FAULT*: term: " << search << ", actual: " << testingString << "\n";
		}
	}
	cout << (testingBool ? "PASS" : "*FAIL*") << ": dataSS checked\n";


	//custom struct as key and value
	SCustomStruct testingSCustomStruct;
	testingSCustomStruct.m_int = 5;
	testingSCustomStruct.m_string = "This is a string";
	cout << "Created testingSCustomStruct\n";
	map<SCustomStruct, SCustomStruct>* dataCSCS = new map<SCustomStruct, SCustomStruct>();
	cout << "PASS: Created dataCSCS, an map<SCustomStruct,SCustomStruct>\n";
	testingBool = dataCSCS->Add(testingSCustomStruct, testingSCustomStruct);
	cout << (testingBool ? "PASS" : "*FAIL*") << ": Added testingSCustomStruct, testingSCustomStruct to dataCSCS result: " << testingBool << ", expected: 1\n";
	testingBool = dataCSCS->Add(testingSCustomStruct, testingSCustomStruct);
	cout << (!testingBool ? "PASS" : "*FAIL*") << ": Added duplicate testingSCustomStruct, testingSCustomStruct to dataCSCS result: " << testingBool << ", expected: 0\n";




	// cleanup
	cout << "\nCleanup:\n";
	/*delete dataCSCS;
	cout << "deleted dataCSCS\n";*/
	delete dataSS;
	cout << "deleted dataSS\n";
	delete dataII;
	cout << "deleted dataII\n";

	// pause
	cout << "Paused.\n";
	system("pause");
	return 0;
}
