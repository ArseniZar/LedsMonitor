<template>
    <Welcome v-if="stepStack[stepStack.length - 1] === StepStartupApp.Welcome" @addToken="onButtonStart" />
    <Token v-if="stepStack[stepStack.length - 1] === StepStartupApp.Token" @addBot="onAddBot"
        @goBack="handleGoBack" />
</template>


<script lang="ts">
import { defineComponent } from 'vue';
import { StepStartupApp } from '../../../basic/types/steps';
import Welcome from './pageWelcome/Welcome.vue';
import Token from './pageToken/Token.vue';
import type { Bot } from '../../../basic/classes/Bot';



export default defineComponent({
    name: 'MainStartup',
    components: {
        Welcome,
        Token,
    },
    emits: ['addBot'],
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



        onAddBot(bot: Bot) {
            this.$emit('addBot', bot);
        },


        handleGoBack() {
            if (this.stepStack.length - 1 > 0) {
                this.stepStack.pop();
            }
        }
    }

});
</script>