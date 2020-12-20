#pragma once

#ifdef SOME_EXPORTS
#define DATABASE_API  __declspec(dllimport)
#else
#define DATABASE_API __declspec(dllexport)
#endif

#include <windows.h>
#include <vector>
#include "Record.h"
using namespace std;

string getLine();
LPWSTR ConvertToLPWSTR(string);
vector<string> split(string, char = ' ');
DATABASE_API vector<Record*> __cdecl loadDB();
DATABASE_API vector<Record*> __cdecl search(Record);
Record getNext();
void updatePointer(int);
bool paramsEquals(string, string);
bool paramsEquals(int, int);
