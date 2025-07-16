<template>
    <Welcome v-if="stepStack[stepStack.length - 1] === StepStartupApp.Welcome" @addToken="onButtonStart" />
    <Token v-if="stepStack[stepStack.length - 1] === StepStartupApp.Token" @tokenAndBotId="onAddTokenAndChat"
        @goBack="handleGoBack" />
</template>


<script lang="ts">
import { defineComponent } from 'vue';
import { StepStartupApp } from '../../../interface';
import Welcome from './pageWelcome/Welcome.vue';
import Token from './pageToken/Token.vue'; './pageToken/Token.vue';



export default defineComponent({
    name: 'MainStartup',
    components: {
        Welcome,
        Token,
    },
    emits: ['tokenAndBotId'],
    data(): {
        stepStack: StepStartupApp[],
        StepStartupApp: typeof StepStartupApp,
    } {
        return {
            stepStack: [StepStartupApp.Welcome],
            StepStartupApp: StepStartupApp,
        };
    },

    methods: {
        onButtonStart() {
            this.stepStack.push(StepStartupApp.Token);
        },



        onAddTokenAndChat(data: { token: string, botId: string }) {
            this.$emit('tokenAndBotId', data);
        },


        handleGoBack() {
            if (this.stepStack.length - 1 > 0) {
                this.stepStack.pop();
            }
        }
    }

});
</script>