<template>
    <FormInput :alert="alertInfo" :text="'Enter your chat ID from WebApp'" :validData="validateChatId"
        :spinStatus="spinStatus" :headerStatus="headerStatus" @submit="handleChatIdSubmit" @goBack="handleGoBack" />
</template>


<script lang="ts">
import { defineComponent } from 'vue';
import FormInput from '../../../basic/FormInput.vue';
import { getChatMemberInfo, getChatInfo } from '../../../../telegram/api';
import { CONFIG } from '../../../../config';
import type { Chat, ChatInfo } from '../../../../telegram/types';
import { HttpError, InvalidJsonError, NetworkError } from '../../../../telegram/errors';


export default defineComponent({
    name: 'ChatId',
    components: {
        FormInput,
    },
    emits: ['goBack', 'newChat'],
    props: {
        headerStatus: {
            type: Boolean,
            required: false,
            default: true
        },
    },

    data(): {
        spinStatus: boolean,
        alertInfo: string,
    } {
        return {
            spinStatus: false,
            alertInfo: 'Enter a valid chat ID',
        };
    },

    methods: {
        async validateChatId(chatId: string): Promise<boolean> {
            try {
                await getChatMemberInfo(CONFIG.token, chatId, CONFIG.botId);
                return true;

            } catch (error: any) {
                if (error instanceof NetworkError) {
                    console.error(error.message);

                } else if (error instanceof HttpError) {
                    if (error.status === 400) {
                        this.alertInfo = "Chat does not exist or the application is not a member"
                    } else {
                        console.error(`HTTP ошибка ${error.status}:`, error.message);
                    }
                } else if (error instanceof InvalidJsonError) {
                    console.error('Некорректный JSON:', error.message);

                } else {
                    console.error('Неизвестная ошибка:', error);
                }
                return false;
            }
        },

        async getChatTittile(chatId: string){
            try {
                const result: ChatInfo = await getChatInfo(CONFIG.token, chatId);
                return result.title

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
               return "" 
            }
        },

        async handleChatIdSubmit(chatId: string) {
            const title = await this.getChatTittile(chatId);
            const chat: Chat  = { id: chatId, title: title};
            this.$emit('newChat', chat);
        },


        handleGoBack() {
            this.$emit('goBack');
        }
    },
}

);
</script>