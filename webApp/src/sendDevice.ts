import { bot, DEVICE_UPDATE_DEBOUNCE_DELAY_MS, MIN_DEVICE_SEND_INTERVAL_MS } from "./basic/config";
import { Device } from "./basic/classes/Device";
import { postCommandToTelegramChat } from "./telegram/api";
import { formatDeviceInfo } from "./telegram/commands";
import { NetworkError, HttpError, InvalidJsonError } from "./telegram/errors";
import emitter from "./basic/eventBus";

let pendingDevice: Device | null = null;
let debounceTimer: ReturnType<typeof setTimeout> | null = null;
let sendDelayTimer: ReturnType<typeof setTimeout> | null = null;
let lastSentTime = 0;
let isSending = false;



export function scheduleSendDevice(newDevice: Device) {
    pendingDevice = newDevice;

    if (debounceTimer) {
        clearTimeout(debounceTimer);
    }

    debounceTimer = setTimeout(() => {
        trySendPendingDevice();
    }, DEVICE_UPDATE_DEBOUNCE_DELAY_MS);
}

async function trySendPendingDevice() {
    if (!pendingDevice) return;

    if (isSending) {
        if (sendDelayTimer) clearTimeout(sendDelayTimer);
        sendDelayTimer = setTimeout(() => {
            trySendPendingDevice();
        }, MIN_DEVICE_SEND_INTERVAL_MS);
        return;
    }

    const now = Date.now();
    const timeSinceLastSend = now - lastSentTime;

    if (timeSinceLastSend >= MIN_DEVICE_SEND_INTERVAL_MS) {
        await sendPendingDevice();
    } else {
        const remaining = MIN_DEVICE_SEND_INTERVAL_MS - timeSinceLastSend;
        if (sendDelayTimer) clearTimeout(sendDelayTimer);
        sendDelayTimer = setTimeout(() => {
            trySendPendingDevice();
        }, remaining);
    }

    debounceTimer = null;
}

async function sendPendingDevice() {
    if (!pendingDevice) return;

    isSending = true;
    try {
        const text = formatDeviceInfo(pendingDevice);
        await postCommandToTelegramChat(bot.value!.token, pendingDevice.chat.id, text);
        console.log("Device sent successfully");
    } catch (error: any) {
        if (error instanceof NetworkError) {
            emitter.emit('alert', `Network error: Please check your internet connection and try again.`);
        } else if (error instanceof HttpError) {
            emitter.emit('alert', `HTTP error ${error.status}: ${error.message}`);
        } else if (error instanceof InvalidJsonError) {
            emitter.emit('alert', `Invalid JSON: ${error.message}`);
        } else {
            emitter.emit('alert', `Unknown error: ${String(error)}`);
        }

    }
    finally {
        isSending = false;
        lastSentTime = Date.now();
        pendingDevice = null;
    }
}


