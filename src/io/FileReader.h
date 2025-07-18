#ifndef FILE_READER_H
#define FILE_READER_H

class FileReader {
	public:
		FileReader();
		~FileReader();
		int readFile();
	private:
		int readLargeFile();
		int readSmallFile();

};

#endif