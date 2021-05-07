#include "HighLoPch.h"
#include "Model.h"

namespace highlo
{
	void Model::AddTexture(const Ref<Texture>& texture)
	{
		for (auto& mesh : m_Meshes)
			mesh->material->AddTexture(texture);
	}
}
