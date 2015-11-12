mkdir -p out/debug
pushd out/debug
cmake -DCMAKE_BUILD_TYPE=debug ../.. -G "CodeBlocks - Unix Makefiles"
popd

mkdir -p out/release
pushd out/release
cmake -DCMAKE_BUILD_TYPE=release ../.. -G "CodeBlocks - Unix Makefiles"
popd

