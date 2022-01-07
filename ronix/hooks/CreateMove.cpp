#include <ronix.hpp>

typedef void (*CreateMoveFn)(IBaseClientDLL *thisptr, int sequence_number, float input_sample_frametime, bool active);

void Ronix::Hooks::CreateMove(IBaseClientDLL *thisptr, int sequence_number, float input_sample_frametime, bool active)
{
	static CreateMoveFn fnCreateMove = Ronix::Data::BaseClientDllVmt->GetOriginal<CreateMoveFn>(21);
	return fnCreateMove(thisptr, sequence_number, input_sample_frametime, active);
}
