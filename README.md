# Aevus

Aevus is a highly experimental tool to generate animations involving Apollonian gaskets
using Iterated Function Systems (IFS) techniques.

At the moment it's capable of generating IFS parameters for very deep zooms and
serializing them to an XML format.

Aevus uses Bazel for its build system and Conan for its package management. It's recommended
to build on Linux, as other OS's are not tested.

To start, clone this repo with

```
git clone https://github.com/FelipeLopes/aevus.git
cd aevus
```

Then, on the conandeps folder, install the packages with conan:

```
cd conandeps
conan install .
```

Finally, back on the aevus folder, run bazel to build the project:

```
cd ..
bazel build gasket
```

Execute the binary with

```
bazel-bin/gasket
```

To develop Aevus, it's recommended to use VSCode with the Bazel, Bazel-stack-vscode,
and clangd extensions. By generating a compile_commands.json file, clangd can provide
indexing to VSCode.
