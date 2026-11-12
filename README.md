<!--
sword:
  schema: 1
  name: soft
  family: ecore-esync
  kind: build-tooling
  generation: shared
  shipped: false
  languages: [Go, TypeScript, C++, Ecore]
  build: [go, make, docker, npm, ant, cmake]
  ng: [ecore-model]
  apps: []
-->

# soft

> SOFT ("Sword Object Facilities") is MASA's implementation of the Eclipse Modeling Framework (EMF) core: the Ecore metamodel runtime in Go, TypeScript and C++, the code generator that turns `.ecore` models into typed code for these runtimes, and an `.ecore` editor for VS Code.

**[Architecture](ARCHITECTURE.md)** · [Contributing](CONTRIBUTING.md) · [known issues and open questions](https://github.com/masagroup/wiki/wiki/known-issues#soft) · [repository on GitHub](https://github.com/masagroup/soft)

## Overview

SOFT ("Sword Object Facilities") is MASA's implementation of the Eclipse Modeling Framework core. This repository only aggregates, as git submodules, the Ecore runtimes (Go, TypeScript, C++), the code generator, the `.ecore` models and the VS Code editor; each submodule has its own README, build and releases.

## Where to start

| You want to... | Read |
|---|---|
| understand how the pieces fit | [ARCHITECTURE.md](ARCHITECTURE.md) |
| contribute | [CONTRIBUTING.md](CONTRIBUTING.md) |
| generate Go or TypeScript code from an `.ecore` model | [soft.gen](https://github.com/masagroup/soft.gen) and [Run](#run) below |
| use or change the Go runtime | [soft.go](https://github.com/masagroup/soft.go) |
| use or change the TypeScript runtime, npm package `@masagroup/ecore` | [soft.ts](https://github.com/masagroup/soft.ts) |
| use or change the C++ runtime | [soft.cpp](https://github.com/masagroup/soft.cpp) |
| edit the metamodels (`ecore.ecore`, `library`, `tournament`, `empty`) | [soft.models](https://github.com/masagroup/soft.models) |
| edit `.ecore` files in VS Code | [soft.vscode](https://github.com/masagroup/soft.vscode) |

## Build

There is no build at the umbrella level: build each submodule from its own README. Clone with the submodules:

```
git clone --recurse-submodules git@github.com:masagroup/soft.git
# in an existing clone
git submodule update --init
```

The runtimes expect `models` as a sibling folder (`../models`), which this layout provides:

```
cd go && make generate   # regenerate the Go sources from ../models
cd ts && make            # regenerate, build and test the TypeScript sources
```

## Run

### Code generator

The generator runs from its Docker image, with the language as first argument.

```
docker run --rm -v "$PWD":/gen -w /gen masagroup/soft.gen go -m /gen/model.ecore -o /gen
docker run --rm -v "$PWD":/gen -w /gen masagroup/soft.gen ts -m /gen/model.ecore -o /gen
```

`soft.gen <language> -h` lists the templates and properties of a language, `-v` prints the version. Pin a version tag (`masagroup/soft.gen:v2.0.0-alpha.5`) in builds, as the Makefiles of soft.go and soft.ts do. Without Docker, install the binary with Go:

```
go install github.com/masagroup/soft.gen/v2/cmd/soft.gen@latest
```

Options, properties and configuration files are in the [soft.gen README](https://github.com/masagroup/soft.gen/blob/master/README.md).

## Documentation and guides

- [Architecture](ARCHITECTURE.md): position in the ecosystem, repository layout, internal structure, how it works, dependencies.
- [Contributing](CONTRIBUTING.md): development environment, Git and Jira workflow, conventions, CI/CD.
- The README of each submodule, linked from the [Where to start](#where-to-start) table.
- The same documentation in the SWORD wiki: [soft](https://github.com/masagroup/wiki/wiki/soft), [architecture](https://github.com/masagroup/wiki/wiki/soft-architecture), [contributing](https://github.com/masagroup/wiki/wiki/soft-contributing).
- License: [Mozilla Public License 2.0](LICENSE).
