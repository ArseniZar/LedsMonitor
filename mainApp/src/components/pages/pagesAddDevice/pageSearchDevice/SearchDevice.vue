<template>
    <HeaderBar @goBack="handleGoBack" :visible-mode="'back'" />
    <div class=" mt-3 w-full h-full flex flex-col items-center gap-2">
        <div class=" mb-3 w-full text-center ">
            <span class="font-bold text-2xl  text-white/90 select-none"> {{ chat.title }} </span>
        </div>
        <DeviceItem v-for="item in itemsDevices" :item="item.toItemData('img/icons8-device-48.png')" />
        <button @click="handleSearchDevice" v-if="searchStatus === 'start' || searchStatus === 'done'"
            class='mt-6  h-7'>
            <img :src="getImage('img/icons8-restart-64.png')" alt="Check" class="h-7" />
        </button>
        <div v-if="searchStatus === 'loading'" class="mt-6 h-7">
            <Spin />
        </div>
        <div class="mt-7 w-full">
            <button @click="handleConfirmDevices" v-if="searchStatus === 'done'"
                class="text-center  w-full shadow-lg  mx-auto p-2 bg-black/30 rounded-4xl hover:shadow-[0_0_10px_3px_rgba(255,255,255,0.8)] hover:scale-105 hover:bg-black/95 duration-400 active:bg-black/95 transition ">
                <span class="font-bold text-white/90 select-none">Confirm</span>
            </button>
        </div>
    </div>
</template>

<script lang="ts">
import { defineComponent, type PropType } from 'vue';

import DeviceItem from './DeviceItem.vue';
import Spin from '../../../basic/Spin.vue';
import HeaderBar from '../../../basic/header/HeaderBar.vue';

import { formatScan } from '../../../../telegram/commands';
import { Device } from '../../../../basic/classes/Device';
import { Chat } from '../../../../basic/classes/Chat';
import type { BoxColor } from '../../../../basic/classes/Device';
import { getPreloadedImage } from '../../../../basic/utils/imagePreloader';


import { bot, DEVICE_SEND_SESSION_DURATION_MS, RETRY_SEND_INTERVAL_MS } from '../../../../basic/config';
import { parseMac, parseName } from '../../../../basic/utils/parse';
import { getOffset, getUpdates, sendMessage, type ResultData } from '../../../../telegram/telegramService';
import type { Update } from '@telegraf/types';





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
        offset: number | null,

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
            this.offset = await this.getOffset();
            await this.sendCommand();
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


        async getOffset(): Promise<number> {
            const result: ResultData<number> = await getOffset(bot.value!.getToken());
            if (result.status) {
                const offset = result.value as number;
                return offset;

            }
            return 0;
        },

        async sendCommand(): Promise<boolean> {
            const result: ResultData<unknown> = await sendMessage(bot.value!.getToken(), this.chat.id, formatScan());
            return result.status;
        },



        async getDevice() {
            const result: ResultData<Update> = await getUpdates(bot.value!.getToken());
            if (result.status) {

                const newUpdates: Update[] = (result.value as Update[]).filter(item => item.update_id > this.offset!).sort((a, b) => a.update_id - b.update_id);
                newUpdates.forEach((item: Update) => {
                    if (('channel_post' in item)) {
                        const channel_post: Update.Channel = item.channel_post;
                        if ('text' in channel_post && 'entities' in channel_post && Array.isArray(channel_post.entities)) {
                            const chatId: string = String(channel_post.chat.id);
                            const text: string = String(channel_post.text);
                            if (String(chatId) === String(this.chat.id)) {
                                try {
                                    const mac: string = parseMac(text);
                                    const name: string = parseName(text);
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
                                } catch (error: any) {

                                }
                            }
                        }
                    }
                    this.offset = item.update_id;
                });
            }
        },



        createDefaultboxColorsOff(): BoxColor {
            return { numberBox: 0, color: '#000000' }
        },

        createDefaultboxColorsMain(): BoxColor {
            return { numberBox: 1, color: '#e5e5e5' }
        },

        createDefaultboxColors(): BoxColor[] {
            return [
                { numberBox: 2, color: '#ffffff' },
                { numberBox: 3, color: '#ffa500' },
                { numberBox: 4, color: '#ff00ff' },
                { numberBox: 5, color: '#008000' },
                { numberBox: 6, color: '#8000ff' }];
        },

        handleGoBack() {
            this.$emit('goBack');
        },

        handleConfirmDevices() {
            this.$emit('newDevices', this.itemsDevices);
        },
        getImage(url: string): string | undefined {
            const img = getPreloadedImage(url);
            return img ? img.src : undefined;
        }

    },
}

);
</script>
