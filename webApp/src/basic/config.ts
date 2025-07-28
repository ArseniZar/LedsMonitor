import { ref, type Ref } from 'vue';
import type { Bot } from './classes/Bot';


export const bot: Ref<Bot | null> = ref(null);
export const DEVICE_SEND_SESSION_DURATION_MS = 5000;
export const RETRY_SEND_INTERVAL_MS = 1200;
export const DEVICE_UPDATE_DEBOUNCE_DELAY_MS = 50;
export const MIN_DEVICE_SEND_INTERVAL_MS = 1000;