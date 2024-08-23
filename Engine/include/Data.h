#pragma once

#include "FileManager.h"

class Data
{
public:
	virtual void SetFile(std::string);
	virtual void Save() = 0;
	virtual void Load() = 0;
protected:
	FileManager m_manager;
private:
	std::string m_filePath;
};

