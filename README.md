Link to class diagram on DrawIO - https://drive.google.com/file/d/1WrwTxvoTF1j4UZCR_1wdCwp_3WmRjqN1/view?usp=sharing

# Library for generating combinations of elements

## Overview
## Implementation details
### BucketPolicy
`BucketPolicy` - a class that represent rule and functionality for definition of *buckets* - groups in which are placed
elements with some common attributes.

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
`[0.0, 1.0, 2.0, 3.0]`.