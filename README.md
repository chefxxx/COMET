## EventMixing²

Bachelor of Engineering thesis project in collaboration with the ALICE experiment group
at the Warsaw University of Technology.

In particle physics, scientists often have to calculate
[combinatorial background](https://physics.stackexchange.com/questions/12590/what-is-meant-by-combinatorial-background-in-experimental-high-energy-physics).
One common approach is to form pairs (or tuples) of data with similar properties,
such as primary vertex Z, multiplicity, or η of the leading particle.

Due to the enormous volume of data, this process must be optimized to ensure the shortest possible runtime.
EventMixing² offers a solution to this problem. It is a generic library designed to generate combinations of
elements, with support for applying specific constraints to the data being combined.

While originally developed for particle physics applications, the library is built in a way that allows it to be used
in broader domains. We see strong potential for future use in statistical research in many areas particularly
such as medicine, pharmaceutical industry and biology. 

## Implementation details

### BucketPolicy
BucketPolicy is a class that provides partitioning data by *buckets*. One *bucket* defines criteria
that elements of data have to meet in order to be assigned to that bucket. Buckets have to be defined as a 
vector of doubles, where each consecutive pair in such vector is a valid interval e.g.
`std::vector vec{0.1, 0.2, 0.3, 0.4}`.

It is important, that user must define how to get access to parts of elements inside container by passing tuple 
of function objects, one for each dimension of grouping.

- `template <typename TElement> auto getBucket(TElement const &arg)` - public method of `BucketPolicy`
class, which main goal is to find number of bucket for element of container. Firstly method applies function objects from
the tuple of function objects, which is member of a class. After that method uses function `findUpperIndex` for finding index of a bucket in each dimension. Method *tempoarary* returns
  a tuple which contains index of range, where this value is placed.
  - typename TElement - type of element that will be passed in parameter and from which bucket number should be found.
  - TElement const& arg - element passed by reference, every function object from member `mCallables` will be called with 
  this element as an argument to define values for finding bucket number.
  - *NOTE:* method requires (as concept) that every function object, passed to constructor before, can take element
  specified in this method as an argument and return *Temporary* value convertible to double.

- `template <typename T> findUpperIndex(std::vector<T> const& data, T const& value)` - function that takes finds number 
of range, in which value is placed. It uses inside function `std::upper_bound` for finding upper bound.
After iterator to element was found, the distance between start and found index is counted. 
According to first argument is a `std::vector` and its iterator is `RandomAccessIterator`, complexity of such operation should be `O(log(n))`.
This function introduces a convention, that in vector of *N* elements, that represents *N + 1* ranges and is sorted,
returned values gives number of range started from 0 to N. Example: Assume we have a vector:
`[0.0, 1.0, 2.0, 3.0]`. It gives us 5 ranges: (-Inf:0.0), [0.0:1.0), [1.0:2.0), [2.0:3.0), [3.0:+Inf), which can be named
with indices 0, 1, 2, 3, 4. That gives us that value *3.4* is placed under index 4.
  - type_with_less_operator T - template argument, that tells what is the type of argument of a function. Typename must 
  have defined `< operator`.
  - std::vector<T> const& data - vector that contains borders of ranges.