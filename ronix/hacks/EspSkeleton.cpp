#include <ronix.hpp>

using Ronix::Data::cstrike;

#define _B(b) PlayerData::BoneIndex::b
#define DEBUG cstrike->Cvar->ConsolePrintf

void Ronix::Hacks::EspSkeleton(PlayerData *player)
{
	auto draw_list = ImGui::GetBackgroundDrawList();
	static const std::vector<int> boneIndices = {
		_B(NECK), _B(CHEST),
		_B(LSHOULDER), _B(LELBOW), _B(LHAND), _B(LELBOW), _B(LSHOULDER), _B(CHEST),
		_B(RSHOULDER), _B(RELBOW), _B(RHAND), _B(RELBOW), _B(RSHOULDER), _B(CHEST),
		_B(CORE),
		_B(LPELVIS), _B(LKNEE), _B(LANKLE), _B(LFOOT), _B(LANKLE), _B(LKNEE), _B(LPELVIS), _B(CORE),
		_B(RPELVIS), _B(RKNEE), _B(RANKLE), _B(RFOOT), _B(RANKLE), _B(RKNEE), _B(RPELVIS), _B(CORE)
	};


	ImVec2 points[2];
	for (size_t i = 0; i < boneIndices.size() - 1; ++i) {
		for (size_t j = 0; j < 2; ++j) {
			Vector &bonePos = player->boneMatrixPos2d[boneIndices[i + j]];
			points[j] = ImVec2(bonePos.x, bonePos.y);
		}

		draw_list->AddLine(points[0], points[1], ImColor(255.0f, 0.0f, 0.0f, 255.0f));
	}
}
