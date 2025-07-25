<template>
    <div
        class="flex flex-row  gap-4 items-center shadow-md w-full justify-between p-1.5 px-4 rounded-4xl select-none  bg-black/60  hover:shadow-[0_0_10px_3px_rgba(255,255,255,0.8)] hover:bg-black/95  active:bg-black/95 hover:scale-105 transition">
        <img :src="device.imgIcon" alt="Check" class="h-6 w-6" />
        <span class="flex-1 text-lg font-bold text-white/90 ">{{ device.title }}</span>
        <button @click="deleteDevice">
            <img :src="'img/icons8-close-52.png'" alt="Check" class="h-5 w-5" />
        </button>
    </div>
</template>

<script lang="ts">
import { defineComponent, type PropType } from 'vue';
import type { ItemData } from '../../../../basic/types/itemData';

export default defineComponent({
    name: 'Device',
    emits: ['deviceSelected', 'deleteDevice'],
    props: {
        device: {
            type: Object as PropType<ItemData>,
            required: true,
        }

    },
    methods: {
        deleteDevice() {
            this.$emit('deleteDevice', this.device.id);
        },
    }
});
</script>
