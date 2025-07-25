<template>
    <div class="w-full flex flex-row gap-2.5 bg-black/30 p-2 rounded-xl ">
        <img :src="item.imgIcon" alt="Check" class="h-6" />
        <span class="w-full font-bold select-none"> {{item.title}} </span>
    </div>
</template>


<script lang="ts">
import { defineComponent, type PropType } from 'vue';
import type { ItemData } from '../../../../basic/types/itemData';

export default defineComponent({
    name: 'DeviceItem',
    emits: [],
    props: {
        item: {
            type: Object as PropType<ItemData>,
            required: true
        },
    },
    methods: {
        
    }
}
);
</script>