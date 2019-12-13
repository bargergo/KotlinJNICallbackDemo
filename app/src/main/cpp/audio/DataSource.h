#ifndef AUDITIV_RITMUSTARTAS_OBOE_DATASOURCE_H
#define AUDITIV_RITMUSTARTAS_OBOE_DATASOURCE_H

#include <cstdint>
#include "../GameConstants.h"

class DataSource {
public:
    virtual ~DataSource() = default;
    virtual int64_t getSize() const = 0;
    virtual AudioProperties getProperties() const  = 0;
    virtual const float* getData() const = 0;
};

#endif //AUDITIV_RITMUSTARTAS_OBOE_DATASOURCE_H
