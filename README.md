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

> SOFT ("Sword Object Facilities") is the umbrella repository that aggregates, as git submodules, MASA's own Ecore implementation in Go, TypeScript and C++, the Go code generator `soft.gen`, the `.ecore` metamodels and the VS Code Ecore editor; it is the foundation of the ecore model code generation chain used by the SWORD ecosystem.

| | |
|---|---|
| **Family** | Ecore / esync |
| **Languages** | Umbrella itself: none. Submodules: Go (`gen`, `go`), TypeScript (`ts`, `vscode`), C++17 (`cpp`), Ecore XMI models (`models`) |
| **Build systems** | none at the umbrella level · per submodule: Make + Go + Docker (`gen`, `go`) · Make + npm + Docker (`ts`) · npm + esbuild (`vscode`) · Ant + CMake/Ninja (`cpp`, Windows/MSVC only) |

**[Architecture](ARCHITECTURE.md)** · [Contributing](CONTRIBUTING.md) · [known issues and open questions](https://claude.ai/artifact/UfvKmky7e8LwEXf192hQjQ#soft) · [repository on GitHub](https://github.com/masagroup/soft)

## Overview

`soft` contains only `.gitmodules`, the documentation, `LICENSE` and six gitlinks. All real content lives in the submodules, checked out at pinned commits. License: Mozilla Public License 2.0 (source headers: "Copyright(c) 2021 MASA Group").

<details>
<summary>More</summary>

SOFT implements the core of the Eclipse Modeling Framework (EObject / EClass / EPackage / EResource, notifications, XMI / XML / binary serialization) in three languages. It is bootstrapped: the same Ecore metamodel (`models/ecore.ecore`) is the input of the generator, which emits the generated half of the Ecore library itself, while a hand-written half (`Basic*`, `*Ext`, resources, codecs) lives in each library.

The generator is `soft.gen` v2: one Go command (`text/template` templates) with one subcommand per language, `go` and `ts`, packaged as the Docker image `masagroup/soft.gen`. It replaces the v1 Java / Acceleo generators (`masagroup/soft.generator.{go,cpp,ts}`), which remain in the history of `soft.gen` up to the `v1.x` tags. C++ is not ported to v2 yet: `cpp` still runs the v1 image `masagroup/soft.generator.cpp`.

The libraries do no network I/O and expose no ports. The only runnable programs are the code generator, the VS Code extension and the test executables.

### Submodules and pinned commits

| Path | `.gitmodules` URL | Pinned commit | Version at pin | Role |
|---|---|---|---|---|
| `gen` (section named `java`, path `gen`) | https://github.com/masagroup/soft.gen.git | `46a1656` (2026-10-27) | v2.0.0-alpha.5 + 4 | Go code generator `soft.gen` for go and ts ([soft.gen](https://github.com/masagroup/soft.gen)) |
| `cpp` | https://github.com/masagroup/soft.cpp.git | `bd217da` (2021-05-03) | v1.2.4 | C++ ecore library ([soft.cpp](https://github.com/masagroup/soft.cpp)) |
| `go` | https://github.com/masagroup/soft.go.git | `ac30827` (2026-10-27) | v1.14.0 + 4 | Go ecore library and codec modules ([soft.go](https://github.com/masagroup/soft.go)) |
| `ts` | https://github.com/masagroup/soft.ts.git | `dc4fe70` (2026-10-27) | v1.7.2 + 27 (`package.json` 1.8.0) | TypeScript ecore library ([soft.ts](https://github.com/masagroup/soft.ts)) |
| `models` | https://github.com/masagroup/soft.models.git | `9217d2a` (2026-10-27) | v1.8.0 + 19 | `.ecore` metamodels used to bootstrap the libraries ([soft.models](https://github.com/masagroup/soft.models)) |
| `vscode` | git@github.com:masagroup/soft.vscode.git | `1c77bc7` (2026-10-26) | v0.1.0 | Ecorium, the VS Code Ecore model editor ([soft.vscode](https://github.com/masagroup/soft.vscode)) |

Every submodule but `cpp` is pinned on the `master` of its repository as of 2026-10-27. `cpp` is still on v1.2.4 (2021); its repository has 6 more commits. The component repositories are the source of truth: their own README, ARCHITECTURE and CONTRIBUTING pages describe them in detail.

</details>

The diagram of this overview is in [Architecture](ARCHITECTURE.md#overview).

## Build

There is no build system at the umbrella level: no `Dockerfile`, `docker-compose.yaml`, `CMakeLists.txt`, `Makefile`, `package.json` or `.github/` at the root. Clone with submodules:

```
git clone --recurse-submodules <url>
# or, in an existing clone
git submodule update --init
```

<details>
<summary>Details</summary>

Dependency order: `gen` (produces the image `masagroup/soft.gen`) -> `go` / `ts` (each regenerates part of its sources from `models` with that image) -> tests; `cpp` regenerates with the v1 image `masagroup/soft.generator.cpp`; `vscode` depends on the published npm package `@masagroup/ecore` (`ts`). `models` must be a sibling directory named `models` of the library (`$(realpath ../models)` in the go and ts Makefiles, `../models` in `cpp/build.xml`); the umbrella layout (`soft/go`, `soft/models`) satisfies this.

The Makefiles of `go` and `ts` pin the generator: `SOFT_GEN_VERSION = v2.0.0-alpha.5`, image `masagroup/soft.gen:$(SOFT_GEN_VERSION)`, pulled from Docker Hub or built from `gen` with `make image`.

</details>

### Linux (Docker)

There is no Dockerfile, docker-compose file or Compose profile at the umbrella level, and no `packages-latest` image. Docker is used to run the generator image (`gen/Dockerfile`, a static `soft.gen` binary on `scratch`), golangci-lint, and `go` in a `golang:1.27.3-alpine` container when Go is not installed. `soft.cpp` has no Linux build.

### Make: soft.gen (Windows and Linux)

Go module `github.com/masagroup/soft.gen/v2`, `go 1.27.0`. Run from `gen/` with GNU make (Git Bash on Windows):

```
make build             # bin/soft.gen, with VERSION from version.sh
make test              # go vet + go test (fixtures compared with testdata/golden)
make lint              # golangci-lint in docker
make image image.test  # the image masagroup/soft.gen and the check of its versions
make versions          # the version of each language (go: soft.go version, ts: soft.ts version)
make publish           # docker push: human action only, do NOT run in our setup
```

<details>
<summary>Details</summary>

The version of soft.gen comes from `version.sh` (the `v2.*` tag of the commit, else a Go pseudo-version). The version of a language is the version of the runtime its templates target, in `internal/lang/golang/VERSION` (follows soft.go) and `internal/lang/typescript/VERSION` (follows soft.ts); `make version.go TO=vX.Y.Z` sets one. Tags: `v1.10.0` is the last v1 (Java / Acceleo) tag, then `v2.0.0-alpha.1` to `v2.0.0-alpha.5`. The command is `cmd/soft-gen` up to `v2.0.0-alpha.2` and `cmd/soft.gen` from `v2.0.0-alpha.3`. Details: [gen/README.md](https://github.com/masagroup/soft.gen/blob/master/README.md), templates and how to add a language: [TEMPLATES.md](https://github.com/masagroup/soft.gen/blob/master/TEMPLATES.md).

</details>

### Go: soft.go (Windows and Linux)

Modules `github.com/masagroup/soft.go` (package `ecore`) and `github.com/masagroup/soft.go/codec/{xml,xmi,binary,json,sqlite,defaults}`, plus `test/integration`, all `go 1.27.0`, tied together by `go.work`. Run from `go/`:

```
make generate build test   # what CI runs
make                       # generate fmt lint build test
```

<details>
<summary>Details</summary>

| Target | What it does |
|---|---|
| `generate` | runs `masagroup/soft.gen:v2.0.0-alpha.5 go` on `../models` with `.generator.yaml`: `ecore.ecore` (into `ecore/`), `empty.ecore` and `library.ecore` (into `test/`), and `tournament.ecore` three times (shared interfaces and mocks, typed implementation, reflective implementation) |
| `fmt`, `build`, `test` | `go fmt`, `go build`, `go test -covermode=atomic` in every module of `go.work` |
| `lint` | `golangci/golangci-lint:v2.14.0-alpine` |
| `coverage.console`, `coverage.html` | coverage reports into `coverage/` |

Prerequisites: Docker, sibling `../models`; Go 1.27 or, without it, `go` runs in a `golang:1.27.3-alpine` container. Since v1.13.0 the codecs are separate modules: a consumer must require them (see the migration notes in [go/README.md](https://github.com/masagroup/soft.go/blob/master/README.md)).

</details>

### npm: soft.ts (Windows and Linux)

npm package `ecore/` (`@masagroup/ecore` 1.8.0, ES modules + CJS) and five examples under `examples/` (`library`, `empty`, `tournament`, `inheritance`, `coexistence`), each depending on `file:../../ecore`. Run from `ts/`:

```
make ci     # install + build + test of ecore and every example, no regeneration
make        # the same with generate and format
```

<details>
<summary>Details</summary>

Each component has the targets `<x>.install`, `<x>.generate`, `<x>.format`, `<x>.build`, `<x>.test`, chained by `<x>` and (without generate / format) `<x>.ci`. Generation runs `masagroup/soft.gen:v2.0.0-alpha.5 ts` on `../models` with `.generator.yaml` (`inheritance` uses its own `examples/inheritance/inheritance.ecore`); `coexistence` generates one tournament package shared by a typed and a reflective implementation. Another generator can be given with `make GENERATOR="..."`. npm scripts of `ecore`: `build` = `tsc` (types, ESM and CJS builds), `test` = `vitest run`, `lint` = `oxlint`, `pretty` = remove the unused imports then `prettier`.

</details>

### npm: soft.vscode (Windows and Linux)

VS Code extension `ecorium` 0.1.0 (publisher `masagroup`), built with esbuild. Run from `vscode/`:

```
npm install
npm run compile      # dist/extension.js and dist/webview.js
npm test             # vitest
npm run package      # ecorium-<version>.vsix
```

<details>
<summary>Details</summary>

`@masagroup/ecore` comes from GitHub Packages, which requires a token even for public packages: put `//npm.pkg.github.com/:_authToken=<token with read:packages>` in your user `.npmrc`. `F5` in VS Code launches an Extension Development Host on `samples/`. Releases are `vX.Y.Z` tags; the Release workflow attaches `ecorium-X.Y.Z.vsix` to the GitHub release.

</details>

### Ant: soft.cpp (Windows only)

Unchanged since v1.2.4: Ant (`build.xml`) + CMake/Ninja with MSVC. The `ecore.generate` target runs the v1 generator image `masagroup/soft.generator.cpp` (Java / Acceleo, from the `v1.x` tags of soft.gen) on `../models/ecore.ecore`.

### Code generation: soft.models

No build. The `.ecore` files are the input of the generators. The Eclipse project (`.project`) and its launch configs (`generate.{ecore,library}.{cpp,go,ts}.launch`, `generate.tournament.go.launch`, `generate.tournament-reflective.go.launch`) run the v1 Java generators from the Eclipse IDE; with v2, regenerate through the Makefiles of `go` and `ts`.

## Quickstart

None of the components is a long-running service, and the umbrella produces no executable. No ports are opened.

### Code generator (`gen`, `soft.gen`)

```
docker run --rm -v "$PWD":/gen -w /gen masagroup/soft.gen go -m /gen/model.ecore -o /gen
```

<details>
<summary>Details</summary>

Purpose: turn an `.ecore` metamodel into typed code for the Soft runtime of a language. One image, `masagroup/soft.gen[:<version>]`, holds every language; the language is the first argument (`go` or `ts`). Also available with `go install github.com/masagroup/soft.gen/v2/cmd/soft.gen@<version>`.

```
soft.gen <language> -m <model> -o <folder> [-p key=value]... [-P file]... [-t template]... [-s] [--dry-run] [--diff] [--format] [--manifest=false] [--config file]
```

| Flag | Meaning |
|---|---|
| `-m`, `--model` | (required) input `.ecore` |
| `-o`, `--output` | (required) output folder |
| `-p`, `--property` | repeatable generator property `key=value` |
| `-P`, `--properties` | repeatable property file (`.properties`, `.yaml`, `.yml`, `.json`), e.g. `.generator.yaml` |
| `-t`, `--template` | repeatable template to run (default: the templates marked `*` in `soft.gen <language> -h`) |
| `-s`, `--silent` | print nothing but failures |
| `--dry-run` | list the files without writing them |
| `--diff` | print the differences with the existing files, write nothing, exit 1 on any difference |
| `--format` | run the external formatter of the language on the generated files |
| `--manifest` | write `.soft.gen.yaml` in every folder of generated files (default true) |
| `-c`, `--config` | read the options from a YAML, JSON or TOML file |
| `-v`, `--version` | `soft.gen version: <v>`, or with a language the version of the runtime it targets |

Every option but `-p` also has an environment variable (`SOFT_GEN_MODEL`, `SOFT_GEN_OUTPUT`, ...). The command line, the templates, the properties and the annotation keys are the ones of v1, and a file generated by v2 is identical to the v1 one once formatted, but for its first line (`// Code generated by soft.gen go. DO NOT EDIT.`). `soft.gen` exits 1 on any failure.

</details>

### Libraries and tools

| Component | Artifact |
|---|---|
| `soft.go` | Go packages `github.com/masagroup/soft.go/ecore` and the codec modules `github.com/masagroup/soft.go/codec/*`; no `main` |
| `soft.ts` | npm package `@masagroup/ecore` (GitHub Packages) |
| `soft.cpp` | shared library `ecore` and static library `ecore.static` (Windows naming `ecore-vc<toolset>-mt[-gd]`, static prefix `lib`), test binary `ecore.tests`; distribution `ecore-<ver>-win64.zip` |
| `soft.vscode` | VS Code extension `ecorium-<version>.vsix`: tree and properties editor for `.ecore` files, default editor for `*.ecore` |

## Interfaces and access methods

No service is produced by this repository (library, data and tooling): see the build and the dependencies.

## Documentation and guides

- [Architecture](ARCHITECTURE.md): position in the ecosystem, repository layout, internal structure, how it works (diagrams), dependencies.
- [Contributing](CONTRIBUTING.md): development environment, Git and Jira workflow, conventions, CI/CD.
- [Known issues and open questions](https://claude.ai/artifact/UfvKmky7e8LwEXf192hQjQ#soft): the private artifact that holds them (every page of the wiki).
- Related repositories: [soft.cpp](https://github.com/masagroup/soft.cpp), [soft.gen](https://github.com/masagroup/soft.gen), [soft.go](https://github.com/masagroup/soft.go), [soft.models](https://github.com/masagroup/soft.models), [soft.ts](https://github.com/masagroup/soft.ts), [soft.vscode](https://github.com/masagroup/soft.vscode), [soft.sync.go](https://github.com/masagroup/soft.sync.go), [soft.sync.ts](https://github.com/masagroup/soft.sync.ts), [sw.poney](https://github.com/masagroup/sw.poney); v1 only: [soft.acceleo](https://github.com/masagroup/soft.acceleo).
- Transverse pages: [Build on Windows](https://github.com/masagroup/wiki/wiki/build-windows), [Build on Linux](https://github.com/masagroup/wiki/wiki/build-linux), [CI/CD pipelines](https://github.com/masagroup/wiki/wiki/cicd-pipelines), [Ecosystem today](https://github.com/masagroup/wiki/wiki/ecosystem-current), [Ecosystem next gen](https://github.com/masagroup/wiki/wiki/ecosystem-nextgen), [esync and ecore](https://github.com/masagroup/wiki/wiki/esync-ecore), [Glossary](https://github.com/masagroup/wiki/wiki/glossary).
- Repository on GitHub: [masagroup/soft](https://github.com/masagroup/soft).
- The same documentation in the SWORD wiki: [soft](https://github.com/masagroup/wiki/wiki/soft), [architecture](https://github.com/masagroup/wiki/wiki/soft-architecture), [contributing](https://github.com/masagroup/wiki/wiki/soft-contributing).
