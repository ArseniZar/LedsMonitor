<template>
    <div class=" w-full h-full flex items-center gap-1 flex-col  rounded-xl">
        <img :src="item.imgIcon" alt="Check" class="h-20" />
        <span class="font-bold text-center text-2xl"> {{ item.title }}</span>
    </div>
</template>


<script lang="ts">
import { defineComponent, type PropType } from 'vue';
import type { ItemData } from '../../../../interface';

export default defineComponent({
    name: 'CurrentSettingsChat',
    emits: [],
    props: {
        item: {
            type: Object as PropType<ItemData>,
            required: true
        },
    },
    methods: {
    }
});
</script>