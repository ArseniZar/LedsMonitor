<template>
    <div className="flex  items-center">
        <button @click="selectDevice"
            className="text-center h-8 font w-full shadow-md text-white/90 mx-auto p-1 bg-black/30 rounded-4xl hover:shadow-[0_0_10px_3px_rgba(255,255,255,0.8)] hover:scale-105  hover:bg-black/95  transition block">
            {{ device.name }}</button>
    </div>
</template>

<script lang="ts">
import { defineComponent } from 'vue';
import type { Device } from '../../interface';

export default defineComponent({
    name: 'ButtonSelect',
    emits: ['selectDevice'],
    props: {
        device: {
            type: Object as () => Device,
            required: true
        },
    },
    methods: {
        selectDevice() {
            this.$emit('selectDevice');
        },
    }
});
</script>