<template>
    <div v-if="currentAlert" class="fixed z-10 pointer-events-none inset-0 mx-auto w-10/12 max-w-md p-4 pt-6">
        <button @click="closeAlert" @mousedown="handleLongPressItemStart" @mouseup="handleLongPressItemEnd"
            @mouseleave="handleLongPressItemEnd" @touchstart="handleLongPressItemStart"
            @touchend="handleLongPressItemEnd" @touchcancel="handleLongPressItemEnd"
            class="flex w-full gap-3 flex-row justify-between p-2 px-4 items-center rounded-2xl pointer-events-auto bg-black/80 active:scale-105 transition-transform duration-400">
            <span class="select-none flex-1 text-center text-sm font-mono text-white">
                {{ currentAlert }}
            </span>
        </button>
    </div>
</template>

<script lang="ts">
import { defineComponent } from 'vue';
import emitter from '../../basic/eventBus';
import { LongPressHandler } from '../../basic/classes/LongPressHadler';

export default defineComponent({
    name: 'Alert',
    data(): {
        queue: string[];
        currentAlert: string | null;
        timerId: number | null;
        longPressHandler: LongPressHandler | null;
    } {
        return {
            longPressHandler: null,
            queue: [],
            currentAlert: null,
            timerId: null,
        };
    },
    created() {
        emitter.on('alert', this.handleNewAlert);
    },
    mounted() {
        this.longPressHandler = new LongPressHandler(400, () => {
            this.hadleCopyAlert();
        });
    },
    beforeUnmount() {
        emitter.off('alert', this.handleNewAlert);
        this.clearTimer();
    },
    methods: {
        handleNewAlert(newAlert: string) {
            if (newAlert && !this.queue.includes(newAlert)) {
                this.queue.push(newAlert);
                this.showNextAlert();
            }
        },
        showNextAlert() {
            if (!this.currentAlert && this.queue.length > 0) {
                this.currentAlert = this.queue[0];
                this.startTimer();
            }
        },
        startTimer() {
            this.clearTimer();
            this.timerId = window.setTimeout(() => {
                this.removeCurrentAlert();
            }, 4000);
        },
        clearTimer() {
            if (this.timerId !== null) {
                clearTimeout(this.timerId);
                this.timerId = null;
            }
        },


        removeCurrentAlert() {
            if (this.currentAlert) {
                this.queue.shift();
                this.currentAlert = null;
                this.clearTimer();

                if (this.queue.length > 0) {
                    setTimeout(() => {
                        this.showNextAlert();
                    }, 100);
                }
            }
        },


        handleLongPressItemStart() {
            this.longPressHandler?.handleLongPressStart();
        },

        handleLongPressItemEnd() {
            this.longPressHandler?.handleLongPressEnd();
        },


        async hadleCopyAlert() {
            if (this.currentAlert)
                await navigator.clipboard.writeText(this.currentAlert);
        },

        closeAlert() {
            this.removeCurrentAlert();
        },
    },
});
</script>
