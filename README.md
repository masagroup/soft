<!--
sword:
  schema: 1
  name: soft
  family: ecore-esync
  kind: build-tooling
  generation: shared
  shipped: false
  languages: [Java, C++, Go, TypeScript, Ecore]
  build: [ant, docker, go, cmake, npm, make, maven]
  ng: [ecore-model]
  apps: []
-->

# soft

> SOFT ("Sword Object Facilities") is the umbrella repository that aggregates, as git submodules, MASA's own Ecore implementation in C++, Go and TypeScript, the Acceleo code generators and the `.ecore` metamodels; it is the foundation of the ecore model code generation chain used by the SWORD ecosystem.

| | |
|---|---|
| **Family** | Ecore / esync |
| **Languages** | Umbrella itself: none. Submodules: C++17 (`cpp`), Go (`go`), TypeScript (`ts`), Java + Acceleo MTL (`gen`), Ecore XMI models (`models`) |
| **Build systems** | none at the umbrella level · per submodule: Make + Docker BuildKit (`gen`, `go`, `ts`) · Ant + CMake/Ninja (`cpp`, Windows/MSVC only) · Go · npm · Maven (inside the generator images) |

**[Architecture](ARCHITECTURE.md)** · [Contributing](CONTRIBUTING.md) · [known issues and open questions](https://claude.ai/artifact/UfvKmky7e8LwEXf192hQjQ#soft) · [repository on GitHub](https://github.com/masagroup/soft)

## Overview

`soft` contains only `.gitmodules`, `README.md`, `LICENSE` and five gitlinks. All real content lives in the submodules, checked out at pinned commits. License: Mozilla Public License 2.0 (source headers: "Copyright(c) 2021 MASA Group").

<details>
<summary>More</summary>

SOFT implements the core of the Eclipse Modeling Framework (EObject / EClass / EPackage / EResource, notifications, XMI / XML / binary serialization) in three languages. It is bootstrapped: the same Ecore metamodel (`models/ecore.ecore`) is the input of the generators, which emit the generated half of the Ecore library itself, while a hand-written half (`Basic*`, `*Ext`, resources, codecs) lives in each library.

The libraries do no network I/O and expose no ports. The only runnable programs are the three code generators (command-line tools packaged as Docker images `masagroup/soft.generator.{go,cpp,ts}`) and the test executables. Who consumes SOFT is not visible from this repository (see Open questions).

### Submodules and pinned commits

| Path | `.gitmodules` URL | Pinned commit | Tag at pin | Role |
|---|---|---|---|---|
| `gen` (section named `java`, path `gen`) | https://github.com/masagroup/soft.gen.git | `d96e5d10` (2023-05-23) | v1.7.0 | Java/Acceleo generators for cpp, go, ts ([soft.gen](https://github.com/masagroup/soft.gen)) |
| `cpp` | https://github.com/masagroup/soft.cpp.git | `bd217da2` (2021-05-03) | v1.2.4 | C++ ecore library ([soft.cpp](https://github.com/masagroup/soft.cpp)) |
| `go` | https://github.com/masagroup/soft.go.git | `8d0a2b27` (2023-04-27) | v1.7.0 | Go ecore library ([soft.go](https://github.com/masagroup/soft.go)) |
| `ts` | https://github.com/masagroup/soft.ts.git | `3047b6e3` (2021-12-16) | v1.0.4 | TypeScript ecore library ([soft.ts](https://github.com/masagroup/soft.ts)) |
| `models` | https://github.com/masagroup/soft.models.git | `a6b7a0c1` (2023-01-11) | v1.7.0 | `.ecore` metamodels used to bootstrap the libraries ([soft.models](https://github.com/masagroup/soft.models)) |

HEAD of the umbrella: `c80fbf86` "Merge pull request #84 from SokaDance/release-v1.7.0", 2023-05-23 (last bump: "update all subprojects version to 1.7.0"). The checked-out submodules point to `git@github.com:masagroup/soft.<x>.git` and are in detached HEAD.

### Pin staleness

The umbrella is dormant. Comparison with the standalone sibling working copies (`soft.<x>`, branch master):

| Component | Pinned in `soft` | Sibling HEAD | Commits ahead of pin | Latest tag in sibling |
|---|---|---|---|---|
| soft.go | `8d0a2b27` (v1.7.0) | `4d866ae` (2026-10-14) | 723 | v1.11.1 (`go.mod`: `go 1.27.0`) |
| soft.gen | `d96e5d10` (v1.7.0) | `106258a` (2026-10-14) | 313 | v1.9.0 |
| soft.ts | `3047b6e3` (v1.0.4) | `1499ba7` (2026-10-14) | 339 | v1.6.1 |
| soft.cpp | `bd217da2` (v1.2.4) | `584851f` (2023-12-15) | 6 | v1.2.4 |
| soft.models | `a6b7a0c1` (v1.7.0) | `f0dd72e` (2024-09-19) | 24 | v1.8.0 |

The component repositories are the source of truth; anything newer than go v1.7.0, gen v1.7.0 or ts v1.0.4 must be read from their own wiki pages, not from this one.

</details>

The diagram of this overview is in [Architecture](ARCHITECTURE.md#overview).

## Build

There is no build system at the umbrella level: no `build.xml`, `Dockerfile`, `docker-compose.yaml`, `CMakeLists.txt`, `Makefile`, `package.json` or `.github/` at the root. Clone with submodules:

```
git clone --recurse-submodules <url>
# or, in an existing clone
git submodule update --init
```

<details>
<summary>Details</summary>

Dependency order (derived from the Makefiles): `gen` (produces the generator Docker images) -> `go` / `ts` / `cpp` (each regenerates part of its sources from `models` using a generator image) -> tests. `models` must be a sibling directory named `models` of the library (`$(realpath ../models)` in the go and ts Makefiles, `../models` in `cpp/build.xml`); the umbrella layout (`soft/go`, `soft/models`) satisfies this.

</details>

### Linux (Docker)

There is no Dockerfile, docker-compose file or Compose profile at the umbrella level, and `soft.cpp` has no Linux/Docker build at the pin (UNVERIFIED for newer commits). The Linux-side tooling is the generator images built by `gen/Makefile` (see the Make subsection below). No `packages-latest` image is produced by `soft.cpp` (the only component with a C++ build); no Docker-as-Artifact publication is visible for the other components either (UNVERIFIED).

### Make: soft.gen (Windows and Linux)

Make + Docker BuildKit (`export DOCKER_BUILDKIT=1`); the Dockerfiles use `# syntax = edrevo/dockerfile-plus` and `INCLUDE+ Dockerfile-base`. Run from `gen/`:

```
make generators        # builds the 3 images masagroup/soft.generator.{go,cpp,ts}
make tests             # checks each image reports the version declared in the Makefile
make                   # default = generators tests
make dist              # exports each generator to dist/soft.generator.<lang>-<version>.tar.gz (docker create + docker cp)
make distclean
make versions          # re-stamps versions in pom.xml, MANIFEST.MF and Dockerfile-<lang> via klakegg/saxon + sed
make publish           # docker push of the 3 images: do NOT run in our setup (no push to DockerHub)
```

<details>
<summary>Details</summary>

`gen/Makefile` variables: `soft.generators.version := 1.6.0`, `soft.generator.common.version := 1.3.0`, `soft.generator.cpp.version := 1.2.4`, `soft.generator.go.version := 1.7.0`, `soft.generator.ts.version := 1.0.5`.

Dockerfile stages (`Dockerfile-base`, `Dockerfile-{go,cpp,ts}`):

| Stage | Base image | Content |
|---|---|---|
| `build` (Dockerfile-base) | `maven:3.8.1-openjdk-16-slim` | `COPY . /home/soft.gen/src`; `mvn -f soft.acceleo/pom.xml -s settings.xml -Pdocker clean install` then `mvn -f soft.generators/pom.xml -s settings.xml -Pdocker clean verify` (BuildKit cache mount on `/root/.m2`) |
| `jvm` | `openjdk:16-alpine` | `jdeps` computes the needed modules, `jlink` builds a trimmed JRE `/java-generator` (cpp adds `jdk.zipfs`) |
| final | `alpine` | jars in `/usr/share/soft.generator.<lang>/`, JRE in `/usr/lib/jvm/java-generator`, `ENTRYPOINT ["/usr/lib/jvm/java-generator/bin/java","-jar","/usr/share/soft.generator.<lang>/soft.generator.<lang>-<ver>.jar"]` |

There is no `FROM masagroup/*` line and no docker-compose file in `gen`. Jar versions are hard-coded in the Dockerfiles (go 1.7.0, cpp 1.2.4, ts 1.0.5) and re-stamped by `make versions`.

</details>

### Maven: soft.gen (Windows and Linux)

Maven structure: `gen/pom.xml` is an empty `<pom/>`. `gen/soft.acceleo/pom.xml` (groupId `soft.acceleo`, 1.0.0, Java 1.8) provides `soft.acceleo.AcceleoURIHandler`. `gen/soft.generators/pom.xml` (1.6.0, packaging `pom`) has modules `soft.generator.common` (1.3.0), `.cpp`, `.go`, `.ts` and two profiles: `docker` (output to `../../../out/${project.artifactId}`, uses `soft.acceleo.AcceleoURIHandler`) and `eclipse` (default, output `.target`, Eclipse URI handler). Acceleo templates are compiled with `org.eclipse.acceleo.maven` 3.6.4; runtime depends on `org.eclipse.acceleo.*` `3.7.11-SNAPSHOT`, resolved from Maven Central, `https://repo.eclipse.org/content/repositories/acceleo` and `https://maven.pkg.github.com/masagroup/soft.acceleo` (server id `github`, credentials in `gen/settings.xml`).

### Go: soft.go (Windows and Linux)

Module `github.com/masagroup/soft.go`, `go 1.20` (`go/go.mod`). Make targets, run from `go/`:

<details>
<summary>Details</summary>

| Target | What it does |
|---|---|
| `all` | `generate fmt lint build test` |
| `generate` | 4 calls of `docker run --rm -v $(CURDIR):/pwd -v $(realpath ../models):/models -w /pwd masagroup/soft.generator.go -m /models/<x>.ecore -o /pwd/<out> -P /pwd/generator.properties` for `ecore.ecore` (out = repo root, files land in `ecore/`), then `library.ecore`, `tournament.ecore`, `empty.ecore` (out = `test`) |
| `fmt` | `go fmt ./...` |
| `lint` | `docker run golangci/golangci-lint:v1.51.1 golangci-lint run` |
| `build` | `go build ./...` |
| `test` | `go test -covermode=atomic ./...` |
| `coverage.console`, `coverage.html` | coverage reports into `coverage/` |

Copy-pastable, from `go/` (exactly what CI runs): `make generate build test`. Prerequisites: Go in PATH (the Makefile errors out otherwise), Docker, the image `masagroup/soft.generator.go` (pull from Docker Hub or build from `gen`), sibling `../models`.

Generated Go code is committed for the `ecore` package (`ecore/*_impl.go`, `*_mock.go`, mock tests) but git-ignored under `go/test/{empty,library,tournament}` (per-directory `.gitignore` ignoring `*.go` and `go.sum`; `go/test/library/.gitignore` keeps hand-written `econtentadapter_test.go`, `serialization_test.go`, `writer_ext*.go`). The tests under `go/test/*` compile only after `make generate`.

</details>

### npm: soft.ts (Windows and Linux)

Two npm packages: `ecore/` (`@masagroup/ecore` 1.0.4) and `library/` (`library` 1.0.0, depends on `"@masagroup/ecore": "file:../ecore"`). Make targets, run from `ts/`:

<details>
<summary>Details</summary>

| Target | Chain |
|---|---|
| `all` | `ecore library` |
| `ecore` | `ecore.install ecore.generate ecore.format ecore.build ecore.test` |
| `ecore.ci` | `ecore.install ecore.generate ecore.build ecore.test` |
| `library` | `library.install library.generate library.format library.build library.test` |
| `ci` | `ecore.ci library.ci` |
| `versions` | `sed` stamps `ECORE_TS_VERSION := 1.0.4` into `ecore/package.json` |

npm scripts (both packages): `build` = `tsc`, `test` = `jest`, `pretty` = `prettier --write src/**/*.ts`. Generation: `docker run --rm -v $(CURDIR):/pwd -v $(realpath ../models):/models -w /pwd masagroup/soft.generator.ts -m /models/<file> -o /pwd/ -P /pwd/generator.properties` (the ecore call adds `-t !generateModule`). Copy-pastable, from `ts/`: `make ecore.ci` or `make ci`.

Defect in `ts/Makefile`: the rule after `.PHONY: library.ci` is written as `ecore.ci: library.install library.generate library.build library.test`, i.e. it re-declares `ecore.ci` (adding library prerequisites) and leaves `library.ci` without a recipe. `make ci` still works by accident.

</details>

### Code generation: soft.models

No build. Eclipse project (`.project`) with seven launch configs (`generate.{ecore,library}.{cpp,go,ts}.launch`, `generate.tournament.go.launch`) that run the generators from the Eclipse IDE (arguments such as `-m ../../../models/ecore.ecore -o ../../../go`).

## Quickstart

None of the five components is a long-running service, and the umbrella produces no executable. No ports are opened and no environment variable is read by the generators (Maven reads the credentials of server id `github` from `gen/settings.xml`).

### Code generators (`gen/soft.generators`)

```
docker run --rm -v "$PWD":/gen -w /gen masagroup/soft.generator.go -s -m /gen/model.ecore -o /gen
```

<details>
<summary>Details</summary>

Purpose: turn an `.ecore` metamodel into typed code. Packaged as Docker images `masagroup/soft.generator.go`, `masagroup/soft.generator.cpp`, `masagroup/soft.generator.ts` (the gen README links the three Docker Hub pages).

| Executable | Entry point | Notes |
|---|---|---|
| `soft.generator.go` | `soft.generator.go.Generate` (`soft.generators/soft.generator.go/src/soft/generator/go/Generate.java`) | NS URI `http://net.masagroup/soft/2019/GenGo`; default templates `generateModel, generateTests, generateMocks, generateMocksTests`; main module `/soft/generator/go/generate` |
| `soft.generator.cpp` | `soft.generator.cpp.Generate` | NS URI `http://net.masagroup/soft/2018/GenCpp`; templates `generateModel, generateLibraryCMakeSources, generateLibraryCMakeProject, generateTests, generateTestsCMakeDependencies, generateTestsCMakeSources, generateTestsCMakeProject`; ships `BoostTest.cmake`, `BoostTestAddTests.cmake`, `FindGMock.cmake`, `FindTurtle.cmake` as resources |
| `soft.generator.ts` | `soft.generator.ts.Generate` | NS URI `http://net.masagroup/soft/2020/GenTS`; templates `generateModel, generateModule, generateTests` |
| shared library | `soft.generator.common.Generator`, `GenerateUtils.java` + `*.mtl` | extends Acceleo `AbstractAcceleoGenerator`; picocli 4.6.1 CLI |

Usage example from the gen README:

CLI (identical for the three generators, `soft.generator.common/src/soft/generator/common/Generator.java`):

| Flag | Meaning |
|---|---|
| `-h`, `--help` | usage |
| `-v`, `--version` | prints "<Implementation-Title> version: <Implementation-Version>" from the jar manifest |
| `-m`, `--model=<model>` | (required) input `.ecore` |
| `-o`, `--output=<folder>` | (required) output folder |
| `-p`, `--property=<property=value>` | repeatable generator property |
| `-P`, `--properties=<propertyfile>` | repeatable property file (e.g. `generator.properties`) |
| `-s`, `--silent` | print nothing but failures |
| `-t`, `--template=<template>` | regex selection of templates; prefix `!` excludes (e.g. `-t !generateModule`) |

Template selection (`Generator.initialize`): each `-t` is a regex matched against the default template list; a plain pattern keeps only the matches, a `!` pattern removes the matches. Properties `nsURI` and `templates` are injected automatically. `generator.properties` in `cpp/`, `go/`, `ts/` only defines `fileNotes` (the MPL header comment inserted in generated files).

Annotation keys understood by the generators (seen in `models/*.ecore`, sources `.../2018/GenCpp`, `.../2019/GenGo`, `.../2020/GenTS`): `baseType, const, constraints, default, exported, extension, factory, featureDelegation, group, include, instanceTypeName, issetVisibility, kind, lazy, name, namespace, operationName, rootExtendsClass, rootExtendsInterface, setVisibility, supressAnnotations (sic), unsetVisibility, visibility`.

</details>

### Libraries (no executables)

| Component | Artifact |
|---|---|
| `soft.cpp` | shared library `ecore` and static library `ecore.static` (Windows naming `ecore-vc<toolset>-mt[-gd]`, static prefix `lib`), test binary `ecore.tests`; distribution `ecore-<ver>-win64.zip` |
| `soft.go` | Go package `github.com/masagroup/soft.go/ecore`; no `cmd/` directory, no `main` |
| `soft.ts` | npm package `@masagroup/ecore` (`main: dist/index.js`, `typings: dist/index.d.ts`) |

## Interfaces and access methods

No executable is produced by this repository (library, contract, data or tooling): see the build and the dependencies.

## Documentation and guides

- [Architecture](ARCHITECTURE.md): position in the ecosystem, repository layout, internal structure, how it works (diagrams), dependencies.
- [Contributing](CONTRIBUTING.md): development environment, Git and Jira workflow, conventions, CI/CD.
- [Known issues and open questions](https://claude.ai/artifact/UfvKmky7e8LwEXf192hQjQ#soft): the private artifact that holds them (every page of the wiki).
- Related repositories: [soft.acceleo](https://github.com/masagroup/soft.acceleo), [soft.cpp](https://github.com/masagroup/soft.cpp), [soft.gen](https://github.com/masagroup/soft.gen), [soft.go](https://github.com/masagroup/soft.go), [soft.models](https://github.com/masagroup/soft.models), [soft.ts](https://github.com/masagroup/soft.ts), [sw.poney](https://github.com/masagroup/sw.poney).
- Transverse pages: [Build on Windows](https://github.com/masagroup/wiki/wiki/build-windows), [Build on Linux](https://github.com/masagroup/wiki/wiki/build-linux), [CI/CD pipelines](https://github.com/masagroup/wiki/wiki/cicd-pipelines), [Ecosystem today](https://github.com/masagroup/wiki/wiki/ecosystem-current), [Ecosystem next gen](https://github.com/masagroup/wiki/wiki/ecosystem-nextgen), [Glossary](https://github.com/masagroup/wiki/wiki/glossary).
- Repository on GitHub: [masagroup/soft](https://github.com/masagroup/soft).
- The same documentation in the SWORD wiki: [soft](https://github.com/masagroup/wiki/wiki/soft), [architecture](https://github.com/masagroup/wiki/wiki/soft-architecture), [contributing](https://github.com/masagroup/wiki/wiki/soft-contributing).
