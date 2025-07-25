<template>
    <FormInput :alert="alertInfo" :text="'Enter your bot token from WebApp'" :validData="validateToken"
        :headerStatus="true" @submit="handleTokenSubmit" @goBack="handleGoBack" />
</template>


<script lang="ts">
import { defineComponent } from 'vue';
import FormInput from '../../../basic/FormInput.vue';
import { getBotInfo } from '../../../../telegram/api';
import type { BotInfo } from '../../../../telegram/types';
import { HttpError, InvalidJsonError, NetworkError } from '../../../../telegram/errors';
import emitter from '../../../../basic/eventBus';

export default defineComponent({
    name: 'Token',
    components: {
        FormInput,
    },
    emits: ['goBack', 'tokenAndBotId'],
    data(): {
        alertInfo: string,
    } {
        return {
            alertInfo: '',
        };
    },

    methods: {

        async validateToken(token: string): Promise<boolean> {
            try {
                await getBotInfo(token);
                return true;
            }
            catch (error: any) {
                if (error instanceof NetworkError) {
                    emitter.emit('alert', `Network error: Please check your internet connection and try again.`);
                } else if (error instanceof HttpError) {
                    if (error.status === 401 || error.status === 404) {
                        this.alertInfo = "Invalid token. Please enter a valid value."
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

        async getBotId(token: string) {
            try {
                const result: BotInfo = await getBotInfo(token);
                return result.id;
            }
            catch (error: any) {
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


        async handleTokenSubmit(token: string) {
            const botId = await this.getBotId(token);
            if (botId) {
                this.$emit('tokenAndBotId', { token, botId });
            }
        },

        handleGoBack() {
            this.$emit('goBack');

        },
    }

});
</script>