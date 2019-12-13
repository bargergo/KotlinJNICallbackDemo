#ifndef AUDITIV_RITMUSTARTAS_OBOE_NDKEXTRACTOR_H
#define AUDITIV_RITMUSTARTAS_OBOE_NDKEXTRACTOR_H

#include <cstdint>
#include <android/asset_manager.h>
#include "../GameConstants.h"

class NDKExtractor {
public:
    static int32_t decode(AAsset *asset, uint8_t *targetData, AudioProperties targetProperties);
};


#endif //AUDITIV_RITMUSTARTAS_OBOE_NDKEXTRACTOR_H
