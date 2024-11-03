{ stdenv, ncurses }:
stdenv.mkDerivation {
  name = "roguelike-c";
  version = "1.0.0";
  installPhase = ''
    mkdir -p $out/bin
    make install
    mv main $out/bin
  '';
  src = ./.;
  buildInputs = [ ncurses ];
}
