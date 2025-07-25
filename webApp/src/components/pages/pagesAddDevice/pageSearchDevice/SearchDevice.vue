<template>
    <HeaderBar @goBack="handleGoBack" :visible-mode="'back'" />
    <div class=" mt-3 w-full h-full flex flex-col items-center gap-2">
        <div class=" mb-3 w-full text-center ">
            <span class="font-bold text-2xl  text-white/90 "> {{ chat.title }} </span>
        </div>
        <DeviceItem v-for="item in itemsDevices" :item="item.toItemData('img/icons8-device-48.png')" />
        <button @click="handleSearchDevice" v-if="searchStatus === 'start' || searchStatus === 'done'"
            class='mt-6  h-7'>
            <img :src="'img/icons8-restart-64.png'" alt="Check" class="h-7" />
        </button>
        <div v-if="searchStatus === 'loading'" class="mt-6 h-7">
            <Spin />
        </div>
        <div class="mt-7 w-full">
            <button @click="handleConfirmDevices" v-if="searchStatus === 'done'"
                class="text-center  w-full shadow-lg  mx-auto p-2 bg-black/30 rounded-4xl hover:shadow-[0_0_10px_3px_rgba(255,255,255,0.8)] hover:scale-105 hover:bg-black/95 duration-400 active:bg-black/95 transition ">
                <span class="font-bold text-white/90">Confirm</span>
            </button>
        </div>
    </div>
</template>

<script lang="ts">
import { defineComponent, type PropType } from 'vue';
import DeviceItem from './DeviceItem.vue';
import Spin from '../../../basic/Spin.vue';
import HeaderBar from '../../../basic/header/HeaderBar.vue';

import type { TelegramUpdate } from '../../../../telegram/types';
import { formatScan } from '../../../../telegram/commands';
import { postCommandToTelegramChat, getTelegramUpdates } from '../../../../telegram/api';
import { HttpError, InvalidJsonError, NetworkError } from '../../../../telegram/errors';

import { Device } from '../../../../basic/classes/Device';
import { Chat } from '../../../../basic/classes/Chat';
import type { BoxColor } from '../../../../basic/classes/Device';
import emitter from '../../../../basic/eventBus';

import { CONFIG, DEVICE_SEND_SESSION_DURATION_MS, RETRY_SEND_INTERVAL_MS } from '../../../../basic/config';


class ParseError extends Error {
    command: string;
    constructor(message: string, command: string) {
        super(message);
        this.command = command;
        this.name = 'ParseError';
    }
}


export default defineComponent({
    name: 'MainAddDevice',
    components: {
        Spin,
        DeviceItem,
        HeaderBar,
    },
    emits: ['goBack', 'newDevices'],
    props: {
        chat: {
            type: Object as PropType<Chat>,
            required: true
        },
    },
    data(): {
        searchStatus: 'done' | 'loading' | 'start',
        itemsDevices: Array<Device>,
        offset: string | null,

    } {
        return {
            searchStatus: 'start',
            offset: null,
            itemsDevices: [],
        };
    },

    methods: {
        async handleSearchDevice() {
            this.searchStatus = 'loading'
            if (!this.offset) {
                this.offset = await this.initConfigOffset();
            }
            await this.SendCommand();
            const startTime = Date.now();
            while (Date.now() - startTime < DEVICE_SEND_SESSION_DURATION_MS) {
                await this.getDevice();
                await new Promise(r => setTimeout(r, RETRY_SEND_INTERVAL_MS));
            }

            if (this.itemsDevices.length > 0) {
                this.searchStatus = 'done';
            } else {
                this.searchStatus = 'start';
            }
        },

        async initConfigOffset() {
            try {
                const result: TelegramUpdate[] = await getTelegramUpdates(CONFIG.token);
                if (!result) return '0';
                return result[result.length - 1].update_id;

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
                return '0';
            }
        },


        async SendCommand() {
            try {
                await postCommandToTelegramChat(CONFIG.token, this.chat.id, formatScan());
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
        },

        async getDevice() {
            try {
                const result: TelegramUpdate[] = await getTelegramUpdates(CONFIG.token);
                if (!result.length) return;
                const newUpdates: TelegramUpdate[] = result
                    .filter(item => Number(item.update_id) > Number(this.offset))
                    .sort((a, b) => Number(a.update_id) - Number(b.update_id));

                newUpdates.forEach((item) => {

                    const chatId: string = item.channel_post.chat.id;
                    const text: string = item.channel_post.text;
                    const typeMessage: string = item.channel_post.entities[0].type

                    if (String(chatId) === String(this.chat.id) && String(typeMessage) === String('bot_command') && text) {

                        try {
                            const mac: string = this._textPars('mac', text);
                            const name: string = this._textPars('name', text);
                            const device = new Device(
                                mac,
                                this.chat,
                                name,
                                false,
                                0,
                                this.createDefaultboxColors(),
                                this.createDefaultboxColorsMain(),
                                this.createDefaultboxColorsOff(),
                            );

                            const index = this.itemsDevices.findIndex(d => d.equals(device));
                            if (index !== -1) {
                                this.itemsDevices[index] = device;
                            } else {
                                this.itemsDevices.push(device);
                            }
                        }
                        catch (error: any) {
                            emitter.emit('alert', `Parse error: ${error.message}`);
                        }
                    }
                    this.offset = String(item.update_id);
                });

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
        },

        _textPars(command: string, text: string): string {
            switch (command.toLowerCase()) {
                case 'mac': {
                    const regex = /\/mac:([0-9A-Fa-f]{2}(:[0-9A-Fa-f]{2}){5})/i;
                    const match = text.match(regex);
                    if (!match) {
                        throw new ParseError('Failed to parse MAC address from text', command);
                    }
                    return match[1].toUpperCase();
                }
                case 'name': {
                    const regex = /\/name:([^\s]+)/i;
                    const match = text.match(regex);
                    if (!match) {
                        throw new ParseError('Failed to parse NAME from text', command);
                    }
                    return match[1].trim();
                }
                default:
                    throw new ParseError(`Unsupported command: ${command}`, command);
            }
        },





        createDefaultboxColorsOff(): BoxColor {
            return { numberBox: Number(0), color: '#000000' }
        },

        createDefaultboxColorsMain(): BoxColor {
            return { numberBox: Number(1), color: '#e5e5e5' }
        },

        createDefaultboxColors(): BoxColor[] {
            return [
                { numberBox: Number(2), color: '#ffffff' },
                { numberBox: Number(3), color: '#ffa500' },
                { numberBox: Number(4), color: '#ff00ff' },
                { numberBox: Number(5), color: '#008000' },
                { numberBox: Number(6), color: '#8000ff' }];
        },

        handleGoBack() {
            this.$emit('goBack');
        },

        handleConfirmDevices() {
            this.$emit('newDevices', this.itemsDevices);
        }

    },
}

);
</script>
