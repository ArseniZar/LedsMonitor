<template>
    <button @click="goBack"
        class="font-bold text-xl border-0  rounded-full w-7 h-7 text-white/90 hover:shadow-[0_0_10px_3px_rgba(255,255,255,0.8)] hover:text-white/100 hover:scale-110  duration-200">
        < </button>
</template>

<script lang="ts">
import { defineComponent } from 'vue';

export default defineComponent({
    name: 'BackPage',
    props: {},
    methods: {
        goBack() {
            this.$emit('goBack');
        }
    }
});
</script>