#include "vmt.hpp"
#include "memory.hpp"

VMT::VMT(void **vmt)
{
	this->vmt = vmt;
	this->length = 0;

	while (this->vmt[this->length]) {
		this->orig_vmt.push_back(this->vmt[this->length]);
		++this->length;
	}

	Memory::Protect(this->vmt, this->length * sizeof(this->vmt[0]), PROT_EXEC | PROT_READ | PROT_WRITE);
}

VMT::~VMT()
{
	// TODO: Restore Memory Protection
}

void **VMT::GetVMT()
{
	return this->vmt;
}

void *VMT::GetFunction(size_t index)
{
	if (index >= this->length)
		return nullptr;
	
	return this->vmt[index];
}

void *VMT::GetOriginal(size_t index)
{
	if (index >= this->length)
		return nullptr;
	
	return this->orig_vmt[index];
}

void VMT::Hook(size_t index, void *func)
{
	if (index >= this->length)
		return;
	
	this->vmt[index] = func;
}

void VMT::Restore(size_t index)
{
	if (index >= this->length)
		return;
	
	this->vmt[index] = this->orig_vmt[index];
}

void VMT::RestoreAll()
{
	for (size_t i = 0; i < this->length; ++i)
		this->Restore(i);
}
