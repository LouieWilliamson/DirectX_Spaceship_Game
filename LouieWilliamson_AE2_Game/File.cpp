#include "File.h"



File::File(const char* fileName)
{
	Filename = fileName;
}

//Reads the file places each line into the vector
void File::Read()
{
	//Open input stream
	std::ifstream fileToRead(Filename);

	//Clear file lines vector
	fileLines.clear();

	int index = 0;
	std::string currentLine;

	//if file has correctly opened
	if (fileToRead.is_open())
	{
		// until the end of file (EOF) is reached
		while (!fileToRead.eof())
		{
			std::getline(fileToRead, currentLine); // extract one line of file into currentline

			if (currentLine != "")
			{
				fileLines.push_back(currentLine); //put currentline int vector
				index++;
			}
		}
		fileToRead.close(); // always close streams once finished
	}
	else
	{
		OutputDebugString("Error opening file for input\n");
	}

}
//Write a string to the file
void File::Write(std::string LineToWrite)
{
	//reads the file, to make sure I can write it all back in
	Read();

	//Opens the file for writing, also clears the file
	std::ofstream fileToWrite(Filename);

	//If the file has been opened successfully
	if (fileToWrite.is_open())
	{
		//copy previous file contents into new file write

		for (int i = 0; i < fileLines.size(); i++)
		{
			fileToWrite << fileLines[i] << std::endl;
			printf("Writing Line: %s to file\n", fileLines[i].c_str());
		}

		//Write new string parameter to file 
		fileToWrite << LineToWrite;

		fileToWrite.close(); //Close file stream
		printf("Writing Done\n");
	}
	else
	{
		printf("Error opening file for output\n");
	}
}
//Writes an int to the file
void File::WriteInt(int hScore)
{
	//Opens the file for writing, also clears the file
	std::ofstream fileToWrite(Filename);

	//If the file has been opened successfully
	if (fileToWrite.is_open())
	{
		//Write new int parameter to file 
		fileToWrite << hScore;

		fileToWrite.close(); //Close file stream
	}
	else
	{
		OutputDebugString("Error opening file for output\n");
	}
}


//Clears the file
void File::Clear()
{
	std::ofstream fileToWrite(Filename);
	fileToWrite.close(); //Close file stream
}

void File::SortLines()
{
}


File::~File()
{
}
