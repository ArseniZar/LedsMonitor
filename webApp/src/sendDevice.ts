import { CONFIG, DEVICE_UPDATE_DEBOUNCE_DELAY_MS, MIN_DEVICE_SEND_INTERVAL_MS } from "./config";
import type { Device } from "./interface";
import { postCommandToTelegramChat } from "./telegram/api";
import { NetworkError, HttpError, InvalidJsonError } from "./telegram/errors";

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
        const text = formatDeviceMessage(pendingDevice);
        await postCommandToTelegramChat(CONFIG.token, pendingDevice.chat.id, text);
        console.log("Device sent successfully");
    } catch (error: any) {
        if (error instanceof NetworkError) {
            console.error(error.message);
        } else if (error instanceof HttpError) {
            console.error(`HTTP ошибка ${error.status}:`, error.message);
        } else if (error instanceof InvalidJsonError) {
            console.error("Некорректный JSON:", error.message);
        } else {
            console.error("Неизвестная ошибка:", error);
        }
    } finally {
        isSending = false;
        lastSentTime = Date.now();
        pendingDevice = null;
    }
}

function formatDeviceMessage(device: Device): string {
    const { currentNumberBox, boxColorMain, boxColorOff, boxColorsList } = device;

    const getColor = (): string => {
        if (currentNumberBox === boxColorOff.numberBox) {
            return boxColorOff.color;
        }
        if (currentNumberBox === boxColorMain.numberBox) {
            return boxColorMain.color;
        }
        const found = boxColorsList.find(
            (bc) => bc.numberBox === currentNumberBox && bc.numberBox !== boxColorOff.numberBox
        );
        return found ? found.color : "";
    };

    const color = getColor();

    return `/ID: ${device.id}\n/CHAT: ${device.chat.id}\n/COLOR: ${color}\n/STATUS: ${device.status ? "ON" : "OFF"}`;
}
