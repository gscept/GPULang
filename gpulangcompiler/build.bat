@Echo off
Pushd "%~dp0.."
fips.cmd build gpulangcompiler-windows
popd
