/**
 *
 *    Copyright (c) 2020-2021 Project CHIP Authors
 *    Copyright (c) 2018 Google LLC.
 *    Copyright (c) 2016-2017 Nest Labs, Inc.
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */

#include "AttributePaths.h"
#include "MessageDefHelper.h"

#include <inttypes.h>
#include <stdarg.h>
#include <stdio.h>

#include <app/AppBuildConfig.h>

namespace chip {
namespace app {
#if CHIP_CONFIG_IM_ENABLE_SCHEMA_CHECK
CHIP_ERROR AttributePaths::Parser::CheckSchemaValidity() const
{
    CHIP_ERROR err          = CHIP_NO_ERROR;
    size_t numAttributePath = 0;
    TLV::TLVReader reader;

    PRETTY_PRINT("AttributePaths =");
    PRETTY_PRINT("[");

    // make a copy of the AttributePaths reader
    reader.Init(mReader);

    while (CHIP_NO_ERROR == (err = reader.Next()))
    {
        VerifyOrReturnError(TLV::AnonymousTag == reader.GetTag(), CHIP_ERROR_INVALID_TLV_TAG);
        VerifyOrReturnError(TLV::kTLVType_List == reader.GetType(), CHIP_ERROR_WRONG_TLV_TYPE);
        {
            AttributePathIB::Parser path;
            ReturnErrorOnFailure(path.Init(reader));
            PRETTY_PRINT_INCDEPTH();
            ReturnErrorOnFailure(path.CheckSchemaValidity());
            PRETTY_PRINT_DECDEPTH();
        }

        ++numAttributePath;
    }

    PRETTY_PRINT("],");
    PRETTY_PRINT("\t");

    // if we have exhausted this container
    if (CHIP_END_OF_TLV == err)
    {
        // if we have at least one data element
        if (numAttributePath > 0)
        {
            err = CHIP_NO_ERROR;
        }
    }
    ReturnErrorOnFailure(err);
    ReturnErrorOnFailure(reader.ExitContainer(mOuterContainerType));
    return CHIP_NO_ERROR;
}
#endif // CHIP_CONFIG_IM_ENABLE_SCHEMA_CHECK

AttributePathIB::Builder & AttributePaths::Builder::CreateAttributePath()
{
    if (mError == CHIP_NO_ERROR)
    {
        mError = mAttributePath.Init(mpWriter);
    }
    return mAttributePath;
}

// Mark the end of this array and recover the type for outer container
AttributePaths::Builder & AttributePaths::Builder::EndOfAttributePaths()
{
    EndOfContainer();
    return *this;
}
} // namespace app
} // namespace chip