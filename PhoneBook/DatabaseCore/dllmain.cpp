// dllmain.cpp : Определяет точку входа для приложения DLL.
#include "pch.h"
#include "core.h"

int currentOffset = 0, totalBytesRead = 0;
const int frameSize = 65536;
LPSTR fileStart = NULL, currPointer = NULL;
DWORD fileSize;
HANDLE file, fileMapping;
SYSTEM_INFO sysinfo = { 0 };

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH: {
        int offset = 0;
        GetSystemInfo(&sysinfo);
        LPCWSTR fileName = L"..\\addresses.txt";
        LPCWSTR name = L"TelephoneBook";
        file = CreateFile(fileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
        fileSize = GetFileSize(file, NULL);
        fileMapping = CreateFileMapping(file, NULL, PAGE_READONLY, 0, 0, name);
        if (fileMapping == NULL) {
            return -1;
        }
        updatePointer(0);
        currPointer = fileStart;
    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

string getLine()
{
    int i = 0, prevLength = 0;
    char* str = new char[500];
    while (fileStart[currentOffset + i] != '\n' && totalBytesRead < fileSize) {
        str[i] = fileStart[currentOffset + i];
        i++;
        totalBytesRead++;
        if ((currentOffset + i) % frameSize == 0) {
            updatePointer(currentOffset + i);//65536
            prevLength = i;
            i = 0;
            currentOffset = 0;
        }
    }
    if (str[prevLength + i - 1] == '\r')
        str[prevLength + i - 1] = '\0';
    else
        str[prevLength + i] = '\0';
    currentOffset = i + currentOffset + 1;
    return string(str);
}

vector<string> split(string str, char separator) {
    vector<string> substrings;
    int startPos = 0;
    int endPos = str.find(separator);
    while (endPos != -1) {
        substrings.push_back(str.substr(startPos, endPos - startPos));
        startPos = endPos + 1;
        endPos = str.find(separator, startPos);
    }
    substrings.push_back(str.substr(startPos, str.length() - startPos));
    return substrings;
}

DATABASE_API vector<Record*> __cdecl search(Record searchVals) {
    vector<Record*> v;
    totalBytesRead -= currentOffset;
    if (totalBytesRead < 0)
        totalBytesRead = 0;
    currentOffset = 0;
    while (v.size() < 15 && totalBytesRead < fileSize) {//||
        Record r = getNext();
        if (paramsEquals(searchVals.fathersName, r.fathersName) &&
            paramsEquals(searchVals.name, r.name) &&
            paramsEquals(searchVals.surname, r.surname) &&
            paramsEquals(searchVals.phoneNumber, r.phoneNumber) &&
            paramsEquals(searchVals.street, r.street) &&
            paramsEquals(searchVals.flatNumber, r.flatNumber) &&
            paramsEquals(searchVals.blockNumber, r.blockNumber) &&
            paramsEquals(searchVals.houseNumber, r.houseNumber)) {
            Record* tmp = new Record(r);
            v.push_back(tmp);
        }

    }
    return v;
}

bool paramsEquals(string expected, string actual) { ///!expected.empty() && actual.compare(expected)==0
    if (expected.empty())
        return true;
    return actual.compare(expected)==0;
}

bool paramsEquals(int expected, int actual) { //expected != 0&&expected == actual
    if (expected == 0)
        return true;
    return expected == actual;
}
Record getNext()
{
    string str = getLine();
    vector<string> params = split(str);
    return Record(params[0],
        params[1],
        params[2],
        params[3],
        params[4],
        stoi(params[5]),
        stoi(params[6]),
        stoi(params[7])
    );
}

DATABASE_API vector<Record*> __cdecl loadDB() {
    vector<Record*> records;
    for (int i = 0; i < 15; i++) {
        Record* r = new Record(getNext());
        records.push_back(r);
    }
    return records;
}

void updatePointer(int offset) {
    if (fileStart != NULL)
        UnmapViewOfFile(fileStart);
    fileStart = (LPSTR)MapViewOfFile(fileMapping, FILE_MAP_READ, 0, offset, sysinfo.dwAllocationGranularity);
    if (fileStart == NULL) {
        fileStart = (LPSTR)MapViewOfFile(fileMapping, FILE_MAP_READ, 0, offset, 0);
    }
}
