{ pkgs ? import <nixpkgs> {} }:
  pkgs.mkShell {
    nativeBuildInputs = with pkgs.buildPackages; [ 
      cargo
      rustc
      rust-analyzer
      bacon
    ];
}
