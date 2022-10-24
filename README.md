# My personal notes on experimenting with C++20

## Coroutines

Each coroutine is associated with
- Promise object: manipulated from inside the coroutine.
- Coroutine handle: manipulated from outside the coroutine, non-owning handle to resume the execution
- Coroutine state: heap-allocated object that contains:
    1. promise object
    2. parameters (copied by value)
    3. suspension point
    4. local variables and temporaries (linked to suspension point)