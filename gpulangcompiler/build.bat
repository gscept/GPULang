@Echo off
Pushd "%~dp0.."
fips.cmd build win64
popd
