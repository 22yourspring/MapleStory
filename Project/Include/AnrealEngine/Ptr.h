#pragma once

template<typename T>

class Ptr 
{
private:
	T*		m_pRes;

public:
	// type conversion
	operator T* ()
	{
		return m_pRes;
	}

	T* operator ->()
	{
		return m_pRes;
	}

	void operator = (T* _pRes)
	{
		if (nullptr != m_pRes)
		{
			m_pRes->SubRef();
		}

		m_pRes = _pRes;

		if (nullptr != _pRes)
		{
			m_pRes->AddRef();
		}
	}

public:
	Ptr() : m_pRes(NULL)
	{}
	Ptr(T* _pRes)
		: m_pRes(_pRes)
	{
		if (NULL != m_pRes)
		{
			m_pRes->AddRef();
		}
	}
	~Ptr()
	{
		if (NULL != m_pRes)
		{
	//		m_pRes->SubRef();
		}
	}
};