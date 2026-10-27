# soft: contributing

> How to set up a development environment, the Git and Jira workflow and the conventions of `soft`. Back to the [README](README.md) · [Architecture](ARCHITECTURE.md)

## Development environment

The build of the repository is described in the [README](README.md#build); the tooling of the ecosystem is on the transverse pages:

- Windows, Ant and sw.poney: Apache Ant, JDK 8, Visual Studio 2022 Professional with the MSVC v140 toolset and the Windows SDK 10.0.19041.0, `gh` authenticated, access to `\\file1` or a local `sw.poney` clone in `PONEY_HOME`; initialise the MSVC environment before any `ant` (see [Build on Windows](https://github.com/masagroup/wiki/wiki/build-windows)).
- Linux and Docker Compose: Docker with BuildKit, the GitHub PAT file named by `SECRET_GITHUB_PAT` (`~/.masabot/github.pat`) or the SSH key of `SECRET_GITHUB_SSH` for the private Go modules and npm packages, the profiles `prod`, `test`, `pack` and `dev` (see [Build on Linux](https://github.com/masagroup/wiki/wiki/build-linux)); never push an image, the CI does it.
- Go: the version of `go.mod`, `GOPRIVATE=github.com/masagroup/*` with `gh auth setup-git` or an SSH `insteadOf` rewrite for the private modules; `staticcheck` installed by the build.
- Node and yarn: the Node version of the Dockerfile or of `package.json` (`engines`), yarn with the lockfile of the repository, a `%USERPROFILE%\.npmrc` with a `read:packages` PAT for `npm.pkg.github.com` (`@masagroup` scope).
- C++: the Windows toolset above, or the development container of the repository on Linux (`docker compose --profile dev up -d`, then `cmake` and `ninja` inside it).
- Editor rules for Claude Code: the shared `claude/` folder of [cicd](https://github.com/masagroup/cicd) installs the same `CLAUDE.md` for every developer (rules, roles, build pages, conventions).

## Git and Jira workflow

- The organisation repository `masagroup/soft` is the reference (CI, releases); work on a fork or on a branch of it, never directly on `master` nor on a release branch (`6.28`...).
- Branch name: `<TICKET>-<short-description>` (for example `SWNG-421-tests-and-fixes`), created from the target branch of the ticket.
- Commit message: `<type>(<scope>): <TICKET-ID>: <imperative description in English>`, with `type` in `feat`, `fix`, `refactor`, `test`, `build`, `ci`, `perf`, `docs`, `chore`; the body says what and why, and the CI impact when there is one. One commit is one coherent change; no `Co-Authored-By` nor other automatic signature.
- Pull request: masabot reads the Jira keys (`SW`, `SWNG`, `IO`, `AI`, `GEFORA`, `COA`) in the title, the branch, the body and the commits, comments the Jira issue and moves it (*In Progress*, *In Review* with `[DONE]`, *In QA* or *In Validation* after the merge). The PR is built rebased on its base branch; the comment commands `/rebase`, `/rebase-all` and `/automerge` are available. See [CI/CD pipelines](https://github.com/masagroup/wiki/wiki/cicd-pipelines).
- Never push to `master` by hand, never create a release or push an image: the CI does it.

## Conventions

- Code, comments, commit messages and file names in English; documentation of the repository in English.
- Formatting of the repository (`clang-format`, `gofmt`/`goimports`, `prettier`/`eslint`) on the lines you touch; no reformatting of a whole file for a one-line change; no dead code, no `TODO` without a ticket; logs at the right level, never a secret.
- C++: C++17 as accepted by both MSVC v140 (`vc140_x64`) and recent GCC/Clang; never replace a forward declaration by an include in a header; the frozen libraries (Boost 1.56, Qt 4.8.7, protobuf 2.x on the old gen) bound what is available; determinism of the simulation (no iteration over unordered containers for observable results, no dependency on addresses, randomness and clock from the simulation only); modern CMake, `std::filesystem`, no hard-coded path.
- Go: the version of `go.mod` and of the Docker image, `GOPRIVATE=github.com/masagroup/*`; `go vet`, `staticcheck`, `gofmt` and `go test -race ./...` green before a commit; wrapped errors (`fmt.Errorf("...: %w", err)`), no `log.Fatal` outside `main`, clean stop on `SIGINT` and `SIGTERM`, timeouts on every network I/O; REST APIs from an OpenAPI contract (`oapi-codegen`).
- TypeScript: Angular with Nx, the `sw.design-system` components before custom ones, yarn with a frozen lockfile, the lint and the tests of the repository, no new `any`, no dependency added without a reason.
- Shared contracts (`.proto`, `.ecore`, OpenAPI, exported Go modules, file formats) are changed with the consumers in mind: grep the workspace, state the compatibility strategy, update the dependants.
- Documentation: this README set (README, ARCHITECTURE, CONTRIBUTING, README.LEGACY) is the source of the [SWORD wiki](https://github.com/masagroup/wiki/wiki/soft); keep the metadata block at the top of the README up to date (the `apps` list feeds the Interfaces section).

## CI/CD

The umbrella has no `.github/` directory. Each submodule has its own self-contained workflow (old-style actions: `actions/checkout@v2`, `setup-go@v2`, `setup-node@v2`); none uses a reusable workflow from `masagroup/cicd` and none reads repository/organization variables. `cpp` and `models` have no `.github` directory at the pinned commit. See [cicd](https://github.com/masagroup/cicd) and the ecosystem wiki page [CI/CD pipelines](https://github.com/masagroup/wiki/wiki/cicd-pipelines) for the shared workflows used by other repositories.

| Submodule | Workflow file | Triggers | Steps | Artifacts |
|---|---|---|---|---|
| gen | `gen/.github/workflows/build_and_test.yml` | PR (opened/synchronize/reopened/ready_for_review) to master, push master, manual | ubuntu-latest: `make generators`, `make tests` | none from the workflow (images / dist exports come from local `make publish` / `make dist`) |
| go | `go/.github/workflows/build_and_test.yaml` | same | Go 1.20.x; checkout `go` and `masagroup/soft.models@a6b7a0c1` as siblings; `make generate build test` in `go/` | none |
| ts | `ts/.github/workflows/build_and_test.yaml` | same | Node 17.2.0; checkout `ts` and `masagroup/soft.models@f487e7c8`; `sudo make ci` in `ts/` | none |
| cpp | none | | `ant` is run manually on Windows | `dist/ecore-<ver>-win64.zip` from `ecore.dist` |

Release artifacts mentioned by the READMEs but not produced by any workflow in this tree: generator jars as GitHub release assets (`soft.generator.<lang>-<version>.jar`), Docker Hub images `masagroup/soft.generator.{cpp,go,ts}`, npm package `@masagroup/ecore`, Go module via `go get github.com/masagroup/soft.go`, C++ VC 2019 binaries on the GitHub releases page. How they are really published is UNVERIFIED.

## Known issues and open questions

They are not kept in the repository: see the [private artifact](https://claude.ai/artifact/UfvKmky7e8LwEXf192hQjQ#soft) that holds them for every page of the wiki.
