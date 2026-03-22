<template>
    <button @click="deviceSelected"
        class="flex items-center justify-between  text-lg font-bold shadow-md w-full text-white/90 p-1.5 bg-black/60 rounded-4xl hover:shadow-[0_0_10px_3px_rgba(255,255,255,0.8)] hover:bg-black/95 hover:scale-105 transition">
        <span class="flex-1 text-center select-none">{{ item.title }}</span>
    </button>

</template>

<script lang="ts">
import { defineComponent, type PropType } from 'vue';
import type { ItemData } from '../../../../basic/types/itemData';

export default defineComponent({
    name: 'SelectDeviceItem',
    emits: ['deviceSelected'],
    props: {
        item: {
            type: Object as PropType<ItemData>,
            required: true
        },
    },
    methods: {
        deviceSelected() {
            this.$emit('deviceSelected', this.item.id);
        },
    }
});
</script>
