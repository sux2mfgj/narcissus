# narcissus

Introduction
---
narcissus is emulator of h8/3069f micro computer.

Build narcissus
---

* install reqired packages
    - cmake
    - clang++
    - boost
    - socat

* build
    ```
    $ mkdir build  
    $ cd build  
    $ cmake ..  
    $ make  
    ```

How to use
---
you should execute 
```
$ narcissus --help
``` 

Build gdb for h8/300
---  
* get gdb   

* build  
    ```
    $ cd gdb-...
    $ ./configure --target=h8300-elf  
    $ make  
    ```
    
License
---
[Boost Software License - Version 1.0](http://www.boost.org/LICENSE_1_0.txt)  
please see this link.

