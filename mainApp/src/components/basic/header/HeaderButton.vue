<template>
    <button @click="event"
        class="font-bold text-xl border-0  h-full p-1 rounded-full text-white/90 hover:shadow-[0_0_10px_3px_rgba(255,255,255,0.5)] hover:text-white/100 hover:scale-110  duration-200">
        <img v-if="data.imgIcon" :src="getImage(data.imgIcon)" alt="icon" class="h-6" />
        <span v-if="data.title" class="font-bold">
            {{ data.title }}
        </span>
    </button>
</template>

<script lang="ts">
import { defineComponent, type PropType } from 'vue';
import type { ItemData } from '../../../basic/types/itemData';
import { getPreloadedImage } from '../../../basic/utils/imagePreloader';

export default defineComponent({
    name: 'HeaderButton',
    emits: ['event'],
    props: {
        data: {
            type: Object as PropType<ItemData>,
            required: true

        }
    },
    methods: {
        event() {
            this.$emit('event', this.data.id);
        },
        getImage(url: string): string | undefined {
            const img = getPreloadedImage(url);
            return img ? img.src : undefined;
        }
    }
});
</script>