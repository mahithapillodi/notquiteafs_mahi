## NotQuiteAFS

### What we have:
We currently have unreliablefs, which is a basic fuse file system. It pretty much just does nothing except basic file system stuff, and allows for fault injection. We also have a client and a server that communicate with grpc. The client and the server are separate entities from unreliablefs currently

### What we have to do next:
The next step would be moving all grpc communication headers and stuff from the `client.cc` file to `unreliablefs_ops.c`. This way, we can have communication occur whenever file system operations are ran. This is merely a copy and paste, and will require some edits to the CMakeLists.txt so that `unreliablefs_ops.c` gets compiled with the correct grpc stuff as well as the fuse stuff.

We then have to facilitate communication between the client and the server on `open` and `close`. We have to make a directory to act as the root directory for the server's persistent storage, and make a file to live in that directory. When the client then tries to open that file, it should send a grpc request to the server, asking for that file. The server should basically just send the full file contents back to the client, and the client should copy those file contents to a new file locally.

Note that the client will not have access to the directory structure of the server's persistent storage (i.e. using `cd` and `ls` on the client side will not allow you to dig through the server's file system) because we will have to change the `lstat`/`getattr` functionality on the client side to ask for that info from the server.

### Building

Prerequisites:

- CentOS: `dnf install -y gcc -y cmake fuse fuse-devel`
- Ubuntu: `apt-get install -y gcc cmake fuse libfuse-dev`
- FreeBSD: `pkg install gcc cmake fusefs-libs pkgconf`
- OpenBSD: `pkg_add cmake`
- macOS: `brew install --cask osxfuse`

Additionally, you will need to get grpc. Follow these [instructions](https://grpc.io/docs/languages/cpp/quickstart/).

In order to build, in the outermost directory run these commands.
```sh
$ cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
$ cmake --build build --parallel
```
### Using client and server
You will see a new folder called build spawn. In there, there is a subdirectory called notquiteafs. Within there, there is a client and a server directory. The executables needed to run the client and server are found there, as well as the original unreliablefs code which can be ran to mount that file system.

### Using unreliablefs

```sh
$ mkdir /tmp/fs
$ unreliablefs /tmp/fs -base_dir=/tmp -seed=1618680646
$ cat << EOF > /tmp/fs/unreliablefs.conf
[errinj_noop]
op_regexp = .*
path_regexp = .*
probability = 30
EOF
$ ls -la
$ umount /tmp/fs
```
### Documentation for unreliablefs

See documentation in [unreliablefs.1](https://ligurio.github.io/unreliablefs/unreliablefs.1.html) and
[unreliablefs.conf.5](https://ligurio.github.io/unreliablefs/unreliablefs.conf.5.html).

