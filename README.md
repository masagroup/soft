# What is SOFT ? #
SOFT is the acronym of Sword Object Facilities.
 
SOFT is an implementation of the Eclipse Modeling Framework (EMF) core, and the Ecore metamodel in C++ and Go.

The current release allows to generate code from Ecore metamodels, and to parse and serialize models and metamodels from and into XMI documents. A reflective API for generated metamodels is also provided.

Soft consits of 2 parts for each langage :  a generator from Ecore metamodels, and the ecore library. The generators are currently implemented using Acceleo and Java.

# Features #
## C++ and Go features ##
* Multiple Inheritance - A class can be defined with many interfaces
* Handling of containment and opposite relations - Setting one side of a relation automatically sets the other side.
* XMI/XML serialization and deserialization of metamodel/model instances - Resource framework allows splitting of serializations, including cross-resource containment.
* Reflection - MetaModel structure is available at runtime
* Notification - Events are raised when the model is changed
* Extension - Generated code can be overriden by the user

## C++ features ##
* C++17
* Memory Management with shared pointers

# Installation #
## Requirements ##
* JDK 8 https://www.oracle.com/java/ 
* Ant https://ant.apache.org/
  * AntContrib Tasks http://ant-contrib.sourceforge.net/
* VS2019 
  
## Installation from source distribution ##
```
git clone https://github.com/masagroup/soft.git
cd soft
ant
```
### Details
Installation process:
* install all dependencies
  * aria2
  * maven
  * cmake
  * ninja
  * vcpkg
    * boost
    * turtle
    * xercesc
* generators
  * build
  * distribute 
* cpp
  * generate, build, test and distribute ecore library
  * generate, build and test user library
* go
  * generate, build and test ecore library
  * generate, build and test user library
### Binaries
The binaries generated by the installation process are located in the dist folder. They consists of:
* ecore-\<version\>-\<platform\>.zip : cpp files with
  * headers
  * libraries
  * cmake files
* soft.generator.cpp-\<version\>.zip : cpp generator
* soft.generator.go-\<version\>.zip : go generator


# Generators #
How to execute a generator:
```
unzip soft.generator.<lang>-<version>.zip
cd soft.generator.<lang>-<version>
java -jar soft.generator.<lang>-<version>.jar -m <ecore file> -o <output directory>
```

Each generator is composed of a group of templates. These templates are listed via the command line:
```
java -jar soft.generator.<lang>-<version>.jar -h
```
You can use a specific template ( or everything except one) via the command line
```
java -jar soft.generator.<lang>-<version>.jar -t <template> !<not template>
```
