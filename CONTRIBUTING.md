# Contributing

Thanks for helping improve KenwoodXS.

Unless explicitly stated otherwise, contributions are submitted under the
Apache License, Version 2.0.

## Development Notes

- Keep the library usable as a standalone Arduino library.
- Put public examples under `examples/<ExampleName>/<ExampleName>.ino`.
- Update `README.md` and `manual.md` when adding or confirming command bytes.
- Prefer confirming protocol behavior with real hardware before changing command names or notes.

## Release Checklist

1. Update `version` in `library.properties` and `library.json`.
2. Check that all examples compile.
3. Confirm `README.md` installation instructions still match the repository layout.
4. Add a Git tag matching the release version, for example `v1.0.0`.
