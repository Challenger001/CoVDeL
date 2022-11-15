# CoVDeL

CoVDeL is a C++ library that provides implementations of classical computer vision and common image
processing algorithms, along with support for neural networks and deep learning variants. Although
the project's software architecture is inspired from popular libraries like **NumPy**, **OpenCV**,
**PIL**, **PyTorch** and **Tensorflow**, everything has been implemented from scratch and no major
third party libraries were used. This project uses ISO C++17 standard.

## Modules

The library has the following hierarchy of modules and the classes defined within -

1. **Multiarray** [ `ma` ]
2. **Computer Vision** [ `cv` ]
    * **Image**
3. **Deep Learning** [ `nn` ]
    * **Tensor**
    * **Module**

## Documentation

### Multiarray

This module contains the base multidimensional array type which will be ubiquitously used in other
modules for various types of data storage, and for building more specific storage types.  
All symbols in this module belong to `covdel::ma` namespace, and their definitions can be found in
source files under `src/ma` directory and in public headers under `include/covdel/ma` directory.

* `datatype.hh`
  * `datatype` enum class holds labels for all supported datatypes for `multiarray`.
  * `dtype` namespace holds the base data type class and its children which are used in the template
  initialization of `multiarray`, as well as a compile-time validator template to check if the
  template type argument is supported.
* `multiarray.hh` `multiarray.cc`
  * `dimension` class is responsible for handling dimensionality behavior with respect to the array
  classes.
  * `multiarray` class template is the core array class which is templated by the type of data it is
  capable of storing. It holds the data in a contiguous array, and a mutable `dimension` object.
* `factory.hh`
  * **Type Aliases** for convenience are provided to construct the array without having to resort to
  the cumbersome template syntax. These are provided for all supported types in the `datatype` enum.
  * `array` factory function template is the recommended method to construct a `multiarray` object.
  This array forwards its arguments to the underlying constructor, which builds the class instance.
