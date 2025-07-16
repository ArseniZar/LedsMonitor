<template>
    <button @click="handleClickItem"
        class="flex  px-3 flex-row gap-4 w-full p-2.5 rounded-xl hover:scale-105 active:bg-black/80 transition-transform duration-400">
        <img v-if="item.imgIcon" :src="item.imgIcon" alt="Check" class="h-6" />
        <p class="text-white/90 text-left w-full break-words font-bold ">{{ item.title }} </p>
        <img :src="'/public/img/icons8-dep-50.png'" alt="Check" class="h-6" />
    </button>

</template>


<script lang="ts">
import { defineComponent, type PropType } from 'vue';
import type { ItemData } from '../../../../interface';

export default defineComponent({
    name: 'SettingsItem',
    emits: ['clickItem'],
    props: {
        item: {
            type: Object as PropType<ItemData>,
            required: true
        },
    },
    methods: {
        handleClickItem() {
            this.$emit('clickItem', this.item.id);
        }
    }
});
</script>