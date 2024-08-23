#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "Log.h"

class FileManager
{
private:
	std::string filePath;
	std::streampos offset;
	bool autoMove;
public:
	FileManager(std::string path);
	FileManager(const FileManager& manager);
	FileManager();
	~FileManager();

	void Delete();
	void Rename(std::string newPath);
	void ChangeFile(std::string newPath);
	void Clear(bool isBinary);
	void BinAutoMoveStreamPos(bool x);
	void SetStreamPos(std::streampos x);
	std::streampos GetStreamPos();
	//text files
	template<typename T>
	bool Write(const T data) {
		std::ofstream file(filePath.c_str());
		if (!file.is_open()) {
			LOG("Error opening file: " + filePath);
			return false;
		}
		file << data;
		file.close();
		return true;
	}
	template<typename T>
	bool WriteAppend(const T data) {
		std::ofstream file(filePath.c_str(), std::ios::app);
		if (!file.is_open()) {
			LOG("Error opening file: " + filePath);
			return false;
		}
		file << data;
		file.close();
		return true;
	}
	std::string Read();
	std::string Read(int lineID);
	//binary files
	template<typename T>
	bool WriteBinary(std::vector<T>& data) {
		std::ofstream file(filePath.c_str(), std::ios::binary);
		if (!file.is_open()) {
			LOG("Error opening file: " + filePath);
			return false;
		}
		for (int i = 0; i < data.size(); i++)
			file.write((char*)&data[i], sizeof(T));
		file.close();
		return true;
	}
	template<typename T>
	bool WriteBinary(T& data) {
		std::ofstream file(filePath.c_str(), std::ios::binary);
		if (!file.is_open()) {
			LOG("Error opening file: " + filePath);
			return false;
		}
		file.write((char*)&data, sizeof(T));
		file.close();
		return true;
	}
	template<typename T>
	bool WriteBinary(T* data, int n) {
		std::ofstream file(filePath.c_str(), std::ios::binary);
		if (!file.is_open()) {
			LOG("Error opening file: " + filePath);
			return false;
		}
		for (int i = 0; i < n; i++) {
			file.write(reinterpret_cast<char*>(data + i), sizeof(T));
			LOG(*(data + i));
			LOG(std::endl);
		}
		file.close();
		return true;
	}
	template<typename T>
	bool WriteAppendBinary(T& data) {
		std::ofstream file(filePath.c_str(), std::ios::binary | std::ios::app);
		if (!file.is_open()) {
			LOG("Error opening file: " + filePath);
			return false;
		}
		file.write((char*)&data, sizeof(T));
		file.close();
		return true;
	}
	template<typename T>
	bool WriteAppendBinary(T* data, int n) {
		std::ofstream file(filePath.c_str(), std::ios::binary | std::ios::app);
		if (!file.is_open()) {
			LOG("Error opening file: " + filePath);
			return false;
		}
		for (int i = 0; i < n; i++) {
			file.write(reinterpret_cast<char*>(data + i), sizeof(T));
			LOG(*(data + i));
			LOG(std::endl);
		}
		file.close();
		return true;
	}
	template<typename T>
	bool ReadBinary(std::vector<T>& data) {
		std::ifstream file(filePath, std::ios::binary);
		if (!file.is_open()) {
			LOG("Error opening file: " + filePath);
			return false;
		}
		if (autoMove) {
			file.seekg(offset, std::ios::beg);
			offset += sizeof(T) * data.size();
		}
		T object;
		while (file.read(reinterpret_cast<char*>(&object), sizeof(T))) {
			data.push_back(object);
		}
		file.close();

		return true;
	}
	template<typename T>
	bool ReadBinary(T* data, size_t recordNumber) {
		std::ifstream file(filePath, std::ios::binary);
		if (!file.is_open()) {
			LOG("Error opening file: " + filePath);
			return false;
		}

		std::streampos offset = static_cast<std::streampos>(recordNumber) * sizeof(T);
		file.seekg(offset, std::ios::beg);

		file.read(reinterpret_cast<char*>(data), sizeof(T));
		if (file.gcount() != static_cast<std::streamsize>(sizeof(T))) {
			LOG("Error reading record from file: " + filePath);
			return false;
		}

		file.close();
		return true;
	}
	template<typename T>
	bool ReadBinary(T* data) {
		std::ifstream file(filePath, std::ios::binary);
		if (!file.is_open()) {
			LOG("Error opening file: " + filePath);
			return false;
		}
		if (autoMove) {
			file.seekg(offset, std::ios::beg);
			offset += sizeof(T);
		}
		file.read(reinterpret_cast<char*>(data), sizeof(T));
		if (!file) {
			LOG("Error reading record from file: " + filePath);
			return false;
		}
		file.close();
		return true;
	}
	template<typename T>
	bool ReadBinary(T* data, int n) {
		std::ifstream file(filePath, std::ios::binary);
		if (!file.is_open()) {
			LOG("Error opening file: " + filePath);
			return false;
		}
		T object;
		if (autoMove) {
			file.seekg(offset, std::ios::beg);
		}
		for (int i = 0; i < n; i++) {
			file.read(reinterpret_cast<char*>(&object), sizeof(T));
			*(data + i) = object;
			if (autoMove) {
				offset += sizeof(T);
			}
		}
		file.close();

		return true;
	}
};

