#ifndef VMT_HPP
#define VMT_HPP

#include <pch.hpp>

class VmtMgr {
private:
	void **vmt;
	size_t length;
	std::vector<void *> orig_vmt;
public:
	VmtMgr(void **vmt);
	~VmtMgr();
	void **GetVmt();
	void *GetFunction(size_t index);
	void *GetOriginal(size_t index);
	void Hook(size_t index, void *func);
	void Restore(size_t index);
	void RestoreAll();

	template <typename T>
	inline T GetFunction(size_t index)
	{
		return reinterpret_cast<T>(this->GetFunction(index));
	}
	template <typename T>
	inline T GetOriginal(size_t index)
	{
		return reinterpret_cast<T>(this->GetOriginal(index));
	}
	template <typename T>
	inline void Hook(size_t index, T func)
	{
		return this->Hook(index, reinterpret_cast<void *>(func));
	}
};

#endif
