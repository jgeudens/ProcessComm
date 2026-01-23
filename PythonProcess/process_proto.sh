OUTPUT=.
python -m grpc_tools.protoc -I ../proto --python_out=$OUTPUT --grpc_python_out=$OUTPUT --mypy_out=$OUTPUT ../proto/echo/v1/echo.proto
