 # Journal

## Initial experiment (grpc and localhost)

### Qt

* https://doc.qt.io/qt-6/qtgrpc-index.html
* Prerequisite
  * `sudo dnf install protobuf-compiler protobuf-devel`
* Proto

## Python

* Generate
  * `python -m grpc_tools.protoc -I ../proto --python_out=. --grpc_python_out=. ../proto/echo.proto`
