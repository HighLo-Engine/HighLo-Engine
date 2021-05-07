#include "HighLoPch.h"
#include "Model.h"

namespace highlo
{
	void Model::SetTexture(int type, const Ref<Texture>& texture)
	{
		for (auto& mesh : m_Meshes)
			mesh->m_Material->SetTexture(type, texture);
	}
}
