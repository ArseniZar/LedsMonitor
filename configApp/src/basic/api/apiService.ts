import type { Network } from "../classes/Network";
import { fetchScanNetworks, fetchEnd, fetchConnectNetwork } from "./api";
import { HttpError, InvalidJsonError, NetworkError } from "./errors";
import type { ResultData } from "./types";
import type {ScanNetworksResponce,ConnectNetworkResponce} from "./apiModels";

export async function ScanNetworks(): Promise<ResultData<Network[]>> {
  try {
    const networksResponse: ScanNetworksResponce = await fetchScanNetworks();

    return { status: true, value: networksResponse.networks };
  } catch (error: any) {
    return handleApiError(error);
  }
}

export async function ConnectNetwork(network: Network): Promise<ResultData<boolean>> {
  try {
    const networkResponse: ConnectNetworkResponce = await fetchConnectNetwork({ network });
    return { status: true, value: networkResponse.status.status };
  } catch (error: any) {
    return handleApiError(error);
  }
}

export async function End(): Promise<ResultData<unknown>> {
  try {
    await fetchEnd();
    return { status: true };
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
