#pragma once

template <typename T> class TSingleton
{
public:
	static T& GetInstance()
	{
		if (_instance == nullptr)
		{
			_instance = new T();
		}
		return *_instance;
	}
private:
	static T* _instance;
};

template <typename T> T* TSingleton<T>::_instance = nullptr;