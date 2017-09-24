#pragma once
#include <string>
#include <iostream>
#include <algorithm>
#include <sstream>
#include <iterator>

/*
  Anything that will go in a resource manager
  must inherit from this.
  Implements functions required for the managers.
*/
class Resource
{
public:

	template <class T> friend class  ResourceManager;

	Resource(std::string resourcename, size_t resourcesize);
	Resource() {}

	virtual ~Resource() {}

	virtual void SetName(std::string name);
	virtual void SetSizeInBytes(size_t size);

	virtual std::string GetName();
	virtual size_t GetSizeInBytes();
	virtual int GetReferencesCount();

	virtual void Read(std::string resourcename)
	{}
	virtual void ReadParams(std::string params)
	{}

protected:
	Resource(const Resource& object) {}

	Resource& operator=(const Resource& object)
	{
		return *this;
	}

private:
	int references;

	std::string name;
	size_t size;

	void IncReferences();
	void DecReferences();
};

