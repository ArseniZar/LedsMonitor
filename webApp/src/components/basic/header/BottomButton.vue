<template>
    <label>
        <div class="flex flex-col items-center">
            <input type="radio" name="bar" :checked="checked" class="hidden peer" @change="event" />
            <img v-if="data.imgIcon" :src="getImage(data.imgIcon)" alt="icon"
                class="h-7 w-7 peer-checked:scale-115 transition-transform duration-300" />
            <span v-if="data.title"
                class="select-none text-white/90  bg-black/40 px-6  rounded-4xl text-xl w-full break-words font-bold  peer-checked:scale-110 transition-transform duration-300">
                {{ data.title }} </span>
            <div
                class="mt-1.5 w-full max-w-20 h-1 rounded-4xl bg-white/10 peer-checked:bg-white/90 peer-checked:shadow-[0_0_10px_1px_rgba(255,255,255,0.8)] transition-colors duration-200 ">
            </div>
        </div>
    </label>
</template>


<script lang="ts">
import { defineComponent, type PropType } from 'vue';
import type { ItemData } from '../../../basic/types/itemData';
import { getPreloadedImage } from '../../../basic/utils/imagePreloader';

export default defineComponent({
    name: 'ButtomButton',
    emits: ['event'],
    props: {
        data: {
            type: Object as PropType<ItemData>,
            required: true

        },
        checked: {
            type: Boolean,
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