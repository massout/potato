# potato
My operating system tests

# build & run
This project requires `make`, `qemu-system-aarch64` and [aarch64-none-elf-gcc](https://developer.arm.com/-/media/Files/downloads/gnu/12.2.rel1/binrel/arm-gnu-toolchain-12.2.rel1-x86_64-aarch64-none-elf.tar.xz?rev=28d5199f6db34e5980aae1062e5a6703&hash=F6F5604BC1A2BBAAEAC4F6E98D8DC35B) packages to generate and execute the code. The `make` build system is implemented in this project.

`make all (or just make)` -> to build project</br>
`make clear` -> to delete the `kernel8.img` image files of the project</br>
`make run` -> to run kernel8.img kernel file in qemu
