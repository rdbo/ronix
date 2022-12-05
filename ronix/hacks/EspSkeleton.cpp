#include <ronix.hpp>

using Ronix::Data::cstrike;

#define _B(b) PlayerData::BoneIndex::b

void Ronix::Hacks::EspSkeleton(PlayerData *player)
{
	if (!player->isBoneMatrixSet)
		return;

	auto draw_list = ImGui::GetBackgroundDrawList();
	static const std::vector<std::vector<int>> boneDrawList = {
		{ _B(NECK), _B(CHEST) },
		{ _B(CHEST), _B(LSHOULDER), _B(LELBOW), _B(LHAND) },
		{ _B(CHEST), _B(RSHOULDER), _B(RELBOW), _B(RHAND) },
		{ _B(CHEST), _B(CORE) },
		{ _B(CORE), _B(LPELVIS), _B(LKNEE), _B(LANKLE), _B(LFOOT) },
		{ _B(CORE), _B(RPELVIS), _B(RKNEE), _B(RANKLE), _B(RFOOT) }
	};


	ImVec2 points[2];
	for (size_t i = 0; i < boneDrawList.size(); ++i) {
		for (size_t j = 0; j < boneDrawList[i].size() - 1; ++j) {
			for (size_t k = 0; k < 2; ++k) {
				Vector &bonePos = player->boneMatrixPos2d[boneDrawList[i][j + k]];
				points[k] = ImVec2(bonePos.x, bonePos.y);
			}

			draw_list->AddLine(points[0], points[1], ImColor(255.0f, 0.0f, 0.0f, 255.0f));
		}
	}
}
