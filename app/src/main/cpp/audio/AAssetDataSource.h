#ifndef AUDITIV_RITMUSTARTAS_OBOE_AASSETDATASOURCE_H
#define AUDITIV_RITMUSTARTAS_OBOE_AASSETDATASOURCE_H

#include <android/asset_manager.h>
#include <memory>
#include "DataSource.h"

class AAssetDataSource : public DataSource {

public:
    int64_t getSize() const override { return mBufferSize; }
    AudioProperties getProperties() const override { return mProperties; }
    const float* getData() const override { return mBuffer.get(); }

    static AAssetDataSource* newFromCompressedAsset(
            AAssetManager &assetManager,
            const char *filename,
            AudioProperties targetProperties);

private:

    AAssetDataSource(std::unique_ptr<float[]> data, size_t size,
                     const AudioProperties properties)
            : mBuffer(std::move(data))
            , mBufferSize(size)
            , mProperties(properties) {
    }

    const std::unique_ptr<float[]> mBuffer;
    const int64_t mBufferSize;
    const AudioProperties mProperties;

};


#endif //AUDITIV_RITMUSTARTAS_OBOE_AASSETDATASOURCE_H
