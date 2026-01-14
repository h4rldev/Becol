{pkgs ? import <nixpkgs> {}}:
with pkgs;
  mkShell {
    name = "becol";
    description = "A basic interpreted language";
    packages = [
      gnumake
      bear

      clang-tools
    ];
  }
