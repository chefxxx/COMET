//
// Created by Mateusz Mikiciuk on 24/07/2025.
//

#include <gtest/gtest.h>
#include "O2Overlays.h"

namespace O2OverlaysTests
{
    struct PosX
    {
        double x;
        int rowIdx;
    };

    struct PosY
    {
        double y;
        int rowId;
    };

    struct MyPoint
    {
        PosX X;
        PosY Y;
        int columnIdx;
    };
}


TEST(O2Overlays, test1) {
    using BinningType = ColumnBinningPolicy<O2OverlaysTests::PosX, O2OverlaysTests::PosY>;
    const std::vector xBins{-0.064, -0.062, -0.060, 0.066, 0.068, 0.070, 0.072};
    const std::vector yBins{-0.320, -0.301, -0.300, 0.330, 0.340, 0.350, 0.360};
    BinningType bp{{xBins, yBins}, true};
};