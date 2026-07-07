# Publishing KenwoodXS As A Standalone Repository

This directory is intended to become the root of the standalone repository.

## One-Time Extraction

From the current `cec2xs` repository, copy the library directory into a new
repository:

```sh
mkdir KenwoodXS
cp -R lib/KenwoodXS/. KenwoodXS/
cd KenwoodXS
git init
git add .
git commit -m "Initial standalone KenwoodXS library"
git branch -M main
git remote add origin git@github.com:gaweueu/KenwoodXS.git
git push -u origin main
```

## Before The First Public Release

1. Review `library.properties`, `library.json`, `LICENSE`, and `NOTICE`.
2. Compile the included examples in Arduino IDE or PlatformIO.
3. Confirm the image links in `README.md` and `manual.md` render correctly on GitHub.
4. Create and push a version tag:

```sh
git tag v1.0.0
git push origin v1.0.0
```

## Arduino Library Manager

To submit the library to Arduino Library Manager, the standalone repository
needs:

- a public GitHub repository,
- a semantic version tag such as `v1.0.0`,
- a valid `library.properties` file,
- Apache-2.0 `LICENSE` and `NOTICE` files,
- source files and examples committed to the repository.

After that, submit the repository URL to the Arduino Library Manager index.
