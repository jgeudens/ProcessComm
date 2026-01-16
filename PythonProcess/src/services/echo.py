"""EchoService implementation."""

import logging

import echo_pb2
import echo_pb2_grpc

logger = logging.getLogger(__name__)


class EchoService(
    echo_pb2_grpc.EchoServiceServicer,
):
    """EchoService implementation."""

    def Echo(self, request, context):
        """Handles the Echo RPC call."""
        self.log_request("Echo", request)

        return echo_pb2.EchoReply(message=request.message)  # pylint: disable=no-member

    def log_request(self, rpc_name: str, request) -> None:
        """Logs the received request.

        :param self: Description
        :param rpc_name: Description
        :type rpc_name: str
        :param request: Description
        """
        logger.info("%s received: %s", rpc_name, request)
