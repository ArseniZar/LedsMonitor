import type { Network } from "../classes/Network";
import { fetchGetNetworks, fetchSendEnd, fetchSendNetwork } from "./api";
import { HttpError, InvalidJsonError, NetworkError } from "./errors";
import type { ResultData } from "./types";



export async function getNetworks(): Promise<ResultData<Network[]>> {
  try {
    const networksResponse: Network[] = await fetchGetNetworks();
    return { status: true, value: networksResponse };
  } catch (error: any) {
    return handleApiError(error);
  }
}


export async function sendNetwork(network: Network): Promise<ResultData<Network>> {
  try {
    const networkResponse = await fetchSendNetwork(network);
    return { status: true, value: networkResponse};
  } catch (error: any) {
    return handleApiError(error);
  }
}

export async function sendEnd(): Promise<ResultData<unknown>> {
  try {
    await fetchSendEnd();
    return { status: true};
  } catch (error: any) {
    return handleApiError(error);
  }
}

function handleApiError<T>(errorOrMessage: any): ResultData<T> {
  let alertMessage = "Unknown error occurred";

  if (typeof errorOrMessage === "string") {
    alertMessage = errorOrMessage;
  } else if (errorOrMessage instanceof NetworkError) {
    alertMessage =
      "Network error: Please check your internet connection and try again.";
  } else if (errorOrMessage instanceof HttpError) {
    switch (errorOrMessage.status) {
      case 400:
        alertMessage =
          "Bad Request: Chat does not exist or the application is not a member.";
        break;
      case 401:
        alertMessage = "Unauthorized: Invalid bot token.";
        break;
      case 403:
        alertMessage =
          "Forbidden: Bot has no permission to perform this action.";
        break;
      case 429:
        alertMessage =
          "Too Many Requests: You are sending requests too quickly. Please slow down.";
        break;
      default:
        if (errorOrMessage.status >= 500 && errorOrMessage.status < 600) {
          alertMessage = `Server error (${errorOrMessage.status}): Telegram service temporarily unavailable. Try again later.`;
        } else {
          alertMessage = `HTTP error ${errorOrMessage.status}: ${errorOrMessage.message}`;
        }
    }
  } else if (errorOrMessage instanceof InvalidJsonError) {
    alertMessage = `Invalid JSON response received: ${errorOrMessage.message}`;
  } else if (errorOrMessage instanceof Error) {
    alertMessage = `Unknown error: ${errorOrMessage.message}`;
  } else {
    alertMessage = `Unknown error: ${String(errorOrMessage)}`;
  }
  return { status: false, alertMessage };
}
