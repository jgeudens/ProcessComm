"""EchoService implementation."""

import logging

from logger.v1 import logger_pb2, logger_pb2_grpc

logger = logging.getLogger(__name__)

# pylint: disable=no-member


class LoggerPluginService(logger_pb2_grpc.LoggerPluginServicer):
    """Implementation of a Logging Plugin gRPC service."""

    def __init__(self):
        """Initializes the LoggerPluginService."""
        self._logging_active = False
        self._data_points = ["temperature", "pressure", "voltage"]
        self._log_storage = {
            "temperature": 25,
            "pressure": 101,
            "voltage": 12,
        }

    # --- Metadata ---
    def GetPluginInfo(self, request, context):
        """Returns plugin metadata information."""
        return logger_pb2.GetPluginInfoResponse(
            version="1.0.0",
            type="logging",
            author="Your Name",
            description="Example logging plugin implementation",
        )

    # --- Logging Operations ---
    def GetAvailableDataPoints(self, request, context):
        """Returns the list of available data points for logging."""
        return logger_pb2.GetAvailableDataPointsResponse(dataPoint=self._data_points)

    def StartLogging(self, request, context):
        """Starts the logging process."""
        self._logging_active = True
        return logger_pb2.StartLoggingResponse()

    def StopLogging(self, request, context):
        """Stops the logging process."""
        self._logging_active = False
        return logger_pb2.StopLoggingResponse()

    def ReadLog(self, request, context):
        """Reads logged data for specified data points."""
        results = []

        for datapoint in request.DataPoint:
            if not self._logging_active:
                results.append(
                    logger_pb2.LogResult(
                        endpoint_name=datapoint,
                        status=logger_pb2.LOG_READ_STATUS_GENERIC_ERROR,
                        error_message="Logging not active",
                    )
                )
                continue

            if datapoint not in self._log_storage:
                results.append(
                    logger_pb2.LogResult(
                        endpoint_name=datapoint,
                        status=logger_pb2.LOG_READ_STATUS_NOT_FOUND,
                        error_message="Data point not found",
                    )
                )
                continue

            results.append(
                logger_pb2.LogResult(
                    endpoint_name=datapoint,
                    value=self._log_storage[datapoint],
                    status=logger_pb2.LOG_READ_STATUS_SUCCESS,
                )
            )

        return logger_pb2.ReadLogResponse(results=results)


# pylint: enable=no-member
