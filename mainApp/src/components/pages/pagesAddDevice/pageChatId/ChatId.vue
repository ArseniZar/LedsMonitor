<template>
    <FormInput :text="'Enter your chat ID with devices'" :validData="validateChatId" @submit="handleChatSubmit"
        @goBack="handleGoBack" />
</template>


<script lang="ts">
import { defineComponent } from 'vue';
import type { Chat as TelegramChat } from '@telegraf/types';

import { bot } from '../../../../basic/config';
import { Chat } from '../../../../basic/classes/Chat';
import FormInput from '../../../basic/FormInput.vue';

import { getChat, validateChatId } from '../../../../telegram/telegramService';
import type { ResultData } from '../../../../telegram/telegramService';


export default defineComponent({
    name: 'ChatId',
    components: {
        FormInput,
    },
    emits: ['goBack', 'newChat'],
    data(): {
    } {
        return {
        };
    },

    methods: {
        async validateChatId(chatId: string): Promise<ResultData<unknown>> {
            return await validateChatId(bot.value!.getToken(), chatId, bot.value!.getBotId());
        },


        async handleChatSubmit(chatId: string) {
            const result: ResultData<TelegramChat> = await getChat(bot.value!.getToken(), chatId);
            if (result.status) {
                const chatInfo = result.value as TelegramChat;
                if (chatInfo.type === 'channel') {
                    const chat = new Chat(String(chatInfo.id), chatInfo.title, chatInfo.type);
                    this.$emit('newChat', chat);
                    return;
                }
            }
        },

        handleGoBack() {
            this.$emit('goBack');
        }
    },
}

);
</script>