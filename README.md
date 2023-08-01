# Aevus

Aevus is a highly experimental fractal flame renderer.

At the moment it has basic custom libraries for XML serialization and OpenCL kernel execution.
Aevus can read and write flames in an Apophysis compatible XML format, and pass the parameters
to an OpenCL kernel, which performs the chaos game iterations and does tone mapping. It can also
export the resulting image in PAM format.

The code is currently very basic and has no support for advanced features like gamma correction
and density filtering. However, Aevus uses a different technique for coloring which, unlike other
renderers like Flam3, does not suffer from clipping issues in color channels. As a result, color information
is preserved better, and flames are not colored close to white at lower values of saturation,
as shown in the example below:

<table cellspacing="0" cellpadding="0">
  <tr>
    <td><img src="img/aevus.png?raw=true" width="320"></td>
    <td><img src="img/flam3.png?raw=true" width="320"></td>
  </tr>
  <tr>
    <td>Aevus render</td>
    <td>flam3 render</td>
  </tr>
</table>

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
bazel build aevus
```

Execute the binary with

```
bazel-bin/aevus
```

To develop Aevus, it's recommended to use VSCode with the Bazel, Bazel-stack-vscode,
and clangd extensions. By generating a compile_commands.json file, clangd can provide
indexing to VSCode, enabling intellisense and autocomplete.
