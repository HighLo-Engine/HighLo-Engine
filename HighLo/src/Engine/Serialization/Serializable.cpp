// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "Serializable.h"

#include "Serializer.h"

namespace highlo
{
    Serializable::Serializable()
    {
        Serializer::RegisterSerializable(this);
    }

    Serializable::~Serializable()
    {
        Serializer::UnregisterSerializable(this);
    }
}

