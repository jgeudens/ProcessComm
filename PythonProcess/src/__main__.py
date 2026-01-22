"""Entrypoint that starts the encapsulated gRPC `Server` class."""

import logging
import sys

from src.server.server import Server

logging.basicConfig(level=logging.INFO)
logger = logging.getLogger(__name__)


if __name__ == "__main__":
    srv = Server()
    srv.start()

    print("gRPC server is running. Press Ctrl+C to stop.")
    try:
        srv.block_until_termination()
        # If wait_for_termination returns normally, exit with error
        logger.error("gRPC server stopped unexpectedly")
        sys.exit(1)
    except KeyboardInterrupt:
        logger.info("Shutting down gRPC server...")
        srv.stop(0)
        sys.exit(0)
