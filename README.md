### Embedded Primes
Computes primes using Sieve of Eratosthenes Algorithm. While currently this only supports x86, the plan is to adapt this algorithm to support threads on supported systems and then port to embedded architectures.

### Todo
* Add Threading.
* Add x86_32 assembly version.
* Support ARMv6 based cpus, i.e. BCM 2835.
* Support ARMv6-M based cpus, i.e. Cortex M0+.
* Support ARMv7E-M based cpus, i.e. Cortex M4(F).
* Support Xtensa LX6 based cpus, i.e. ESP32.
* Support RISC-V based cpus, i.e. K210.
* Support AVR RISC based cpus, i.e. ATmega128, ATmega328(P) and ATmega32U4.
* Support other programming languages, if so rename project since then it would be less about embedded support and more just having lots of implemnentations.

I plan to support these architectures specifically because I own processors implementing them. 

Pull requests with support for other architectures would be appreciated.

This project is based on code from https://www.cs.cmu.edu/~scandal/cacm/node8.html, license is included.
