#include <Helium/Core/HeFile.h>

namespace ArtificialNature {

	HeFile::HeFile()
		: m_pFileStream(nullptr)
	{
		m_pFileStream = new fstream();
	}

	HeFile::~HeFile()
	{
		if (m_pFileStream != nullptr) {
			delete m_pFileStream;
		}
	}

	void HeFile::Create(string fileName, bool isBinary)
	{
		if (isBinary) {
			(*m_pFileStream).open(fileName, ios::binary | ios::in | ios::out | ios::trunc);
		}
		else {
			(*m_pFileStream).open(fileName, ios::in | ios::out | ios::trunc);
		}
	}

	bool HeFile::Open(string fileName, bool isBinary)
	{
		if (isBinary) {
			(*m_pFileStream).open(fileName, ios::binary | ios::in);
		}
		else {
			(*m_pFileStream).open(fileName, ios::in);
		}

		if ((*m_pFileStream).is_open()) {
			(*m_pFileStream).seekg(0, ios::end);
			m_fileLength = (*m_pFileStream).tellg();
			(*m_pFileStream).seekg(0, ios::beg);

			return true;
		}

		return false;
	}

	void HeFile::Close()
	{
		(*m_pFileStream).close();
	}

	bool HeFile::isOpen()
	{
		return (*m_pFileStream).is_open();
	}

	bool HeFile::GetWord(string& word)
	{
		return !((*m_pFileStream) >> word).eof();
	}

	bool HeFile::GetLine(string& line)
	{
		return !(getline((*m_pFileStream), line).eof());
	}

	void HeFile::Read(char* buffer, int length)
	{
		(*m_pFileStream).read(buffer, length);
	}

	void HeFile::Write(char* buffer, int length)
	{
		(*m_pFileStream).write(buffer, length);
	}

	fstream& HeFile::operator << (bool data)
	{
		return (fstream&)((*m_pFileStream) << data);
	}

	fstream& HeFile::operator << (short data)
	{
		return (fstream&)((*m_pFileStream) << data);
	}

	fstream& HeFile::operator << (unsigned short data)
	{
		return (fstream&)((*m_pFileStream) << data);
	}

	fstream& HeFile::operator << (int data)
	{
		return (fstream&)((*m_pFileStream) << data);
	}

	fstream& HeFile::operator << (unsigned int data)
	{
		return (fstream&)((*m_pFileStream) << data);
	}

	fstream& HeFile::operator << (long data)
	{
		return (fstream&)((*m_pFileStream) << data);
	}

	fstream& HeFile::operator << (unsigned long data)
	{
		return (fstream&)((*m_pFileStream) << data);
	}

	fstream& HeFile::operator << (float data)
	{
		return (fstream&)((*m_pFileStream) << data);
	}

	fstream& HeFile::operator << (double data)
	{
		return (fstream&)((*m_pFileStream) << data);
	}

	fstream& HeFile::operator << (string& data)
	{
		return (fstream&)((*m_pFileStream) << data);
	}

	fstream& HeFile::operator << (const string& data)
	{
		return (fstream&)((*m_pFileStream) << data);
	}

	fstream& HeFile::operator << (char* data)
	{
		return (fstream&)((*m_pFileStream) << data);
	}

	fstream& HeFile::operator << (const char* data)
	{
		return (fstream&)((*m_pFileStream) << data);
	}

	//

	fstream& HeFile::operator >> (bool& data)
	{
		return (fstream&)((*m_pFileStream) >> data);
	}

	fstream& HeFile::operator >> (short& data)
	{
		return (fstream&)((*m_pFileStream) >> data);
	}

	fstream& HeFile::operator >> (unsigned short& data)
	{
		return (fstream&)((*m_pFileStream) >> data);
	}

	fstream& HeFile::operator >> (int& data)
	{
		return (fstream&)((*m_pFileStream) >> data);
	}

	fstream& HeFile::operator >> (unsigned int& data)
	{
		return (fstream&)((*m_pFileStream) >> data);
	}

	fstream& HeFile::operator >> (long& data)
	{
		return (fstream&)((*m_pFileStream) >> data);
	}

	fstream& HeFile::operator >> (unsigned long& data)
	{
		return (fstream&)((*m_pFileStream) >> data);
	}

	fstream& HeFile::operator >> (float& data)
	{
		return (fstream&)((*m_pFileStream) >> data);
	}

	fstream& HeFile::operator >> (double& data)
	{
		return (fstream&)((*m_pFileStream) >> data);
	}

	fstream& HeFile::operator >> (string& data)
	{
		return (fstream&)((*m_pFileStream) >> data);
	}

	//fstream& HeFile::operator >> (char* data)
	//{
	//	return (fstream&)((*m_pFileStream) >> data);
	//}

}
