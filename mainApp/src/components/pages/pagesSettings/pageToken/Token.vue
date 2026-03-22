<template>
    <HeaderBar :visible-mode="'back'" @goBack="goBack" />
    <div class="mt-10 flex w-full flex-col items-center gap-2">
        <div @click="hadleCopyText" @mousedown="handleLongPressItemStart" @mouseup="handleLongPressItemEnd"
            @mouseleave="handleLongPressItemEnd" @touchstart="handleLongPressItemStart"
            @touchend="handleLongPressItemEnd" @touchcancel="handleLongPressItemEnd" class=" flex w-full flex-row items-center justify-between p-2 px-4 rounded-3xl bg-black/60
            hover:shadow-[0_0_10px_3px_rgba(255,255,255,0.8)] hover:bg-black/95 active:bg-black/95 active:scale-110 hover:scale-105
            transition">
            <span
                class="font-mono text-white text-sm cursor-default select-none max-w-[calc(100%-3rem)] overflow-hidden whitespace-nowrap truncate block">
                {{ bot!.token }}
            </span>
            <button @click="hadleCopyText">
                <img :src="getImage('img/icons8-copy-24.png')" alt="Check" class="h-5 w-5" />
            </button>
        </div>
        <span class="text-sm w-full bg-black/20 rounded-3xl p-3 px-4 text-white/80 select-none">{{ message }} </span>
    </div>
    <ConfirmDelete v-if="tokenToDelete" :title="'Delete token'" @hasPermission="hasPermission" />
</template>


<script lang="ts">
import { defineComponent } from 'vue';
import { bot } from '../../../../basic/config';
import HeaderBar from '../../../basic/header/HeaderBar.vue';
import ConfirmDelete from '../../../basic/ConfirmDelete.vue';
import { LongPressHandler } from '../../../../basic/classes/LongPressHadler';
import { getPreloadedImage } from '../../../../basic/utils/imagePreloader';


export default defineComponent({
    name: 'Token',
    components: {
        HeaderBar,
        ConfirmDelete,
    },
    emits: ['goBack', 'deleteToken'],
    data(): {
        longPressHandler: LongPressHandler | null;
        tokenToDelete: boolean;
        message: string;
        bot: typeof bot;
    } {
        return {
            longPressHandler: null,
            tokenToDelete: false,
            bot: bot,
            message: `This is the bot token that will be used to handle 
                        sending messages in the chat with devices.`
        };
    },
    mounted() {
        this.longPressHandler = new LongPressHandler(400, () => {
            this.tokenToDelete = true;
        });
    },

    methods: {
        async hadleCopyText() {
            await navigator.clipboard.writeText(bot.value!.token);
        },

        handleLongPressItemStart() {
            this.longPressHandler?.handleLongPressStart();
        },

        handleLongPressItemEnd() {
            this.longPressHandler?.handleLongPressEnd();
        },

        hasPermission(isAllowed: boolean) {
            if (isAllowed && this.tokenToDelete) {
                this.$emit('deleteToken');
            }
            this.tokenToDelete = false;
        },


        goBack() {
            this.$emit('goBack');
        },

        getImage(url: string): string | undefined {
            const img = getPreloadedImage(url);
            return img ? img.src : undefined;
        },
    },
});

</script>