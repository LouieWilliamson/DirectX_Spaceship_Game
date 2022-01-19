#pragma once

#define _XM_NO_INTRINSICS_
#define XM_NO_ALIGNMENT

#include <d3d11.h>
#include <d3dx11.h>
#include <dxerr.h>
#include <DirectXMath.h>

#include <vector>
#include <string>
#include <fstream>


using namespace std;

class File
{
public:
	File(const char* fileName);
	void Read();
	void Write(std::string LineToWrite);
	void WriteInt(int hScore);
	void Clear();
	void SortLines();
	~File();
	vector<string> fileLines;
private:
	const char* Filename = "";

};

