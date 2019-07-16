//
// Created by alex on 7/16/19.
//

#ifndef GIE_GIENODEID_H
#define GIE_GIENODEID_H

#include <StrongAlias.h>
#include <QUuid>

using GieNodeId = StrongAlias<QUuid, struct GieNodeIdTag>;

inline bool operator< (const GieNodeId& lhs, const GieNodeId& rhs)
{
    return lhs.get() < rhs.get();
}

#endif //GIE_GIENODEID_H
