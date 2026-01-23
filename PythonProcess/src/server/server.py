"""Encapsulated gRPC server implementation.

Provides a `Server` class that manages server creation, service
registration, starting (foreground or background thread), stopping,
and joining.
"""

import logging
from concurrent import futures

import grpc

from echo.v1 import echo_pb2_grpc
from src.services.echo import EchoService

logger = logging.getLogger(__name__)


class Server:
    """Encapsulated gRPC server."""

    def __init__(
        self,
        address: str = "127.0.0.1:50051",
        max_workers: int = 4,
        max_message_size: int = 10 * 1024 * 1024,
    ) -> None:
        """Initialize the gRPC server."""
        self.address = address
        self.max_workers = max_workers
        self.max_message_size = max_message_size

        self._server: grpc.Server = grpc.server(
            futures.ThreadPoolExecutor(max_workers=self.max_workers),
            options=[
                ("grpc.max_send_message_length", self.max_message_size),
                ("grpc.max_receive_message_length", self.max_message_size),
            ],
        )

        self._register_services()

    def _register_services(self) -> None:
        echo_pb2_grpc.add_EchoServiceServicer_to_server(EchoService(), self._server)

    def start(self):
        """Start the gRPC server (non-threaded).

        This starts the server and returns the underlying `grpc.Server`.
        Callers may block on `block_until_termination()` if they want
        the process to wait for shutdown.
        """
        self._server.add_insecure_port(self.address)
        self._server.start()

        logger.info("gRPC server listening on %s", self.address)

    def block_until_termination(self) -> None:
        """Block until the server is terminated."""
        self._server.wait_for_termination()

    def stop(self, grace: float = 0) -> None:
        """Stop the server with the given grace period."""
        self._server.stop(grace)
