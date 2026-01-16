"""gRPC server setup and service registration."""

import logging
from concurrent import futures

import grpc

import echo_pb2_grpc
from src.services.echo import EchoService

logging.basicConfig(level=logging.INFO)
logger = logging.getLogger(__name__)


def create_server(
    *,
    max_workers: int = 4,
    max_message_size: int = 10 * 1024 * 1024,
) -> grpc.Server:
    """Creates a gRPC server with specified parameters."""
    return grpc.server(
        futures.ThreadPoolExecutor(max_workers=max_workers),
        options=[
            ("grpc.max_send_message_length", max_message_size),
            ("grpc.max_receive_message_length", max_message_size),
        ],
    )


def register_services(server):
    """Registers services with the gRPC server.

    :param server: Description
    """
    echo_pb2_grpc.add_EchoServiceServicer_to_server(
        EchoService(),
        server,
    )


def serve(address: str = "127.0.0.1:50051"):
    """Starts the gRPC server and listens for incoming requests.

    :param address: Description
    :type address: str
    """
    server = create_server()

    register_services(server)

    server.add_insecure_port(address)
    server.start()

    logger.info("gRPC server listening on %s", address)
    server.wait_for_termination()


if __name__ == "__main__":
    serve()
