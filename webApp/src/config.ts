export const CONFIG = {
  token: '',
  botId: '',
}


export type ConfigType = {
  token: string;
  botId: string;
};

export const DEVICE_SEND_SESSION_DURATION_MS = 15000;
export const RETRY_SEND_INTERVAL_MS = 1200;
export const DEVICE_UPDATE_DEBOUNCE_DELAY_MS = 50;
export const MIN_DEVICE_SEND_INTERVAL_MS = 1000;