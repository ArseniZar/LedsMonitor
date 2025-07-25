<template>
    <FormInput :alert="alertInfo" :text="'Enter your chat ID from WebApp'" :validData="validateChatId"
        :spinStatus="spinStatus" :headerStatus="headerStatus" @submit="handleChatIdSubmit" @goBack="handleGoBack" />
</template>


<script lang="ts">
import { defineComponent } from 'vue';
import FormInput from '../../../basic/FormInput.vue';
import { getChatMemberInfo, getChatInfo } from '../../../../telegram/api';
import { CONFIG } from '../../../../basic/config';
import type { ChatInfo } from '../../../../telegram/types';
import { Chat } from '../../../../basic/classes/Chat'
import { HttpError, InvalidJsonError, NetworkError } from '../../../../telegram/errors';
import emitter from '../../../../basic/eventBus';


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
            alertInfo: '',
        };
    },

    methods: {
        async validateChatId(chatId: string): Promise<boolean> {
            try {
                await getChatMemberInfo(CONFIG.token, chatId, CONFIG.botId);
                return true;

            } catch (error: any) {
                if (error instanceof NetworkError) {
                    emitter.emit('alert', `Network error: Please check your internet connection and try again.`);

                } else if (error instanceof HttpError) {
                    if (error.status === 400) {
                        this.alertInfo = "Chat does not exist or the application is not a member"
                    } else {
                        emitter.emit('alert', `HTTP error ${error.status}: ${error.message}`);
                    }
                } else if (error instanceof InvalidJsonError) {
                    emitter.emit('alert', `Invalid JSON: ${error.message}`);
                } else {
                    emitter.emit('alert', `Unknown error: ${String(error)}`);
                }

                return false;
            }

        },

        async getChatTittile(chatId: string) {
            try {
                const result: ChatInfo = await getChatInfo(CONFIG.token, chatId);
                return result.title

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
                return null;
            }
        },

        async handleChatIdSubmit(chatId: string) {
            const title = await this.getChatTittile(chatId);
            if (title && chatId) {
                const chat = new Chat(chatId, title);
                this.$emit('newChat', chat);
            }
        },


        handleGoBack() {
            this.$emit('goBack');
        }
    },
}

);
</script>