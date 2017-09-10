#pragma once

class cObject
{
private:
	int m_nRefCount;

public:
	cObject();
	virtual ~cObject();

	void AddRef();
	void Release();
};

