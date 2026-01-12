//
// Created by msh on 1/6/26.
//

#ifndef COMET_HEAVYSTRUCTURES_H
#define COMET_HEAVYSTRUCTURES_H
#include <array>
#include <random>
#include <string>

struct HeavyGroupingStruct {
    int globalIndex;
    double posX;
    double posY;
    double posZ;

    int multiplicityFirst  = 0;
    int multiplicitySecond = 0;
    int multiplicityThird  = 0;

    std::array<double, 16> firstMatrix;
    std::array<double, 16> secondMatrix;
    std::array<double, 16> thirdMatrix;
    std::array<double, 64> largerArray;
};

struct GetIdHeavyGroupingCallable {
    auto operator()(const HeavyGroupingStruct& element) const { return element.globalIndex; }
};

template <typename... AssociatedMeanMultiplicities>
    requires(sizeof...(AssociatedMeanMultiplicities) <= 3)
struct HeavyGroupingStructGenerator {
    HeavyGroupingStructGenerator(
        int seed, const AssociatedMeanMultiplicities&... associatedMultiplicities
    )
        : m_mtEngine(seed),
          m_doubleDistribution(std::uniform_real_distribution<double>(-10.0, 10.0)),
          m_poissonDistributions(std::poisson_distribution<int>(associatedMultiplicities)...){};

    HeavyGroupingStruct generate(int id)
    {
        HeavyGroupingStruct result{};
        result.globalIndex = id;
        constexpr int N    = sizeof...(AssociatedMeanMultiplicities);
        result.posX        = m_doubleDistribution(m_mtEngine);
        result.posY        = m_doubleDistribution(m_mtEngine);
        result.posZ        = m_doubleDistribution(m_mtEngine);
        if constexpr (N >= 1) {
            result.multiplicityFirst = std::get<0>(m_poissonDistributions)(m_mtEngine);
            if constexpr (N >= 2) {
                result.multiplicitySecond = std::get<1>(m_poissonDistributions)(m_mtEngine);
                if constexpr (N == 3) {
                    result.multiplicityThird = std::get<2>(m_poissonDistributions)(m_mtEngine);
                }
            }
        }

        result.firstMatrix.fill(0.0);
        result.secondMatrix.fill(2.0);
        result.thirdMatrix.fill(4.0);
        result.largerArray.fill(8.0);

        return result;
    }

    HeavyGroupingStruct operator()(int id) { return generate(id); }

    private:
    std::mt19937 m_mtEngine;
    std::uniform_real_distribution<double> m_doubleDistribution;
    std::tuple<
        std::conditional_t<true, std::poisson_distribution<int>, AssociatedMeanMultiplicities>...>
        m_poissonDistributions;
};

struct HeavyAssociatedFirstStruct {
    int value;
    int groupingIndex;
    double px;
    double py;
    double pz;

    unsigned char flags;
    std::array<double, 16> valuesInMatrix;
    std::array<double, 16> secondValuesInMatrix;
};

struct GetIdHeavyAssociatedFirstStructCallable {
    auto operator()(const HeavyAssociatedFirstStruct& element) const
    {
        return element.groupingIndex;
    }
};

struct HeavyAssociatedFirstStructGenerator {
    HeavyAssociatedFirstStructGenerator(int seed)
        : m_mtEngine(seed),
          m_doubleDistribution(std::uniform_real_distribution<double>(-10.0, 10.0))
    {
    }

    HeavyAssociatedFirstStruct generate(int parentId, int value)
    {
        HeavyAssociatedFirstStruct result{};
        result.groupingIndex = parentId;
        result.value         = value;
        result.px            = m_doubleDistribution(m_mtEngine);
        result.py            = m_doubleDistribution(m_mtEngine);
        result.pz            = m_doubleDistribution(m_mtEngine);
        result.flags         = 0b00001111;
        result.valuesInMatrix.fill(48.0);
        result.secondValuesInMatrix.fill(4.0);
        return result;
    }

    HeavyAssociatedFirstStruct operator()(int parentId, int value)
    {
        return generate(parentId, value);
    }

    private:
    std::mt19937 m_mtEngine;
    std::uniform_real_distribution<double> m_doubleDistribution;
};

struct HeavyAssociatedSecondStruct {
    int value;
    int globalIndex;
    std::string name;
    std::string groupName;
    std::string color;

    std::array<std::string, 16> arrayOfStrings;
};

struct GetIdHeavyAssociatedSecondStructCallable {
    auto operator()(const HeavyAssociatedSecondStruct& element) const
    {
        return element.globalIndex;
    }
};

struct HeavyAssociatedSecondStructGenerator {
    HeavyAssociatedSecondStruct generate(int parentId, int value)
    {
        HeavyAssociatedSecondStruct result{};
        result.globalIndex = parentId;
        result.value       = value;
        result.name        = std::to_string(value);
        result.color       = m_color + result.name;
        result.groupName   = m_groupName + result.name;
        result.arrayOfStrings.fill("TEST_STRING");
        return result;
    }

    HeavyAssociatedSecondStruct operator()(int parentId, int value)
    {
        return generate(parentId, value);
    }

    private:
    std::string m_color     = "green_";
    std::string m_groupName = "group_";
};

struct HeavyAssociatedThirdStruct {
    int value;
    int indexForGrouping;
    std::array<double, 128> bigArrayDoubles;
    std::array<double, 8> smallArrayDoubles;
};

struct GetIdHeavyAssociatedThirdStructCallable {
    auto operator()(const HeavyAssociatedThirdStruct& element) const
    {
        return element.indexForGrouping;
    }
};

struct HeavyAssociatedThirdStructGenerator {
    HeavyAssociatedThirdStruct generate(int parentId, int value)
    {
        HeavyAssociatedThirdStruct result{};
        result.indexForGrouping = parentId;
        result.value            = value;
        result.bigArrayDoubles.fill(1.0);
        result.smallArrayDoubles.fill(2.0);
        return result;
    }

    HeavyAssociatedThirdStruct operator()(int parentId, int value)
    {
        return generate(parentId, value);
    }
};

#endif  // COMET_HEAVYSTRUCTURES_H
