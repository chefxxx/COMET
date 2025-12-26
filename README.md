## 🔬 COMET

COMET is a highly optimised, generic library for generating tuples from large datasets. 

## 🌟 Features

- **BucketPolicy**
  - Define bucket vectors (doubles)
  - Get numer of bucket for given data
  - Get maximal number of buckets possible
  
- **Group data**
  - From given policy partitions data into buckets
  - Include/discard overflow values

## ℹ️ Overview

Bachelor of Engineering thesis project in collaboration with the [ALICE](https://alice-collaboration.web.cern.ch/) experiment group
at the Warsaw University of Technology.

In particle physics, scientists often have to calculate
[combinatorial background](https://physics.stackexchange.com/questions/12590/what-is-meant-by-combinatorial-background-in-experimental-high-energy-physics).
One common approach is to form pairs (or tuples) of data with similar properties,
such as primary vertex Z, multiplicity, or η of the leading particle.

Due to the enormous volume of data, this process must be optimized to ensure the shortest possible runtime.
EventMixing² offers a solution to this problem. It is a generic library designed to generate combinations of
elements, with support for applying specific constraints to the data being combined.

## 🚀 Usage

## 📖 Documentation 

Documentation is available [here](https://drive.google.com/file/d/1xbmc14TEWe5kFkSHFa9ceQpABfisQzlN/view?usp=sharing).

## 📑 License 

This project is licensed under GPL-3.0 license. See the [LICENSE](LICENSE) file for the full license text.
