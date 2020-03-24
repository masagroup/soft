# What is SOFT ? #
SOFT is the acronym of Sword Object Facilities.
 
SOFT is an implementation of the Eclipse Modeling Framework (EMF) core, and the Ecore metamodel in C++ and Go.

The current release allows to generate code from Ecore metamodels, and to parse and serialize models and metamodels from and into XMI documents. A reflective API for generated metamodels is also provided.

Soft consits of 2 parts for each langage :  a generator from Ecore metamodels, and the ecore library. The generators are currently implemented using Acceleo and Java.

# Features #

# Installation #
## Requirements ##
* JDK 8 https://www.oracle.com/java/ 
* Ant https://ant.apache.org/
  * AntContrib Tasks http://ant-contrib.sourceforge.net/
  
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
