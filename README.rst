=========
heaptrace
=========

Traces calls to malloc, realloc, and free

Usage
=====
The shared object can be loaded using ``LD_PRELOAD`` like so
::
    LD_PRELOAD=heap_trace.so ./executable

Example
=======
heaptrace will output information about calls to malloc, realloc, and free
    - address or resolved symbol of parent function if possible
    - the call being made including arguments
    - the return value of the call

::

    % LD_PRELOAD=heap_trace.so ./nc
    0x7f2287787492: malloc(5) = 0xec4010
    0x7f228778232e: free(0xec4010)
    ...
    0x7f22877879f0: free(0xec6360)
    0x7f2287785aa7: free(0xec4f00)
    nc: you must specify the address/port couple of the remote endpoint

Building
========
Run ``make``
