# brik

![ci](https://github.com/BrikWare/brik/actions/workflows/ci.yml/badge.svg)

A small compiler and language that will serve as the foundation for the [brik kernel](https://github.com/BrikWare/brik-kernel).

Currently, the following features are planned:
- **typestates** for kernel object safety;
- **comptime** functions;
- **implicit context** (an arena allocator is passed to every function secretly);
- **impl blocks** for methods and behavior grouping.

## Status

- [x] arena allocator + hash map
- [x] lexer
- [x] AST (still very simple: literals, identifiers, unary/binary)
- [x] tests (ongoing)
- [ ] parser
- [ ] semantic analysis
- [ ] comptime evaluator
- [ ] code generation

## Examples

1. Typestate Timer Object
    ```bk
    let t = timer();    // t: Timer<Stopped>
    t.start();          // t: Timer<Running>
    wait(t);            // works! requires Timer<Running>
    t.stop();           // t: Timer<Stopped>
    wait(t);            // compile error! t is Timer<Stopped>
    ```

2. Small Uart Driver

    ```bk
    // @ triggers evaluation for compile time execution
    @ baud_divisor(target: u32) -> u32 {
        return CLOCK_HZ / (target * 16);
    }
    
    struct Uart {
        base: u32,
    }
    
    impl Uart {
        fn open(self: &Uart<Uninit>) -> void { ... }
        fn arm(self: &Uart<Open>, irq: u8) -> void { ... }
        fn read(self: &Uart<Armed>) -> u8 { ... }
        fn close(self: &Uart<Armed>) -> void { ... }
    }
    
    fn driver() {
        let console: &Uart<Uninit> = uart0();   // uart0() returns a pointer to the board's console
        let div: u32 = @baud_divisor(115200); // no runtime div, it's a constant in the binary
        
        console.open(); // console: &Uart<Open>
        console.arm(4); // console: &Uart<Armed>
        
        let ring_buffer = alloc(4096);  // implicit context! the compiler passes the arena allocator
        
        let byte = console.read();  // works! requires Uart<Armed>
        console.close();    // &Uart<Closed>
        console.read();     // error caught in compile time! needs &Uart<Armed>, has &Uart<Closed>
    }
    ```

## Building

Needs C17 compiler and CMake 3.20+. \
`-Wall -Wextra -Werror -pedantic` are used to consider warnings as failures.

```bash
cmake -B build
cmake --build build

./build/brik file.bk     # tokenize a file (full pipeline still under construction)
./build/brik_tests       # run the tests
```
