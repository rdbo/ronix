#include "vmtmgr.hpp"
#include "memory.hpp"

VmtMgr::VmtMgr(void **vmt)
{
	this->vmt = vmt;
	this->length = 0;

	while (this->vmt[this->length]) {
		this->orig_vmt.push_back(this->vmt[this->length]);
		++this->length;
	}

	Memory::Protect(this->vmt, this->length * sizeof(this->vmt[0]), PROT_EXEC | PROT_READ | PROT_WRITE);
}

VmtMgr::~VmtMgr()
{
	
}

void **VmtMgr::GetVmt()
{
	return this->vmt;
}

void *VmtMgr::GetFunction(size_t index)
{
	if (index >= this->length)
		return nullptr;
	
	return this->vmt[index];
}

void *VmtMgr::GetOriginal(size_t index)
{
	if (index >= this->length)
		return nullptr;
	
	return this->orig_vmt[index];
}

void VmtMgr::Hook(size_t index, void *func)
{
	if (index >= this->length)
		return;
	
	this->vmt[index] = func;
}

void VmtMgr::Restore(size_t index)
{
	if (index >= this->length)
		return;
	
	this->vmt[index] = this->orig_vmt[index];
}

void VmtMgr::RestoreAll()
{
	for (size_t i = 0; i < this->length; ++i)
		this->Restore(i);
}
