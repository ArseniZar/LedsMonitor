<template>
    <button @click="deviceSelected"
        class="text-center font-bold shadow-md w-full text-white/90  p-1.5  bg-black/60 rounded-4xl hover:shadow-[0_0_10px_3px_rgba(255,255,255,0.8)] hover:bg-black/95 hover:scale-105  transition block">
        {{ device.name }}
    </button>
</template>

<script lang="ts">
import { defineComponent } from 'vue';
import type { Device } from '../../interface';

export default defineComponent({
    name: 'DeviceItem',
    props: {
        device: {
            type: Object as () => Device,
            required: true
        }
    },
    methods: {
        deviceSelected() {
            this.$emit('deviceSelected', this.device);
        }
    }
});
</script>
