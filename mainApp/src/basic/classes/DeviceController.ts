import type { Chat } from "./Chat";
import { Device } from "./Device";

export class DeviceController {
    public chatById: Record<string, Chat> = {};
    public devicesByChatId: Record<string, Device[]> = {};

    public currentChat: Chat | null = null;
    public currentDevice: Device | null = null;

    constructor(devices?: Device[]) {
        if (devices) {
            for (const device of devices) {
                this.addDevice(device);
            }
        }
    }


    addDevice(device: Device) {
        const chat = device.chat;
        const chatId = chat.id;

        if (!this.chatById[chatId]) {
            this.chatById[chatId] = chat;
        }

        if (!this.devicesByChatId[chatId]) {
            this.devicesByChatId[chatId] = [];
        }

        const devices = this.devicesByChatId[chatId];
        const index = devices.findIndex(d => d.equals(device));

        if (index !== -1) {
            devices[index] = device;
        } else {
            devices.push(device);
        }

        this.currentDevice = device;
        this.currentChat = chat;
    }


    updateDevice(device: Device) {
        if (!this.currentChat) return;
        if (device.chat.id !== this.currentChat.id) return;

        const devices = this.devicesByChatId[this.currentChat.id];
        if (!devices) return;

        const index = devices.findIndex(d => d.equals(device));
        if (index !== -1) {
            devices[index] = device;
            this.currentDevice = device;
        }
    }


    removeDevice(device: Device) {

        if (!this.currentChat) {
            return;
        }


        if (device.chat.id !== this.currentChat.id) {
            return;
        }

        const chatId = this.currentChat.id;
        const devices = this.devicesByChatId[chatId];
        if (!devices) {
            return;
        }

        const index = devices.findIndex(d => d.equals(device));
        if (index === -1) {
            return;
        }

        devices.splice(index, 1);

        if (devices.length === 0) {
            delete this.devicesByChatId[chatId];
            delete this.chatById[chatId];
            this.selectLastAvailableChatAndDevice();
            return;
        }

        if (this.currentDevice && this.currentDevice.equals(device)) {
            this.currentDevice = devices[devices.length - 1];
        }
    }

    removeChat(chat: Chat) {
        const chatId = chat.id;

        if (!this.chatById[chatId]) {
            return;
        }

        delete this.devicesByChatId[chatId];
        delete this.chatById[chatId];

        if (this.currentChat && this.currentChat.id === chatId) {
            this.selectLastAvailableChatAndDevice();
        }
    }


    private selectLastAvailableChatAndDevice() {
        const remainingChatIds = Object.keys(this.chatById);

        if (remainingChatIds.length > 0) {
            const lastChatId = remainingChatIds[remainingChatIds.length - 1];
            this.currentChat = this.chatById[lastChatId];

            const lastChatDevices = this.devicesByChatId[lastChatId];
            this.currentDevice = lastChatDevices[lastChatDevices.length - 1];
        } else {
            this.currentChat = null;
            this.currentDevice = null;
        }
    }


    getDevicesByChat(chat: Chat): Device[] | null {
        const devices = this.devicesByChatId[chat.id] || [];
        return devices.length > 0 ? devices : null;
    }

    getAllChats(): Chat[] | null {
        const chats = Object.values(this.chatById);
        return chats.length > 0 ? chats : null;
    }

    getAllDevices(): Device[] {
        return Object.values(this.devicesByChatId).flat();
    }



    getCurrentDevice(): Device | null {
        return this.currentDevice;
    }

    getCurrentChat(): Chat | null {
        return this.currentChat;
    }

    setCurrentChat(chat: Chat) {
        if (this.chatById[chat.id]) {
            this.currentChat = chat;

            const devices = this.devicesByChatId[chat.id];
            this.currentDevice = devices[devices.length - 1];
        } else {
            this.currentChat = null;
            this.currentDevice = null;
        }
    }

    setCurrentDevice(device: Device) {
        if (this.currentChat) {
            const devices = this.devicesByChatId[this.currentChat.id];
            const exists = devices.some(d => d.equals(device));
            if (exists) {
                this.currentDevice = device;
            }
        }
    }

    getCurrentChatDevices(): Device[] | null {
        if (!this.currentChat) {
            return null;
        }
        return this.devicesByChatId[this.currentChat.id];
    }


    hasCurrentDevice(): boolean {
        return this.currentDevice !== null;
    }

    hasCurrentChat(): boolean {
        return this.currentChat !== null;
    }

    hasAnyChats(): boolean {
        return Object.keys(this.chatById).length > 0;
    }

    hasAnyDevices(): boolean {
        return Object.values(this.devicesByChatId).some(devices => devices.length > 0);
    }


}