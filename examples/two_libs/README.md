Shared vs. Static Library
=========================

The purpose of this short little example is to demonstrate how to build a
statically compiled archive (a `.a` file) from one or more object files and how
to take the same input source code and generate a shared object file from it
instead.  Then, using the two libraries, statically or dynamically link against
them.

Instructions
------------

Clone the git repository and then, from the top level directory, run the
following:
```bash
$ make clean
$ make all
```
Assuming that your compiler, assembler, and coreutils are all in the location
assumed in the `Makefile` you should see a `shared_demo` and `static_demo`
appear. Now run the following:

```bash
shared_demo:
	linux-vdso.so.1 (0x00007fff063f6000)
	libsquares.so => not found
	libc.so.6 => /lib/x86_64-linux-gnu/libc.so.6 (0x00007f41f0832000)
	/lib64/ld-linux-x86-64.so.2 (0x00007f41f0a4a000)
static_demo:
	linux-vdso.so.1 (0x00007ffff29ef000)
	libc.so.6 => /lib/x86_64-linux-gnu/libc.so.6 (0x00007fa5a17b7000)
	/lib64/ld-linux-x86-64.so.2 (0x00007fa5a19cf000)
```
Fair enough - the `static_demo` ELF depends on the virtual dynamic shared object
that the kernel exposes, the dynamic linker, and the GNU C library in `/lib`.
Also, as expected, `shared_demo` has an additional dependancy on the
`libsquares` library that was built by one of the targets in the `Makefile`, but
unfortunately, it would appear that the dynamic linker cannot find it:
```bash
$ ./shared_demo 
./shared_demo: error while loading shared libraries: libsquares.so: cannot open shared object file: No such file or directory
```
So the question that one might ask is why was it able to get through the
compilation and linking phase of the build process but then doesn't work now.
The answer lies in the behavior of the GNU linker, which has a well-defined method for
resolving shared object dependancies and for whatever reason, was not able to
resolve that one. The man pages for `ld.so` have all the excruciating detail
and specify the steps it goes through to try to do it all and in which order.
One of several ways to modify the behavior of the dynamic linker is through the
use of the `LD_LIBRARY_PATH` environment variable.  Its value is consulted at
runtime by the linker to obtain additional locations to search in attempting to
resolve those dependancies. To see this, we have the following:
```bash
$ LD_LIBRARY_PATH=$(readlink -f lib) ./shared_demo
The square of 42 is 1764
```
One can further experiment by running the same commands with and without setting
`LD_LIBRARY_PATH` with `strace` and watching the ensuing output change.  For all
of the additional gory details, check out `man 8 ld.so` and search on
<b>LD_LIBRARY_PATH</b>. Note that the linker ignores the value of this
environment variable if the executable is being run in secure-execution mode,
which explains this behavior
```bash
$ LD_LIBRARY_PATH=$(readlink -f lib) sudo ./shared_demo 
./shared_demo: error while loading shared libraries: libsquares.so: cannot open shared object file: No such file or directory
$ LD_LIBRARY_PATH=$(readlink -f lib) ./shared_demo 
The square of 42 is 1764
```
As the `ld.so` and `getauxval` man pages go on to discuss, one of the ways to trigger
secure-execution mode is to attempt to run it as root.

