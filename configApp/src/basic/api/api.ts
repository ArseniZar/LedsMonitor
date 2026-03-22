
import {HttpError,InvalidJsonError,NetworkError,ApiReturnedError,} from "./errors";
import type { ApiError, ApiSuccess } from "./types";
import type { ConnectNetworkRequest, ConnectNetworkResponce, ScanNetworksResponce } from "./apiModels";

export async function fetchScanNetworks(): Promise<ScanNetworksResponce> {
  const response: Response = await fetch(`${__API_URL__}/scan`).catch(() => {
    throw new NetworkError("Network error occurred while fetching updates");
  });

  if (!response.ok) {
    const body: ApiError = await response.json().catch(() => {
      throw new InvalidJsonError();
    });
    throw new HttpError(response.status, body.message);
  }

  const body: ApiSuccess<ScanNetworksResponce> | ApiError = await response.json().catch(() => {
    throw new InvalidJsonError();
  });

  if (!body.ok) {
    const apiError: ApiError = body as ApiError;
    throw new ApiReturnedError(apiError.error_code, apiError.message);
  }

  return body.result;
}

export async function fetchConnectNetwork(request: ConnectNetworkRequest): Promise<ConnectNetworkResponce> {
  const response: Response = await fetch(`${__API_URL__}/network`, {
    method: "POST",
    headers: { "Content-Type": "application/json" },
    body: JSON.stringify({
      data: request.network,
    }),
  }).catch(() => {
    throw new NetworkError("Network error occurred while fetching updates");
  });
  if (!response.ok) {
    const body: ApiError = await response.json().catch(() => {
      throw new InvalidJsonError();
    });
    throw new HttpError(response.status, body.message);
  }

  const body: ApiSuccess<ConnectNetworkResponce> | ApiError = await response
    .json()
    .catch(() => {
      throw new InvalidJsonError();
    });

  if (!body.ok) {
    const apiError: ApiError = body as ApiError;
    throw new ApiReturnedError(apiError.error_code, apiError.message);
  }

  return body.result;
}

export async function fetchEnd(): Promise<unknown> {
  const response: Response = await fetch(`${__API_URL__}/end`).catch(() => {
    throw new NetworkError("Network error occurred while fetching updates");
  });
  if (!response.ok) {
    const body: ApiError = await response.json().catch(() => {
      throw new InvalidJsonError();
    });
    throw new HttpError(response.status, body.message);
  }

  const body: ApiSuccess<unknown> | ApiError = await response
    .json()
    .catch(() => {
      throw new InvalidJsonError();
    });

  if (!body.ok) {
    const apiError: ApiError = body as ApiError;
    throw new ApiReturnedError(apiError.error_code, apiError.message);
  }
  return body.result;
}
