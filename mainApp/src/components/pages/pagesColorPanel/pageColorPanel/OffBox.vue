<template>
    <label class="block mt-5 cursor-pointer h-6 w-full">
        <input @change="onSelect" type="radio" :checked="modelValue === Number(colorBox.numberBox)"
            name="color" :value="colorBox.numberBox" class="hidden peer" />
        <div
            class="w-full h-full rounded-xl  transition duration-150  transform
             peer-checked:scale-110  bg-red-600/80 peer-checked:translate-y-[-3px] hover:scale-110 hover:shadow-[0_0_10px_1px_rgba(255,255,255,0.8)] peer-checked:shadow-[0_0_10px_1px_rgba(255,255,255,0.8)]">
            <span class="text-center font-bold text-white/90 block">OFF</span>
        </div>
    </label>
</template>


<script lang="ts">
import type { PropType } from 'vue';
import type { BoxColor } from '../../../../basic/classes/Device';

export default {
    name: 'OffBox',
    emits: ['onSelect'],
    props: {
        colorBox: {
            type: Object as PropType<BoxColor>,
            required: true
        },
        modelValue: {
            type: Number,
            required: true,
        }
    },
    methods: {
        onSelect(event: Event) {
            const input = event.target as HTMLInputElement;
            this.$emit('onSelect', input.value);
        },
    }
};
</script>
