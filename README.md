### Tracer
Use `make` to build the binary.

### Usage
```
./tracer <program to debug>
```

#### Available commands
* run - runs the program.
* b <address of breakpoint> - sets a new breakpoint at the given address.
* cont - continues execution after encountering a breakpoint.
* r - prints all registers.

Addresses can for instance be fetched using `objdump -d <program to debug>`.
