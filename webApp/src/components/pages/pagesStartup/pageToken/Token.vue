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
            alertInfo: 'Enter a valid token',
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
                    this.alertInfo = 'Network issue, please try again later';

                } else if (error instanceof HttpError) {
                    this.alertInfo = 'Invalid token, please check';

                } else if (error instanceof InvalidJsonError) {
                    console.error('Некорректный JSON:', error.message);

                } else {
                    this.alertInfo = 'An error occurred: ' + error.message;
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
                    this.alertInfo = 'Network issue, please try again later';

                } else if (error instanceof HttpError) {
                    this.alertInfo = 'Invalid token, please check';

                } else if (error instanceof InvalidJsonError) {
                    console.error('Некорректный JSON:', error.message);

                } else {
                    this.alertInfo = 'An error occurred: ' + error.message;
                }
                return "";
            }
        },


        async handleTokenSubmit(token: string) {
            const botId = await this.getBotId(token);
            this.$emit('tokenAndBotId', { token, botId });
        },

        handleGoBack() {
            this.$emit('goBack');

        },
    }

});
</script>