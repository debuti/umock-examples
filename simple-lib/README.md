# UMock examples: Simple lib

This example illustrates the ease of use of UMock combined with CMocka.

## Usage
Build and test with
```
rm -rf build 2> /dev/null
mkdir build
pushd build
cmake .. -DTESTING=ON && cmake --build .
./mylib_test
popd
```

Check that the mocks are not active in a not testing build.
```
rm -rf build 2> /dev/null
mkdir build
pushd build
cmake .. && cmake --build .
./mylib_app
popd
```