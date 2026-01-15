from concurrent import futures

import grpc

import echo_pb2
import echo_pb2_grpc


class EchoService(echo_pb2_grpc.EchoServiceServicer):
    """
    Implements the EchoService defined in echo.proto.
    """

    def Echo(self, request, context):
        print(f"Received: {request.message}")
        return echo_pb2.EchoReply(message=request.message)


def serve():
    """
    Starts the gRPC server and listens for incoming requests.
    """
    server = grpc.server(
        futures.ThreadPoolExecutor(max_workers=4),
        options=[
            ("grpc.so_reuseport", 1),
            ("grpc.max_send_message_length", 10 * 1024 * 1024),
            ("grpc.max_receive_message_length", 10 * 1024 * 1024),
        ],
    )

    echo_pb2_grpc.add_EchoServiceServicer_to_server(EchoService(), server)

    server.add_insecure_port("127.0.0.1:50051")
    server.start()

    print("Python gRPC server listening on 127.0.0.1:50051")
    server.wait_for_termination()


if __name__ == "__main__":
    serve()
