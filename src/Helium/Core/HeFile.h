#pragma once

#include <Helium/Core/HeliumCommon.h>

namespace ArtificialNature {

	class HeFile
	{
	public:
		HeFile();
		~HeFile();

		static bool Exists(const string& filename);

		void Create(string fileName, bool isBinary);
		bool Open(string fileName, bool isBinary);
		void Close();
		bool isOpen();

		bool GetWord(string& word);
		bool GetLine(string& line);

		void Read(char* buffer, int length);
		void Write(char* buffer, int length);

		fstream& operator << (bool data);
		fstream& operator << (short data);
		fstream& operator << (unsigned short data);
		fstream& operator << (int data);
		fstream& operator << (unsigned int data);
		fstream& operator << (long data);
		fstream& operator << (unsigned long data);
		fstream& operator << (float data);
		fstream& operator << (double data);
		fstream& operator << (string& data);
		fstream& operator << (const string& data);
		fstream& operator << (char* data);
		fstream& operator << (const char* data);

		fstream& operator >> (bool& data);
		fstream& operator >> (short& data);
		fstream& operator >> (unsigned short& data);
		fstream& operator >> (int& data);
		fstream& operator >> (unsigned int& data);
		fstream& operator >> (long& data);
		fstream& operator >> (unsigned long& data);
		fstream& operator >> (float& data);
		fstream& operator >> (double& data);
		fstream& operator >> (string& data);
		//fstream& operator >> (char* data);

		inline int GetFileLength() const { return m_fileLength; }

	private:
		fstream* m_pFileStream = nullptr;
		string m_stlFileName = "";
		int m_fileLength = 0;
	};
}
