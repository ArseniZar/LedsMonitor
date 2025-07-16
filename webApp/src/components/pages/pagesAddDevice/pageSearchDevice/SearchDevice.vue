<template>
    <HeaderBar :goBack="handleGoBack" :visible-mode="'back'" />
    <div class=" mt-3 w-full h-full flex flex-col items-center gap-2">
        <div class=" mb-3 w-full text-center ">
            <span class="font-bold text-2xl  text-white/90 "> {{ chat.title }} </span>
        </div>
        <DeviceItem v-for="itemDevice in itemsDevices" :item="{ id: itemDevice.id, name: itemDevice.name }" />
        <button @click="handleSearchDevice" v-if="searchStatus === 'start' || searchStatus === 'done'"
            class='mt-6  h-7'>
            <img src="../../../../assets/img/icons8-restart-64.png" alt="Check" class="h-7" />
        </button>
        <div v-if="searchStatus === 'loading'" class="mt-6 h-7">
            <Spin />
        </div>
        <div class="mt-7 w-full">
            <button @click="handleConfirmDevice" v-if="searchStatus === 'done'"
                class="text-center  w-full shadow-lg  mx-auto p-2 bg-black/30 rounded-4xl hover:shadow-[0_0_10px_3px_rgba(255,255,255,0.8)] hover:scale-105 hover:bg-black/95 duration-400 active:bg-black/95 transition ">
                <span class="font-bold text-white/90">Confirm</span>

            </button>
        </div>



    </div>
</template>

<script lang="ts">
import { defineComponent, type PropType } from 'vue';
import type { BoxColor } from '../../../../interface';
import { Device } from '../../../../interface';
import DeviceItem from './DeviceItem.vue';
import Spin from '../../../basic/Spin.vue';
import HeaderBar from '../../../basic/header/HeaderBar.vue';
import { CONFIG, DURATION_MS, INTERVAL_MS } from '../../../../config';
import { TELEGRAM_COMMANDS } from '../../../../telegram/commands';
import { postCommandToTelegramChat, getTelegramUpdates } from '../../../../telegram/api';
import type { Chat, TelegramUpdate } from '../../../../telegram/types';
import { HttpError, InvalidJsonError, NetworkError } from '../../../../telegram/errors'


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
    emits: ['goBack', 'newDevices', 'offset'],
    props: {
        chat: {
            type: Object as PropType<Chat>,
            required: true
        },
    },
    data(): {
        searchStatus: 'done' | 'loading' | 'start',
        itemsDevices: Array<Device>,
        offset: string,

    } {
        return {
            searchStatus: 'start',
            offset: "",
            itemsDevices: [],
        };
    },

    mounted() {
        this.offset = CONFIG.offset
    },

    methods: {
        async handleSearchDevice() {
            this.searchStatus = 'loading'

            await this.SendCommand();
            const startTime = Date.now();
            while (Date.now() - startTime < DURATION_MS) {
                await this.getDevice();
                await new Promise(r => setTimeout(r, INTERVAL_MS));
            }

            if (this.itemsDevices.length) {
                this.searchStatus = 'done';
            } else {
                this.searchStatus = 'start';
            }
            this.$emit("offset", this.offset)
        },

        async SendCommand() {
            try {
                await postCommandToTelegramChat(CONFIG.token, this.chat.id, TELEGRAM_COMMANDS.scan);
            } catch (error: any) {
                if (error instanceof NetworkError) {
                    console.error(error.message);

                } else if (error instanceof HttpError) {
                    console.error(`HTTP ошибка ${error.status}:`, error.message);

                } else if (error instanceof InvalidJsonError) {
                    console.error('Некорректный JSON:', error.message);

                } else {
                    console.error('Неизвестная ошибка:', error);
                }
            }
        },

        async getDevice() {
            try {
                const result: TelegramUpdate[] = await getTelegramUpdates(CONFIG.token);


                if (!result.length) return;

                const newUpdates: TelegramUpdate[] = [];
                for (let i: number = result.length - 1; i >= 0; i--) {
                    const item: TelegramUpdate = result[i];

                    if (Number(this.offset) - Number(item.update_id) >= 0) break;
                    newUpdates.unshift(item);
                }
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
                                this._createDefaultboxColors(),
                                this._createDefaultboxColorsMain(),
                                this._createDefaultboxColorsOff(),
                            );

                            this.itemsDevices.push(device);
                            console.log("ok")
                        }
                        catch (error: any) {
                            console.error(error.message);
                        }
                    }
                    this.offset = String(item.update_id);
                });

            } catch (error: any) {
                if (error instanceof NetworkError) {
                    console.error(error.message);

                } else if (error instanceof HttpError) {
                    console.error(`HTTP ошибка ${error.status}:`, error.message);

                } else if (error instanceof InvalidJsonError) {
                    console.error('Некорректный JSON:', error.message);

                } else {
                    console.error('Неизвестная ошибка:', error);
                }
            }
        },

        _textPars(command: string, text: string): string {
            switch (command) {
                case 'mac': {
                    const match = text.match(/([0-9A-Fa-f]{2}:){5}[0-9A-Fa-f]{2}/);
                    if (!match) {
                        throw new ParseError('Failed to parse MAC address from text', command);
                    }
                    return match[0];
                }
                case 'name': {
                    const match = text.match(/NAME:\s*([^\s]+)/i);
                    if (!match) {
                        throw new Error('Failed to parse NAME from text');
                    }
                    return match[1].trim();
                }

                default:
                    throw new ParseError(`Unsupported command: ${command}`, command);
            }
        },





        _createDefaultboxColorsOff(): BoxColor {
            return { numberBox: Number(0), color: '#e5e5e5' }
        },

        _createDefaultboxColorsMain(): BoxColor {
            return { numberBox: Number(1), color: '#e5e5e5' }
        },

        _createDefaultboxColors(): BoxColor[] {
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

        handleConfirmDevice() {
            this.$emit('newDevices', this.itemsDevices);
        }

    },
}

);
</script>
