//
// Created by Mateusz Mikiciuk on 19/01/2026.
//

#include <iostream>
#include <vector>

#include "include/Combinations.h"
#include "include/BucketPolicy.h"
#include "include/Producers.h"

struct Car
{
    explicit Car(const double some_trait, const std::string& id) : some_trait_1(some_trait), some_trait_2(some_trait), id(id) {}
    double some_trait_1;
    double some_trait_2;
    std::string id;
};

struct Plane
{
    explicit Plane(const double some_trait, const std::string& id) : some_trait_1(some_trait), some_trait_2(some_trait), id(id) {}
    double some_trait_1;
    double some_trait_2;
    std::string id;
};

constexpr float PLANE_COST_COEFFICIENT = 2.0;
constexpr float PLANE_TIME_COEFFICIENT = 0.5;
constexpr float CAR_COST_COEFFICIENT   = 0.5;
constexpr float CAR_TIME_COEFFICIENT   = 2.0;

struct TimeFun
{
    double operator()(const Plane& plane) const { return plane.some_trait_1 * PLANE_TIME_COEFFICIENT; }
    double operator()(const Car& car) const { return car.some_trait_1 * CAR_TIME_COEFFICIENT; }
};

struct CostFun
{
    double operator()(const Plane& plane) const { return plane.some_trait_2 * PLANE_TIME_COEFFICIENT; }
    double operator()(const Car& car) const { return car.some_trait_2 * CAR_TIME_COEFFICIENT; }
};

inline std::ostream& operator<<(std::ostream& os, const Car& car)
{
    os << car.id;
    return os;
}

inline std::ostream& operator<<(std::ostream& os, const Plane& plane)
{
    os << plane.id;
    return os;
}

int main() {
    // containers that satisfy random access iterator concept
    const std::vector s1 = {
        Car(1.0, "honda_1"),
        Car(2.0, "toytoa_1"),
        Car(3.0, "audi_1"),
        Car(4.0, "bmw_1"),
        Car(5.0, "fiat_1"),
        Car(6.0, "honda_1"),
        Car(7.0, "toytoa_1"),
        Car(8.0, "audi_1"),
        Car(9.0, "bmw_1"),
        Car(10.0, "fiat_1")
    };
    const std::array  s2 = {
        Plane(1.0, "airbus_1"),
        Plane(2.0, "boeing_1"),
        Plane(3.0, "tu_1"),
        Plane(4.0, "su_1"),
        Plane(5.0, "mig_1"),
        Plane(6.0, "f16_1"),
        Plane(6.6, "airbus_1"),
        Plane(7.0, "boeing_1"),
        Plane(8.0, "tu_1"),
        Plane(9.0, "su_1"),
        Plane(10.0, "mig_1")
    };
    const std::array  s3 = {
        Plane(1.0, "airbus_2"),
        Plane(2.0, "boeing_2"),
        Plane(3.0, "tu_2"),
        Plane(4.0, "su_2"),
        Plane(5.0, "mig_2"),
        Plane(6.0, "f16_2"),
        Plane(6.6, "airbus_2"),
        Plane(7.0, "boeing_2"),
        Plane(8.0, "tu_2"),
        Plane(9.0, "su_2"),
        Plane(10.0, "mig_2")
    };
    const std::vector s4 = {
        Car(1.0, "honda_2"),
        Car(2.0, "toytoa_2"),
        Car(3.0, "audi_2"),
        Car(4.0, "bmw_2"),
        Car(5.0, "fiat_2"),
        Car(6.0, "honda_2"),
        Car(7.0, "toytoa_2"),
        Car(8.0, "audi_2"),
        Car(9.0, "bmw_2"),
        Car(10.0, "fiat_2")
    };

    constexpr int minimumCategorySize = 1;
    constexpr int maxNeighbours = 1;

    constexpr std::array costBuckets  = {0.0, 11.0, 22.0};
    constexpr std::array timeBuckets  = {0.0, 6.0, 12.0};

    const auto bucketPolicy = BucketPolicy(true, CostFun{}, TimeFun{}, costBuckets, timeBuckets);
    // Note: This buckets assignment can serve use to make combinations
    // between comparable time and cost efficiency cars and planes.

    auto combinationsProducer = makeBlockCombinations<FullCombinationsPolicy>(bucketPolicy, minimumCategorySize, s1, s2);
    std::cout << "Full producer:\n";
    for (const auto& [elem1, elem2] : combinationsProducer) {
        // Do something with the created combination ...
        std::cout << "Produced tuple: (" << elem1 << ", " << elem2 << ")\n";
    }

    std::cout << "StrictlyUpper producer:\n";
    auto strictlyUpperCombinationsProducer = makeBlockCombinations<StrictlyUpperCombinationsPolicy>(bucketPolicy, minimumCategorySize, s1, s2);
    for (const auto& [elem1, elem2] : strictlyUpperCombinationsProducer) {
        // Do something with the created combination ...
        std::cout << "Produced tuple: (" << elem1 << ", " << elem2 << ")\n";
    }

    std::cout << "SameKindFull producer:\n";
    auto sameKindFull = makeSameKindBlockCombinations<SameTypeFullCombinationsPolicy>(bucketPolicy, minimumCategorySize, maxNeighbours, s2, s3);
    for (const auto& [elem1, elem2] : sameKindFull) {
        // Do something with the created combination ...
        std::cout << "Produced tuple: (" << elem1 << ", " << elem2 << ")\n";
    }

    std::cout << "SameKindStrictlyUpper producer:\n";
    auto sameKindStrictlyUpper = makeSameKindBlockCombinations<SameTypeStrictlyUpperCombinationsPolicy>(bucketPolicy, minimumCategorySize, maxNeighbours, s1, s4);
    for (const auto& [elem1, elem2] : sameKindStrictlyUpper) {
        // Do something with the created combination ...
        std::cout << "Produced tuple: (" << elem1 << ", " << elem2 << ")\n";
    }
}