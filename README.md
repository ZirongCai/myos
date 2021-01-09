# myos

[![standard-readme compliant](https://img.shields.io/badge/standard--readme-OK-green.svg?style=flat-square)](https://github.com/RichardLitt/standard-readme)
TODO: Put more badges here.

build a operating system by myself

TODO: Fill out this long description.

## Table of Contents

- [Background](#background)
- [Usage](#usage)
- [Maintainers](#maintainers)
- [Contributing](#contributing)
- [License](#license)

## Background

### Global Descriptor Table

When interrup occurs, some code should be executed. But the current process has no knowledge about the target adress. The GDT stores the target adress of every interrupt. 

![](./pictures/GDT.jpeg)

Problem i have met:
```
    asm volatile("lgdt (%0)": :"p" (((uint8_t *) i)+2));
```

```
LGDT:Loads the values in the source operand into the global descriptor table register (GDTR) or the interrupt descriptor table register (IDTR). The source operand specifies a 6-byte memory location that contains the base address (a linear address) and the limit (size of table in bytes) of the global descriptor table (GDT) or the interrupt descriptor table (IDT). If operand-size attribute is 32 bits, a 16-bit limit (lower 2 bytes of the 6-byte data operand) and a 32-bit base address (upper 4 bytes of the data operand) are loaded into the register. If the operand-size attribute is 16 bits, a 16-bit limit (lower 2 bytes) and a 24-bit base address (third, fourth, and fifth byte) are loaded. Here, the high-order byte of the operand is not used and the high-order byte of the base address in the GDTR or IDTR is filled with zeros.
```

Consider the following function

```
int main(int argc, const char * argv[]) {
    // insert code here...

    unsigned int i[2];
    i[0] = 0x41424344;
    i[1] = 0x45464748;

    cout<<((unsigned char *)i+2)<<endl;
    return 0;
}
```
## Usage

```
```

## Maintainers

[@ZirongCai](https://github.com/ZirongCai)

## Contributing

PRs accepted.

Small note: If editing the README, please conform to the [standard-readme](https://github.com/RichardLitt/standard-readme) specification.

## License

? © 2021 ZirongCai
