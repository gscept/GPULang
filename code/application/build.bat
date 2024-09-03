@Echo off
Pushd "%~dp0.."
fips.cmd build anyfxcompiler-windows
popd
