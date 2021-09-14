// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "Model.h"

namespace highlo
{
    Ref<Model> Model::Create()
    {
        return Ref<Model>::Create();
    }
    
    Ref<Model> Model::Create(const std::initializer_list<Ref<Mesh>> &meshes)
    {
        return Ref<Model>::Create(meshes);
    }
    
    Ref<Model> Model::Create(const std::vector<Ref<Mesh>> &meshes)
    {
        return Ref<Model>::Create(meshes);
    }

    Ref<Mesh> Model::GetMesh(uint64 idx) const
    {
        HL_ASSERT(idx < m_Meshes.size());
        return m_Meshes[idx];
    }
}
