import type { Network } from "../classes/Network";
import {
  HttpError,
  InvalidJsonError,
  NetworkError,
  ApiReturnedError,
} from "./errors";
import type { ApiError, ApiSuccess } from "./types";
const url = "";

export async function fetchGetNetworks(): Promise<Network[]> {
  const response: Response = await fetch(`${url}/scan`).catch(() => {
    throw new NetworkError("Network error occurred while fetching updates");
  });

  if (!response.ok) {
    const body: ApiError = await response.json().catch(() => {
      throw new InvalidJsonError();
    });
    throw new HttpError(response.status, body.description);
  }

  const body: ApiSuccess<Network[]> | ApiError = await response
    .json()
    .catch(() => {
      throw new InvalidJsonError();
    });

  if (!body.ok) {
    const apiError: ApiError = body as ApiError;
    throw new ApiReturnedError(apiError.error_code, apiError.description);
  }

  return body.result;
}

export async function fetchSendNetwork(network: Network): Promise<Network> {
  const response: Response = await fetch(`${url}/network`, {
    method: "POST",
    headers: { "Content-Type": "application/json" },
    body: JSON.stringify(network),
  }).catch(() => {
    throw new NetworkError("Network error occurred while fetching updates");
  });
  if (!response.ok) {
    const body: ApiError = await response.json().catch(() => {
      throw new InvalidJsonError();
    });
    throw new HttpError(response.status, body.description);
  }

  const body: ApiSuccess<Network> | ApiError = await response
    .json()
    .catch(() => {
      throw new InvalidJsonError();
    });

  if (!body.ok) {
    const apiError: ApiError = body as ApiError;
    throw new ApiReturnedError(apiError.error_code, apiError.description);
  }

  return body.result;
}

export async function fetchSendEnd(): Promise<unknown> {
  const response: Response = await fetch(`${url}/end`).catch(() => {
    throw new NetworkError("Network error occurred while fetching updates");
  });
  if (!response.ok) {
    const body: ApiError = await response.json().catch(() => {
      throw new InvalidJsonError();
    });
    throw new HttpError(response.status, body.description);
  }

  const body: ApiSuccess<unknown> | ApiError = await response
    .json()
    .catch(() => {
      throw new InvalidJsonError();
    });

  if (!body.ok) {
    const apiError: ApiError = body as ApiError;
    throw new ApiReturnedError(apiError.error_code, apiError.description);
  }
  return body.result;
}