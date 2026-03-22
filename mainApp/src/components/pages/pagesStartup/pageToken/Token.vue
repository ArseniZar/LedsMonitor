<template>
    <FormInput :text="'Enter your bot token'" :validData="validateToken" @submit="handleTokenSubmit"
        @goBack="handleGoBack" />
</template>


<script lang="ts">
import { defineComponent } from 'vue';
import type { User } from '@telegraf/types';

import FormInput from '../../../basic/FormInput.vue';
import { getBot, validateToken, type ResultData } from '../../../../telegram/telegramService';
import { Bot } from '../../../../basic/classes/Bot';

export default defineComponent({
    name: 'Token',
    components: {
        FormInput,
    },
    emits: ['goBack', 'addBot'],
    data(): {
    } {
        return {
        };
    },

    methods: {

        async validateToken(token: string): Promise<ResultData<unknown>> {
            return await validateToken(token);
        },

        async handleTokenSubmit(token: string) {
            const result: ResultData<User> = await getBot(token);
            if (result.status) {
                const botInfo = result.value as User;
                const bot = new Bot(token, botInfo.first_name, String(botInfo.id));
                this.$emit('addBot', bot);
            }
        },

        handleGoBack() {
            this.$emit('goBack');

        },
    }

});
</script>