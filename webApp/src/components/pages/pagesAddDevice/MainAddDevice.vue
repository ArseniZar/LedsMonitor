<template>
    <ChatId v-if="stepStack[stepStack.length - 1] === StepAddDevice.ChatId" @goBack="handleGoBack" :headerStatus="true"
        @newChat="handleChatIdSubmit" />
    <SearchDevice v-if="stepStack[stepStack.length - 1] === StepAddDevice.SearchDevices" @goBack="handleGoBack"
        :chat="newChat" @newDevices="handleNewDevices" @offset="handleOffset" />


</template>


<script lang="ts">
import { defineComponent } from 'vue';
import { StepAddDevice, type Device } from '../../../interface';
import SearchDevice from './pageSearchDevice/SearchDevice.vue';
import ChatId from './pageChatId/ChatId.vue';
import type { Chat } from '../../../telegram/types';


export default defineComponent({
    name: 'MainAddDevice',
    components: {
        SearchDevice,
        ChatId
    },
    emits: ['goBack', 'chatId', 'newOffset', 'newDevices'],
    data(): {
        stepStack: StepAddDevice[],
        StepAddDevice: typeof StepAddDevice,
        newChat: Chat,
        alertInfo: string,

    } {
        return {
            newChat: {} as Chat,
            stepStack: [StepAddDevice.ChatId],
            StepAddDevice: StepAddDevice,
            alertInfo: 'Enter a valid chat ID',
        };
    },

    methods: {

        handleChatIdSubmit(newChat: Chat) {
            this.newChat = newChat;
            this.stepStack.push(StepAddDevice.SearchDevices);
        },

        handleOffset(offset: string) {
            this.$emit('newOffset', offset)
        },

        handleNewDevices(devices: Device[]) {
            this.$emit('newDevices', devices)
        },
        
        handleGoBack() {
            this.stepStack.pop();
            if (this.stepStack.length === 0) {
                this.$emit('goBack');
            }
        }
    },
}

);
</script>
