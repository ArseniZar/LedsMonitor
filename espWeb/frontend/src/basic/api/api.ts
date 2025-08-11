import type { Network } from "../classes/Network";
import {
  HttpError,
  InvalidJsonError,
  NetworkError,
  ApiReturnedError,
} from "./errors";
import type { ApiError, ApiSuccess } from "./types";

export async function fetchGetNetworks(): Promise<Network[]> {
  const response: Response = await fetch(`${__API_URL__}/scan`).catch(() => {
    throw new NetworkError("Network error occurred while fetching updates");
  });

  if (!response.ok) {
    const body: ApiError = await response.json().catch(() => {
      throw new InvalidJsonError();
    });
    throw new HttpError(response.status, body.message);
  }

  const body: ApiSuccess<Network[]> | ApiError = await response
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

export async function fetchSendNetwork(network: Network): Promise<Network> {
  const response: Response = await fetch(`${__API_URL__}/network`, {
    method: "POST",
    headers: { "Content-Type": "application/json" },
    body: JSON.stringify({
      data: network,
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

  const body: ApiSuccess<Network> | ApiError = await response
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

export async function fetchSendEnd(): Promise<unknown> {
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
